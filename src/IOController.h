#ifndef IO_CONTROLLER_H
#define IO_CONTROLLER_H

#include <Arduino.h>

// GPIO Mapping for ESP32 DevKit V1
#define PIN_DI1 34 // Input Only (No Pull-up/down internal)
#define PIN_DI2 35 // Input Only (No Pull-up/down internal)
#define PIN_DI3 32
#define PIN_DI4 33
#define PIN_DI5 25
#define PIN_DI6 26
#define PIN_DI7 27
#define PIN_DI8 14

#define PIN_DO1 2 // Onboard LED
#define PIN_DO2 4
#define PIN_DO3 5
#define PIN_DO4 18
#define PIN_DO5 19
#define PIN_DO6 21

/*
 * Class: IOController
 * Description: Hardware abstraction layer. Handles reading digital/analog inputs
 * and writing to digital outputs.
 */
class IOController
{
public:
    // State variables
    bool DI1, DI2, DI3, DI4, DI5, DI6, DI7, DI8;
    bool DO1, DO2, DO3, DO4, DO5, DO6;
    int AN1, AN2, AN3, AN4;

    IOController() {}

    // Initialize Pin Modes
    void begin()
    {
        pinMode(PIN_DI1, INPUT);
        pinMode(PIN_DI2, INPUT);
        pinMode(PIN_DI3, INPUT_PULLUP);
        pinMode(PIN_DI4, INPUT_PULLUP);
        pinMode(PIN_DI5, INPUT_PULLUP);
        pinMode(PIN_DI6, INPUT_PULLUP);
        pinMode(PIN_DI7, INPUT_PULLUP);
        pinMode(PIN_DI8, INPUT_PULLUP);

        pinMode(PIN_DO1, OUTPUT);
        pinMode(PIN_DO2, OUTPUT);
        pinMode(PIN_DO3, OUTPUT);
        pinMode(PIN_DO4, OUTPUT);
        pinMode(PIN_DO5, OUTPUT);
        pinMode(PIN_DO6, OUTPUT);
    }

    // Read Inputs and Write Outputs
    void update()
    {
        // Read Inputs (Inverted logic for Pull-Up configuration)
        DI1 = !digitalRead(PIN_DI1);
        DI2 = !digitalRead(PIN_DI2);
        DI3 = !digitalRead(PIN_DI3);
        DI4 = !digitalRead(PIN_DI4);
        DI5 = !digitalRead(PIN_DI5);
        DI6 = !digitalRead(PIN_DI6);
        DI7 = !digitalRead(PIN_DI7);
        DI8 = !digitalRead(PIN_DI8);

        // Write Outputs
        digitalWrite(PIN_DO1, DO1);
        digitalWrite(PIN_DO2, DO2);
        digitalWrite(PIN_DO3, DO3);
        digitalWrite(PIN_DO4, DO4);
        digitalWrite(PIN_DO5, DO5);
        digitalWrite(PIN_DO6, DO6);

        // Read Analog Channels
        AN1 = analogRead(36); // VP
        AN2 = analogRead(39); // VN
        AN3 = analogRead(32);
        AN4 = analogRead(33);
    }
};

#endif