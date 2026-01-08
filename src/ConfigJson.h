#ifndef CONFIG_JSON_H
#define CONFIG_JSON_H

#include <PubSubClient.h>
#include <ArduinoJson.h>

/*
 * Class: ClassJsonConfig
 * Description: Helper to serialize ArduinoJson documents and publish 
 * them via MQTT.
 */
class ClassJsonConfig
{
private:
    PubSubClient &mqttClient;

public:
    ClassJsonConfig(PubSubClient &client) : mqttClient(client) {}

    void mEnvioDadosJson(JsonDocument &doc, const char *topic)
    {
        char buffer[1024];
        // Serialize JSON to buffer
        size_t n = serializeJson(doc, buffer);
        
        if (mqttClient.connected()) {
            mqttClient.publish(topic, buffer, n);
        }
    }
};

#endif