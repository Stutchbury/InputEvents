/**
 * An example of using the EventEncoderButton with position limits
 * 
 * The encoder is limited to the positions 0-10 but will 'wrap around' from >10 to 0 or <0 to 10.
 * When pressed and turned, the pressed position is hard limited (the default) from -5 to +5.
   * 
 */
//First include your chosen encoder library
#include <Encoder.h>  //PJRC's Encoder library
//Then include the adapter for your chosen encoder library
#include <PjrcEncoderAdapter.h> //Adapter for PJRC's Encoder
//Then include EventEncoderButton
#include <EventEncoderButton.h>

const uint8_t encoderPin1 = 2;  //must be in interrupt pin
const uint8_t encoderPin2 = 3;  //should be in interrupt pin
const uint8_t buttonPin = 7;  //change to suit your wiring

//Create an encoder adapter
PjrcEncoderAdapter encoderAdapter(encoderPin1, encoderPin2); //Adapter for PJRC's Encoder.
//Create the EventEncoderButton, passing a reference to the adapter
EventEncoderButton myEncoderButton(&encoderAdapter, buttonPin);

/**
 * Utility function to print the encoder button events to Serial.
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
    Serial.print((uint8_t)iet);
    break;
  }
}

/**
 * A function to handle the events
 * Can be called anything but requires InputEventType and
 * EventEncoderButton& defined as parameters.
 */
void onEncoderButtonEvent(InputEventType et, EventEncoderButton& eeb) {
  Serial.print("onEncoderButtonEvent: ");
  printEvent(et);
  if ( et == InputEventType::CLICKED ) {
    if ( eeb.clickCount() > 1 ) {
      Serial.print(" - ");
      Serial.print(eeb.clickCount());
      Serial.print(" click(s)"); 
    }
  }
  if ( et == InputEventType::CHANGED ) {
    Serial.print(", increment: ");
    Serial.print(eeb.increment());
    Serial.print(", position: ");
    Serial.print(eeb.position());
  }
  if ( et == InputEventType::CHANGED_PRESSED ) {
    Serial.print(", increment: ");
    Serial.print(eeb.increment());
    Serial.print(", pressed position: ");
    Serial.print(eeb.pressedPosition());
  }
  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myEncoderButton.begin();
  delay(500);
  Serial.println("EventEncoderButton With Limits Example");

  //Link the event(s) to your function
  myEncoderButton.setCallback(onEncoderButtonEvent);

  //Set the minimum position limit to zero
  myEncoderButton.setMinPosition(0);
  //Set the maximum position limit to 10
  myEncoderButton.setMaxPosition(10);
  //Make the position wrap max->min->max
  myEncoderButton.wrapMinMaxPosition(true);

  myEncoderButton.setMinPressedPosition(-5);
  myEncoderButton.setMaxPressedPosition(5);


}

void loop() {
  // You must call update() for every defined EventEncoderButton.
  // This will update the state of the encoder button and 
  // fire the appropriate events.
  myEncoderButton.update();
}
