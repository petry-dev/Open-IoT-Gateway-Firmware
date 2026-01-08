#ifndef ConfigWifi_h
#define ConfigWifi_h

#include <WiFi.h>
#include <WebServer.h>
#include <LittleFS.h>
#include "ConfigStorage.h"

extern ConfigStorage storage;

/*
 * Class: ClassWifiController
 * Description: Manages Wi-Fi connections (Station/AP), WebServer for configuration,
 * and validation of user settings.
 */
class ClassWifiController
{
private:
    String ssid, password, cncIp, mqttServer, mqttUser, mqttPassword;
    String networkListHTML;
    String _jsonCache = "[]";
    WebServer server;

    String loadHtml(String path);
    String loadJsonConfig();

public:
    // Helper to format CNC IP address
    const char *getCncIp()
    {
        static String u;
        u = "http://" + cncIp + ":5134/current";
        return u.c_str();
    }
    
    // Getters
    const char *getSsid() { return ssid.c_str(); }
    const char *getMqttServer() { return mqttServer.c_str(); }
    const char *getMqttUser() { return mqttUser.c_str(); }
    const char *getMqttPassword() { return mqttPassword.c_str(); }

    WiFiClient client() { return WiFiClient(); }
    
    // Lifecycle methods
    bool mBegin();
    void mLoop();
    void waitForConnection();
    bool mConnected();
    void scanNetworks();
    void applyLoadedConfig(const ConfigStorage::Config &cfg);
    
    // Web Server Handlers
    void handleRoot();
    void handleSave();
    void handleScanNetworks();
    void handleValidate();
};

#endif