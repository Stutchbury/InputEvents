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

//Include the required Expander Adapter:
// #include <GpioExpanderAdapter/AdafruitMCP23017ExpanderAdapter.h>
// #include <GpioExpanderAdapter/AdafruitPCF8574ExpanderAdapter.h>
#include <GpioExpanderAdapter/RobTillaartPCF8575ExpanderAdapter.h>

//Include your preferred expander encoder adapter
#include <EncoderAdapter/ExpanderEncoderAdapter.h> // For faster boards
// #include <EncoderAdapter/SlowExpanderEncoderAdapter.h> // For slower boards

// Include the pin adapter for the button
#include <PinAdapter/ExpanderPinAdapter.h>

// And finally, include the event encoder button itself.
#include <EventEncoderButton.h>

//Create the required expander adapter:
RobTillaartPCF8575ExpanderAdapter expanderAdapter;

// To use an alternative I2C address, pass to the constructor (default is 0x20)
// RobTillaartPCF8575ExpanderAdapter expanderAdapter(0x21);

// Or create and pass a separate instance if you want to retain access to the base library methods
// PCF8575 pcf;
// RobTillaartPCF8575ExpanderAdapter expanderAdapter(pcf);


// To use the GPIO expander for both encoder and the encoder's button, we must create adapters for each:
ExpanderEncoderAdapter encoderAdapter(4, 5, expanderAdapter);
// SlowExpanderEncoderAdapter encoderAdapter(4, 5, expanderAdapter);
ExpanderPinAdapter pinAdapter(0, expanderAdapter);

EventEncoderButton eventEncoderButton(&encoderAdapter, &pinAdapter);
//You could also create in place...
//EventEncoderButton eventEncoderButton(&encoderAdapter, new ExpanderPinAdapter(2, &expanderAdapter));
// Or max & match, using a regular GPIO pin number or other PinAdapter


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
  case InputEventType::CHANGED :
    Serial.print("CHANGED");
    break;
  case InputEventType::CHANGED_PRESSED :
    Serial.print("CHANGED_PRESSED");
    break;
  case InputEventType::CHANGED_RELEASED :
    Serial.print("CHANGED_RELEASED");
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
 * EventEncoderButton& defined as parameters.
 */

void onEncoderButtonEvent(InputEventType et, EventEncoderButton &eeb) {
  Serial.print("onEncoderEvent: ");
  printEvent(et); //Call the utility event to print the event type
  Serial.print("\tposition: \t");
  Serial.print(eeb.position());
  Serial.print("\tpressed position: \t");
  Serial.print(eeb.pressedPosition());
  if ( et == InputEventType::MULTI_CLICKED ) {
    Serial.print(" - ");
    Serial.print(eeb.clickCount());
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
    
    eventEncoderButton.begin();
    
    eventEncoderButton.setCallback(onEncoderButtonEvent);

    Serial.println("Starting GpioExpanderEncoderButton example sketch");
}

void loop() {
    // You must call update() to refresh the expander pin states
    expanderAdapter.update();
    eventEncoderButton.update();

}