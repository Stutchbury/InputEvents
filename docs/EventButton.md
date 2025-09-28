# EventButton Class

The [`EventButton`](EventButton.md) class is for momentary inputs. The momentary switch (button) is normally wired between the pin and GND, but this can be changed with the `setPressedState()` method and changing the pullup type for the pin.

`EventButton` is probably the easiest way to implement a button - a single function and four lines of code gives you information on all types of user inputs as detailed in the [Event Types](InputEventTypes.md).

![button](../images/button.jpg)


## Basic Usage

```cpp
#include <EventButton.h>
// Create a callback handler function
void onButtonEvent(InputEventType et, EventButton& eb) {
    Serial.print("Button event fired.");
}
// Create a EventButton input.
EventButton myButton(19);             //<-- Line one.
void setup() {
  Serial.begin(9600);
  myButton.begin();                   //<-- Line two.
  delay(500);
  //Link the event(s) to your function
  myButton.setCallback(onButtonEvent); //<-- Line three.
}
void loop() {
    // Update button
  myButton.update();                   //<-- Line four.
}
```
See [example Button.ino](../examples/Button/Button.ino) for a slightly more detailed sketch.

By default the button for EventButton is wired between the GPIO pin and GND,  using the `INPUT_PULLUP` mode of the pin. It is possible to use a pull down resistor with an EventButton by specifying the pin mode in the [GPIOAdapter constructor](https://stutchbury.github.io/InputEvents/api/classGpioPinAdapter.html).

Since v1.4.0, the `EventButton` can use 'virtual pins' via the `PinAdapter`. You don't need to worry about these unless you're using a GPIO expander, doing testing or something else that doesn't involve regular GPIO pins!.

## API Docs

See EventButton's [Doxygen generated API documentation](https://stutchbury.github.io/InputEvents/api/classEventButton.html) for more information.






