/**
 * Example of connecting two buttons with 74HC165 Shift Register
 * using ExpanderPinAdapter and HC165ExpanderAdapter.
 *
 * Key Features:
 * - Uses HC165ExpanderAdapter to read states of multiple buttons
 * - Supports reading up to 32 pin states by chaining four 74HC165 registers
 * - Requires only 3 Arduino pins to read multiple button states
 *
 * Button Connection Details:
 * - Button 1: Connected to 74HC165's pin A (bit 0 in the register)
 * - Button 2: Connected to 74HC165's pin H (bit 7 in the register)
 *
 */

#include "Arduino.h"

#include <EventButton.h>
#include <GpioExpanderAdapter/HC165ExpanderAdapter.h>
#include <PinAdapter/ExpanderPinAdapter.h>

#define DATA_PIN 2  // Qh pin on 74HC165
#define CLOCK_PIN 3 // CLK pin on 74HC165
#define SHLD_PIN 4  // SH/LD pin on 74HC165

HC165ExpanderAdapter e(DATA_PIN, CLOCK_PIN, SHLD_PIN);

#define NUM_BUTTONS 2
EventButton buttons[NUM_BUTTONS] = {
    new ExpanderPinAdapter(0, &e),
    new ExpanderPinAdapter(7, &e),
};

void onButtonEvent(InputEventType et, EventButton &eb) {
    if (et == InputEventType::PRESSED) {
        Serial.print("button ");
        Serial.print(eb.getInputId());
        Serial.println(" pressed");
    } else if (et == InputEventType::RELEASED) {
        Serial.print("button ");
        Serial.print(eb.getInputId());
        Serial.println(" released");
    }
}

void setup() {
    Serial.begin(115200);

    // connect buttons to events
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].begin();
        buttons[i].setInputId(i);
        buttons[i].setCallback(onButtonEvent);
    }

    Serial.println("init complete.");
}

void loop() {
    // refresh 74HC165
    e.update();

    // trigger button events
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].update();
    }
}