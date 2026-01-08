#ifndef MTCONNECT2MQTT_H
#define MTCONNECT2MQTT_H

#include <WiFiClient.h>
#include <PubSubClient.h>
#include <HTTPClient.h>

/*
 * Class: MTConnect2MQTT
 * Description: Fetches data from an MTConnect Agent (via HTTP) and republishes 
 * it to an MQTT broker.
 */
class MTConnect2MQTT
{
private:
    PubSubClient &client;

public:
    MTConnect2MQTT(PubSubClient &_client) : client(_client) {}

    void fetchDataAndPublish(HTTPClient &http, const char *url, const char *topic)
    {
        http.begin(url);
        int httpCode = http.GET();
        
        if (httpCode > 0) {
            String payload = http.getString();
            // Ensure MQTT is connected before publishing
            if (client.connected()) {
                client.publish(topic, payload.c_str());
            }
        }
        http.end();
    }
};

#endif