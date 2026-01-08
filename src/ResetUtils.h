#ifndef RESET_UTILS_H
#define RESET_UTILS_H

#include <Arduino.h>
#include "ConfigStorage.h"

#define WIFI_RESET_PIN 0 // Generally the BOOT button on ESP32 DevKits

/*
 * Function: checkResetButtonInterrupt
 * Description: Monitors the BOOT button. If held for >3 seconds,
 * clears NVS configuration (Factory Reset) and reboots.
 */
void checkResetButtonInterrupt()
{
    if (digitalRead(WIFI_RESET_PIN) == LOW) {
        unsigned long start = millis();
        while (digitalRead(WIFI_RESET_PIN) == LOW) {
            // Check for 3-second long press
            if (millis() - start > 3000) {
                Serial.println("Factory Reset...");
                extern ConfigStorage storage;
                storage.clear();
                delay(500);
                ESP.restart();
            }
        }
    }
}

#endif