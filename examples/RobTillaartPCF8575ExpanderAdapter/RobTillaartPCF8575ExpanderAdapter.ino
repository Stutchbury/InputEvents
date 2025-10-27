/**
 * Example of connecting four buttons using the ExpanderPinAdapter and 
 * one of the RobTillaartPCF8575ExpanderAdapter for his PCF8575 library
 *
 * You will need to install Rob's PCF8575 GPOI Expander library.
 * 
 * Supports multiple I2C addresses, set via the constructor.
 *
 */

#include "Arduino.h"

#include <EventButton.h>
// We will use the ExpanderPinAdapter
#include <PinAdapter/ExpanderPinAdapter.h>

//Include the required Expander Adapter:
#include <GpioExpanderAdapter/RobTillaartPCF8575ExpanderAdapter.h>

//Create the required expander adapter:
RobTillaartPCF8575ExpanderAdapter expanderAdapter;

// To use an alternative I2C address, pass to the constructor (default is 0x20)
// RobTillaartPCF8575ExpanderAdapter expanderAdapter(0x21);

// Or create and pass a separate instance if you want to retain access to the base library methods
// PCF8575 pcf;
// RobTillaartPCF8575ExpanderAdapter expanderAdapter(pcf);


#define NUM_BUTTONS 4

// Create the pins and link them to the expanderAdapter
// Change the pin numbers to suit your set up.
// These are just the ones I have picked for testing
EventButton buttons[NUM_BUTTONS] = {
    new ExpanderPinAdapter(1, &expanderAdapter), //Change the pin number to suit your board
    new ExpanderPinAdapter(3, &expanderAdapter), //Change the pin number to suit your board
    new ExpanderPinAdapter(9, &expanderAdapter), //Change the pin number to suit your board
    new ExpanderPinAdapter(15, &expanderAdapter) //Change the pin number to suit your board
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
    
    // Rob Tillaart's PCF8575 library does not call Wire.begin, so if it has not been
    // called previously, we must call it here.
    Wire.begin();

    delay(1000);

    expanderAdapter.begin();
    
    // connect buttons to events
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].begin();
        buttons[i].setInputId(i);
        buttons[i].setCallback(onButtonEvent);
    }

    Serial.println("Starting RobTillaartPCF8575ExpanderAdapter example sketch");
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