/**
 * A basic example of using the EventEncoder
 * Note: This example is for an encoder without a button so
 * button events and 'pressed changed' etc will not be fired. .
 * See the EventEncoderButton example for that.
 */

//First include your chosen encoder library
#include <Encoder.h> //PJRC's Encoder library
//Then include the adapter for your chosen encoder library
#include <EncoderAdapter/PjrcEncoderAdapter.h> //Adapter for PJRC's Encoder
//Then include EventEncoder
#include <EventEncoder.h>

const uint8_t encoderPin1 = 2;  //must be in interrupt pin
const uint8_t encoderPin2 = 3;  //should be in interrupt pin

//Create an encoder adapter
PjrcEncoderAdapter encoderAdapter(encoderPin1, encoderPin2); //Adapter for PJRC's Encoder.
//Create the EventEncoder, passing a reference to the adapter
EventEncoder myEncoder(&encoderAdapter); //Create an EventEncoder

/**
 * Utility function to print the encoder events to Serial.
 * See other examples for other event types
 */
void printEvent(InputEventType iet) {
  switch (iet) {
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
    Serial.print((uint8_t)iet);
    break;
  }
}


/**
 * A function to handle the events
 * Can be called anything but requires InputEventType and
 * EventEncoder& defined as parameters.
 */
void onEncoderEvent(InputEventType et, EventEncoder& ee) {
  Serial.print("onEncoderEvent: ");
  printEvent(et);
  if ( et == InputEventType::CHANGED ) {
    Serial.print(", increment: ");
    Serial.print(ee.increment());
    Serial.print(", position: ");
    Serial.print(ee.position());
  }
  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myEncoder.begin();
  delay(500);
  Serial.println("EventEncoder Basic Example");

  //Link the event(s) to your function
  myEncoder.setCallback(onEncoderEvent);
}

void loop() {
  // You must call update() for every defined EventEncoder.
  // This will update the state of the encoder and 
  // fire the appropriate events.
  myEncoder.update();
}
