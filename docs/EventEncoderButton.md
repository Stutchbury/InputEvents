# EventEncoderButton Class

The [`EventEncoderButton`](EventEncoderButton.md) class combines the [`EventEncoder`](EventEncoder.md) class with the [`EventButton`](EventButton.md) class and  is used for the common encoder buttons.


The class provides all the encoder events with the addition of `CHANGED_PRESSED` which is fired when the encoder is pressed and turned. 

All the [`EventButton`](EventButton.md) events are also fired but if the encoder is turned while pressed the following changes to events occur:

- on release, the button's `RELEASED` event is translated to a `CHANGED_RELEASED` event.
- the `LONG_PRESS` is not fired.
- Neither `CLICKED` or `LONG_CLICKED` are fired.

When the encoder is not pressed and turned, its button behaves just like a regular [`EventButton`](EventButton.md).



![button](../images/encoder-button.png)


## Basic Usage

> Note: Unlike other inputs, `EventEncoderButton` and `EventEncoder` are not directly linked to the pins but through an `EncoderAdapter`. This allows the classes to use different underlying encoder libraries. 
 
You must explicitly `#include` both the chosen encoder library and its adapter and then pass the adapter to the `EventEncoderButton`:

```cpp
//First include your chosen encoder library
#include <Encoder.h> //PJRC's Encoder library
//Then include the adapter for the encoder library
#include <PjrcEncoderAdapter.h> //Adapter for PJRC's Encoder
//Then include EventEncoderButton
#include <EventEncoderButton.h>
//Create an adapter for PJRC's Encoder.
PjrcEncoderAdapter encoderAdapter(2,3); //Should be interrupt pins
// Create an EventEncoderButton, passing a reference to the adapter
EventEncoderButton myEncoderButton(&encoderAdapter, 7);
// Create a callback handler function
void onEncoderButtonEvent(InputEventType et, EventEncoderButton& eeb) {
    Serial.print("Encoder button event fired. Position is: ");
    Serial.print(eeb.position());
    Serial.print(" Pressed position is: ");
    Serial.println(eeb.pressedPosition());
}
void setup() {
    Serial.begin(9600);
    myEncoderButton.begin();
    // Link the encoder button's callback to function defined above
    myEncoderButton.setCallback(onEncoderButtonEvent);
}
void loop() {
    // Call 'update' for every EventEncoder
    myEncoderButton.update();
}
```

See [example EncoderButton.ino](../examples/EncoderButton/EncoderButton.ino) for a slightly more detailed sketch.


## Event Types

In addition to the [common events](Common.md#common-events) (Enabled, Disabled and Idle) and the [`EventEncoder`](EventEncoder.md) & [`EventButton`](EventButton.md) events, the following event types are fired by EventEncoderButton:


----

#### `InputEventType::CHANGED_PRESSED` 
Will be fired on each change of encoder increment is pressed and turned.

----

#### `InputEventType::CHANGED_RELEASED` 
Will be fired in place of the button's `RELEASED` event if the encoder is pressed and turned.


## Constructor

Construct an EventEncoderButton
```cpp
EventEncoder(EncoderAdapter encoderAdapter, buttonPin);
```

## Class Methods

In addition to the [common methods](Common.md#common-methods) the following are available for EventAnalog:

#### `void update()`

Must be called within `loop()`. See [common methods](Common.md#void-update) for details.


### Setup

Setup methods are typically called from within `setup()` but can be updated at runtime.

#### `void resetPressedPosition(long pos = 0)`
Reset the counted position of the encoder. 

Please see [`EventEncoder` setup](EventEncoder.md#setup) and [`EventButton` setup](EventButton.md#setup) documentation for details about the other setup methods.

### State

#### `long pressedPosition()`
The current pressed position of the encoder. Can be reset by `resetPressedPosition()`.


Please see [`EventEncoder` state](EventEncoder.md#setup) and [`EventButton` sstate](EventButton.md#setup) documentation for details about the other state methods.



