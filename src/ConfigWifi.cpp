#include "ConfigWifi.h"
#include <PubSubClient.h>
#include <HTTPClient.h>
#include "ConfigStorage.h"

// Helper: Loads HTML content from LittleFS
String ClassWifiController::loadHtml(String path)
{
    File file = LittleFS.open(path, "r");
    if (!file) {
        return "<h1>Error 404</h1>";
    }
    String html = file.readString();
    file.close();
    return html;
}

// Helper: Loads Machine Configuration JSON
String ClassWifiController::loadJsonConfig()
{
    File file = LittleFS.open("/web/config/machines.json", "r");
    if (!file) {
        return "{\"machines\":[], \"sensors\":[]}";
    }
    String json = file.readString();
    file.close();
    return json;
}

// Handles the Async Wi-Fi Scan request from the frontend
void ClassWifiController::handleScanNetworks()
{
    int n = WiFi.scanComplete();

    // Case 1: Scan failed or not started -> Start Async Scan
    if (n == -2) {
        WiFi.scanNetworks(true); // true = async
        server.send(200, "application/json", "[]"); // Tell frontend to wait
        return;
    }

    // Case 2: Scan still in progress -> Tell frontend to wait
    if (n == -1) {
        server.send(200, "application/json", "[]");
        return;
    }

    // Case 3: Scan Completed -> Build JSON and return results
    String json = "[";
    for (int i = 0; i < n; ++i) {
        if (i > 0) {
            json += ",";
        }
        json += "\"" + WiFi.SSID(i) + "\"";
    }
    json += "]";

    server.send(200, "application/json", json);
    // Cleanup scan data to free memory
    WiFi.scanDelete();
}

// Validates Wi-Fi and MQTT credentials without saving them
void ClassWifiController::handleValidate()
{
    String t_ssid = server.arg("ssid");
    String t_pass = server.arg("password");
    String t_mqtt = server.arg("mqttServer");

    bool wifiOk = false;
    bool mqttOk = true;
    String msg = "";

    // Step 1: Validate Wi-Fi Connection
    if (WiFi.SSID() != t_ssid || WiFi.status() != WL_CONNECTED) {
        WiFi.begin(t_ssid.c_str(), t_pass.c_str());
        unsigned long s = millis();
        // Wait up to 6 seconds for connection
        while (WiFi.status() != WL_CONNECTED && millis() - s < 6000) {
            delay(100);
        }
    }
    wifiOk = (WiFi.status() == WL_CONNECTED);

    // Step 2: Validate MQTT Connection (if provided)
    if (wifiOk && t_mqtt.length() > 0) {
        mqttOk = false;
        WiFiClient client;
        client.setTimeout(2);
        PubSubClient mqtt(client);
        mqtt.setServer(t_mqtt.c_str(), 1883);
        mqtt.setSocketTimeout(2);

        if (mqtt.connect("Test_Client")) {
            mqttOk = true;
            mqtt.disconnect();
        } else {
            msg = "MQTT Error.";
        }
    }

    if (msg == "" && wifiOk) {
        msg = "Success!";
    }

    // Return Validation Result
    String json = "{\"wifi\":" + String(wifiOk ? "true" : "false") +
                  ",\"mqtt\":" + String(mqttOk ? "true" : "false") +
                  ",\"cnc\":true,\"message\":\"" + msg + "\"}";
    server.send(200, "application/json", json);
}

bool ClassWifiController::mBegin()
{
    if (!LittleFS.begin(true)) {
        return false;
    }

    // Try to connect to saved Wi-Fi
    if (ssid.length() > 0) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), password.c_str());
        unsigned long s = millis();
        // Short timeout to avoid blocking boot
        while (WiFi.status() != WL_CONNECTED && millis() - s < 3000) {
            delay(100);
        }
    }

    // Setup Access Point for configuration
    WiFi.mode(WIFI_AP_STA);
    String mac = WiFi.macAddress();
    mac.replace(":", "");
    WiFi.softAP(("IoT-Gateway-" + mac.substring(6)).c_str(), "admin1234", 6);

    // Start initial scan to populate list
    WiFi.scanNetworks(true);

    // Configure Web Server
    server.serveStatic("/static", LittleFS, "/web/static");

    server.on("/", HTTP_GET, std::bind(&ClassWifiController::handleRoot, this));
    server.on("/scan", HTTP_GET, std::bind(&ClassWifiController::handleScanNetworks, this));
    server.on("/validate", HTTP_GET, std::bind(&ClassWifiController::handleValidate, this));
    server.on("/save", HTTP_POST, std::bind(&ClassWifiController::handleSave, this));
    server.begin();
    return true;
}

// Serves the main configuration page with template replacement
void ClassWifiController::handleRoot()
{
    String html = loadHtml("/web/templates/index.html");
    html.replace("{{NETWORK_LIST}}", networkListHTML);
    html.replace("{{SSID}}", storage.config.ssid);
    html.replace("{{PASS}}", storage.config.password);
    html.replace("{{SAVED_TYPE}}", storage.config.machineType);
    html.replace("{{SAVED_IP}}", storage.config.cncIp);
    html.replace("{{MQTT_SERVER}}", storage.config.mqttServer);
    html.replace("{{MQTT_USER}}", storage.config.mqttUser);
    html.replace("{{MQTT_PASS}}", storage.config.mqttPassword);
    html.replace("{{SAVED_SENSORS}}", storage.config.activeSensors);
    html.replace("{{ MACHINES_JSON }}", loadJsonConfig());
    server.send(200, "text/html", html);
}

void ClassWifiController::mLoop()
{
    server.handleClient();

    // Check for completed background scans to update the list
    int n = WiFi.scanComplete();
    if (n >= 0) {
        networkListHTML = "";
        String found = "|";
        for (int i = 0; i < n; ++i) {
            String s = WiFi.SSID(i);
            // Filter duplicates
            if (s.length() > 0 && found.indexOf("|" + s + "|") == -1) {
                char buf[150];
                sprintf(buf, "<option value='%s'>%s (%d dBm)</option>", s.c_str(), s.c_str(), WiFi.RSSI(i));
                networkListHTML += String(buf);
                found += s + "|";
            }
        }
    }
}

// Saves configuration to NVS and Restarts
void ClassWifiController::handleSave()
{
    if (server.hasArg("ssid")) storage.config.ssid = server.arg("ssid");
    if (server.hasArg("password")) storage.config.password = server.arg("password");
    if (server.hasArg("machineType")) storage.config.machineType = server.arg("machineType");
    if (server.hasArg("cncIp")) storage.config.cncIp = server.arg("cncIp");
    if (server.hasArg("mqttServer")) storage.config.mqttServer = server.arg("mqttServer");
    if (server.hasArg("mqttUser")) storage.config.mqttUser = server.arg("mqttUser");
    if (server.hasArg("mqttPassword")) storage.config.mqttPassword = server.arg("mqttPassword");

    if (server.hasArg("activeSensors")) {
        storage.config.activeSensors = server.arg("activeSensors");
    } else {
        storage.config.activeSensors = "";
    }

    storage.save(storage.config);

    server.send(200, "application/json", "{\"status\":\"saved\", \"wait\": 2000}");

    delay(500);
    ESP.restart();
}

void ClassWifiController::applyLoadedConfig(const ConfigStorage::Config &cfg)
{
    ssid = cfg.ssid;
    password = cfg.password;
    cncIp = cfg.cncIp;
    mqttServer = cfg.mqttServer;
    mqttUser = cfg.mqttUser;
    mqttPassword = cfg.mqttPassword;
}

bool ClassWifiController::mConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void ClassWifiController::waitForConnection()
{
    while (!mConnected()) {
        mLoop();
        delay(500);
    }
}