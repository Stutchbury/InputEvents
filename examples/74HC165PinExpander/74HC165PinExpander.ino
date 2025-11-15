/**
 * Example of connecting two buttons with 74HC165 Shift Register
 * using ExpanderPinAdapter and HC165ExpanderAdapter.
 *
 * Key Features:
 * - Uses HC165ExpanderAdapter to read states of multiple buttons
 * - Supports reading up to 32 pin states by chaining four 74HC165 registers
 * - Requires only 3 Arduino pins to read multiple button states
 *
 * Button Connection Details for this example:
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

HC165ExpanderAdapter expanderAdapter(DATA_PIN, CLOCK_PIN, SHLD_PIN);

//HC165ExpanderAdapter expanderAdapter(DATA_PIN, CLOCK_PIN, SHLD_PIN, 4); // chain 1-4 74HC165 (default is 1)

#define NUM_BUTTONS 2

//Create the pins and link them to the expanderAdapter
EventButton buttons[NUM_BUTTONS] = {
    new ExpanderPinAdapter(0, expanderAdapter),
    new ExpanderPinAdapter(7, expanderAdapter),
};

/**
 * Utility function to print the button events to Serial.
 * You don't need this - it's just for the example.
 * See other examples for other event types
 */
void printEvent(InputEventType et) {
  switch (et) {
  case InputEventType::ENABLED :
    Serial.print("ENABLED");
    break;
  case InputEventType::DISABLED :
    Serial.print("DISABLED");
    break;
  case InputEventType::IDLE :
    Serial.print("IDLE");
    break;
  case InputEventType::PRESSED :
    Serial.print("PRESSED");
    break;
  case InputEventType::RELEASED :
    Serial.print("RELEASED");
    break;
  case InputEventType::CLICKED :
    Serial.print("CLICKED");
    break;
  case InputEventType::DOUBLE_CLICKED :
    Serial.print("DOUBLE_CLICKED");
    break;
  case InputEventType::MULTI_CLICKED :
    Serial.print("MULTI_CLICKED");
    break;
  case InputEventType::LONG_CLICKED :
    Serial.print("LONG_CLICKED");
    break;
  case InputEventType::LONG_PRESS :
    Serial.print("LONG_PRESS");
    break;
  default:
    Serial.print("Unknown event: ");
    Serial.print((uint8_t)et);
    break;
  }
}


/**
 * A function to handle the events
 * Can be called anything but requires InputEventType and
 * EventButton& defined as parameters.
 */
void onButtonEvent(InputEventType et, EventButton &eb) {
  Serial.print("onButtonEvent for button ");
  Serial.print(eb.getInputId());
  Serial.print(": ");
  printEvent(et); //Call the utility event to print the event type
  if ( et == InputEventType::MULTI_CLICKED ) {
    Serial.print(" - ");
    Serial.print(eb.clickCount());
    Serial.print(" clicks"); 
  }
  Serial.println();
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    expanderAdapter.begin();
    
    // connect buttons to events
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].begin();
        buttons[i].setInputId(i);
        buttons[i].setCallback(onButtonEvent);
    }

    Serial.println("init complete.");
}

void loop() {
    // You must call update() to refresh the 74HC165(s)
    expanderAdapter.update();

    // You must call update() for every defined EventButton.
    // This will update the state of the button and 
    // fire the appropriate events.
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].update();
    }
}