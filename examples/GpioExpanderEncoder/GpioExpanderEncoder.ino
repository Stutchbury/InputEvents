/**
 * Example of connecting an encoder using the GpioExpanderAdapter and 
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

//Include the required Expander Adapter:
// #include <GpioExpanderAdapter/AdafruitMCP23017ExpanderAdapter.h>
// #include <GpioExpanderAdapter/AdafruitPCF8574ExpanderAdapter.h>
#include <GpioExpanderAdapter/AdafruitPCF8575ExpanderAdapter.h>

//Include the encoder adapter for a GPIO expander
#include <EncoderAdapter/ExpanderEncoderAdapter.h>
// #include <EncoderAdapter/SlowExpanderEncoderAdapter.h>

//Include the EventEncoder
#include <EventEncoder.h>

//Create the required expander adapter:
// AdafruitMCP23017ExpanderAdapter expanderAdapter;
// AdafruitPCF8574ExpanderAdapter expanderAdapter;
AdafruitPCF8575ExpanderAdapter expanderAdapter;

// Or create and pass a separate instance if you want to retain access to the base library methods
// Adafruit_PCF8575 pcf;
// AdafruitPCF8575ExpanderAdapter expanderAdapter(pcf);


ExpanderEncoderAdapter encoderAdapter(4, 5, expanderAdapter);
// SlowExpanderEncoderAdapter encoderAdapter(4, 5, expanderAdapter);

EventEncoder eventEncoder(&encoderAdapter);


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
  case InputEventType::CHANGED :
    Serial.print("CHANGED");
    break;
  default:
    Serial.print("Unknown event: ");
    Serial.print((uint8_t)et);
    break;
  }
}


/**
 * @brief The callback function for the eventEncoder
 * 
 * @param et The event type
 * @param ee A reference to the eventEncoder
 */
void onEncoderEvent(InputEventType et, EventEncoder &ee) {
  Serial.print("onEncoderEvent: ");
  printEvent(et); //Call the utility event to print the event type
  Serial.print("\tposition: \t");
  Serial.print(ee.position());
  Serial.println();
}

void setup() {
    Serial.begin(9600);
    delay(1000);

    expanderAdapter.begin();
    // To use an alternative I2C address:
    // expanderAdapter.begin(<your addr>);
    
    eventEncoder.begin();

    eventEncoder.setCallback(onEncoderEvent);

    Serial.println("Starting GpioExpanderEncoder example sketch");
}

void loop() {
    // You must call update() to refresh the expander pin states
    expanderAdapter.update();
    // Update the encoder to fire events as required
    eventEncoder.update();
}