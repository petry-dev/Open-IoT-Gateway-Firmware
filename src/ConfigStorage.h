#ifndef CONFIG_STORAGE_H
#define CONFIG_STORAGE_H

#include <Preferences.h>
#include <Arduino.h>

/*
 * Class: ConfigStorage
 * Description: Handles persistent storage of configuration parameters 
 * using the ESP32 Preferences library (NVS).
 */
class ConfigStorage
{
private:
    Preferences prefs;

public:
    struct Config
    {
        String ssid, password, cncIp, mqttServer, mqttUser, mqttPassword, machineType, activeSensors;
    } config;

    void begin() { prefs.begin("config", false); }

    // Loads configuration from NVS
    void load()
    {
        prefs.begin("config", true); // Read-only mode
        config.ssid = prefs.getString("ssid", "");
        config.password = prefs.getString("pass", "");
        config.cncIp = prefs.getString("cncIp", "");
        config.mqttServer = prefs.getString("mqttSrv", "");
        config.mqttUser = prefs.getString("mqttUser", "");
        config.mqttPassword = prefs.getString("mqttPass", "");
        config.machineType = prefs.getString("machType", "");
        config.activeSensors = prefs.getString("sensors", "");
        prefs.end();
    }

    // Saves configuration to NVS
    void save(const Config &cfg)
    {
        prefs.begin("config", false); // Read/Write mode
        prefs.putString("ssid", cfg.ssid);
        prefs.putString("pass", cfg.password);
        prefs.putString("cncIp", cfg.cncIp);
        prefs.putString("mqttSrv", cfg.mqttServer);
        prefs.putString("mqttUser", cfg.mqttUser);
        prefs.putString("mqttPass", cfg.mqttPassword);
        prefs.putString("machType", cfg.machineType);
        prefs.putString("sensors", cfg.activeSensors);
        prefs.end();
    }

    // Clears all stored data (Factory Reset)
    void clear()
    {
        prefs.begin("config", false);
        prefs.clear();
        prefs.end();
    }
};

#endif