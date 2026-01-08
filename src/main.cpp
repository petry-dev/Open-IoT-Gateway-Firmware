/*
 * Project: Open IoT Gateway (Generic ESP32 Version)
 * Description: Main firmware entry point. Handles Wi-Fi modes, MQTT communication,
 * and hardware I/O orchestration.
 */

#include <HTTPClient.h>
#include "ConfigWifi.h"
#include "ConfigJson.h"
#include <PubSubClient.h>
#include "IOController.h"
#include "MTConnect2MQTT.h"
#include "ConfigStorage.h"
#include "ResetUtils.h"

// Hardware and Controller Initialization
IOController IO;
ClassWifiController WifiController;
ConfigStorage storage;

// Timing variables
static unsigned long lastSendTime = 0;
static unsigned time2send = 1000; // Data transmission interval (ms)
static unsigned long ap_mode_start_time = 0;
const unsigned long ap_reboot_timeout = 300000; // Reboot after 5 min in AP mode if no client connects
static unsigned long lastMqttAttempt = 0;

// Network Objects
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
MTConnect2MQTT mtconnect2mqtt(mqttClient);
ClassJsonConfig JsonConfig(mqttClient);
StaticJsonDocument<1024> doc;

String deviceId = "";

void setup()
{
    Serial.begin(115200);
    delay(100);

    // Initialize Hardware I/O
    IO.begin();

    // Visual Boot Feedback: Blink LED 3 times
    for (int i = 0; i < 3; i++) {
        IO.DO1 = true;
        IO.update();
        delay(100);
        IO.DO1 = false;
        IO.update();
        delay(100);
    }

    // Configure Wi-Fi sleep and reset button
    WiFi.setSleep(false);
    pinMode(WIFI_RESET_PIN, INPUT_PULLUP);

    // Load Configuration from NVS (Non-Volatile Storage)
    storage.begin();
    storage.load();
    WifiController.applyLoadedConfig(storage.config);

    // Attempt to connect to saved Wi-Fi; otherwise start Access Point
    if (WifiController.mBegin()) {
        Serial.println("Starting Station Mode...");
    } else {
        Serial.println("Starting AP Mode...");
        ap_mode_start_time = millis();
    }

    // Generate Device ID from MAC Address
    String mac = WiFi.macAddress();
    mac.replace(":", "");
    deviceId = mac.substring(6);
    Serial.println("ID: " + deviceId);

    // Configure MQTT Server
    mqttClient.setServer(WifiController.getMqttServer(), 1883);
    mqttClient.setBufferSize(102400); // Increase buffer for large payloads
}

// Handles MQTT connection and reconnection logic
void manageMqttConnection()
{
    if (!mqttClient.connected()) {
        unsigned long now = millis();
        // Retry every 5 seconds
        if (now - lastMqttAttempt > 5000) {
            lastMqttAttempt = now;
            Serial.print("MQTT Connecting... ");
            String clientId = "Gateway-" + deviceId;

            if (mqttClient.connect(clientId.c_str(), WifiController.getMqttUser(), WifiController.getMqttPassword())) {
                Serial.println("OK");
            } else {
                Serial.print("Fail rc=");
                Serial.println(mqttClient.state());
            }
        }
    } else {
        mqttClient.loop();
    }
}

// Collects sensor data and publishes to MQTT
void sendJsonData()
{
    unsigned long currentMillis = millis();

    if (WifiController.mConnected()) {
        manageMqttConnection();

        if (currentMillis - lastSendTime >= time2send) {
            // Determine machine type for topic structure
            String type = storage.config.machineType;
            if (type == "") {
                type = "generic";
            }
            String baseTopic = "industry/" + type + "-" + deviceId;

            // Populate JSON Document
            doc.clear();
            doc["ssid"] = WifiController.getSsid();
            doc["id"] = deviceId;
            doc["rssi"] = WiFi.RSSI();

            // Digital Inputs
            doc["DI1"] = IO.DI1;
            doc["DI2"] = IO.DI2;
            doc["DI3"] = IO.DI3;
            doc["DI4"] = IO.DI4;
            // Analog Inputs
            doc["AN1"] = IO.AN1;

            // Publish I/O Data
            if (mqttClient.connected()) {
                String topicIO = baseTopic + "/io";
                JsonConfig.mEnvioDadosJson(doc, topicIO.c_str());
            }

            // Handle MTConnect Proxy if IP is configured
            if (storage.config.cncIp.length() > 0) {
                String topicMTC = baseTopic + "/mtconnect";
                HTTPClient httpClient;
                mtconnect2mqtt.fetchDataAndPublish(httpClient, WifiController.getCncIp(), topicMTC.c_str());
            }
            lastSendTime = currentMillis;
        }
    }
}

// Update Inputs/Outputs and handle Reset Button
void UpdateInputOutput()
{
    checkResetButtonInterrupt();
    IO.update();
    
    // Direct hardware feedback: Mirror DI1 to DO1 (LED)
    IO.DO1 = IO.DI1;
}

void loop()
{
    // Mode: Access Point (Configuration Mode)
    if (WiFi.getMode() == WIFI_AP || WiFi.getMode() == WIFI_AP_STA) {
        WifiController.mLoop();
        
        // Safety feature: Reboot if AP is idle for too long
        if (WiFi.getMode() == WIFI_AP && millis() - ap_mode_start_time > ap_reboot_timeout) {
            if (WiFi.softAPgetStationNum() == 0) {
                ESP.restart();
            }
        }
    }

    // Mode: Station (Normal Operation)
    if (WiFi.getMode() != WIFI_AP) {
        checkResetButtonInterrupt();
        UpdateInputOutput();
        sendJsonData();
    }
}