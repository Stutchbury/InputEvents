/**
 * Example of connecting four buttons using the ExpanderPinAdapter and 
 * one of the following Adafruit GPIO expander adapters:
 * - AdafruitMCP23017ExpanderAdapter for Adafruit's MCP23017 library
 * - AdafruitPCF8574ExpanderAdapter for Adafruit's PCF8574 library
 * - AdafruitPCF8575ExpanderAdapter for Adafruit's PCF8575 library
 *
 * You will need to install the appropriate Adafurit GPOI Expander library and
 * comment in/out the approriate adapter in the #include and creation lines below
 * 
 * Supports multiple I2C addresses, set via begin(<addr>) below.
 *
 */

#include "Arduino.h"

#include <EventButton.h>
// We will use the ExpanderPinAdapter
#include <PinAdapter/ExpanderPinAdapter.h>

//Include the required Expander Adapter:
// #include <GpioExpanderAdapter/AdafruitMCP23017ExpanderAdapter.h>
// #include <GpioExpanderAdapter/AdafruitPCF8574ExpanderAdapter.h>
#include <GpioExpanderAdapter/AdafruitPCF8575ExpanderAdapter.h>

//Create the required expander adapter:
// AdafruitMCP23017ExpanderAdapter expanderAdapter;
// AdafruitPCF8574ExpanderAdapter expanderAdapter;
AdafruitPCF8575ExpanderAdapter expanderAdapter;

// Or create and pass a separate instance if you want to retain access to the base library methods
// Adafruit_PCF8575 pcf;
// AdafruitPCF8575ExpanderAdapter expanderAdapter(pcf);


#define NUM_BUTTONS 4

// Create the pins and link them to the expanderAdapter
// Change the pin numbers to suit your set up.
// These are just the ones I have picked for testing
EventButton buttons[NUM_BUTTONS] = {
    new ExpanderPinAdapter(1, expanderAdapter), //Change the pin number to suit your board
    new ExpanderPinAdapter(3, expanderAdapter), //Change the pin number to suit your board
    new ExpanderPinAdapter(9, expanderAdapter), //Change the pin number to suit your board
    new ExpanderPinAdapter(15, expanderAdapter) //Change the pin number to suit your board
};

/**
 * Utility function to print the button events to Serial.
 * You don't need this - it's just for the example.
 * See other examples & docs for other event types
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
 * Here we have a single callback function shared by all
 * buttons and identify them by getInputId().
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
    Serial.begin(9600);
    delay(1000);

    expanderAdapter.begin();
    // To use an alternative I2C address:
    // expanderAdapter.begin(<your addr>);
    
    // connect buttons to events
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].begin();
        buttons[i].setInputId(i);
        buttons[i].setCallback(onButtonEvent);
    }

    Serial.println("Starting AdafruitGPIOExpanderAdapter example sketch");
}

void loop() {
    // You must call update() to refresh the expander pin states
    expanderAdapter.update();

    // You must call update() for every defined EventButton.
    // This will update the state of the button and 
    // fire the appropriate events.
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].update();
    }
}