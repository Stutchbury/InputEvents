# EventSwitch Class

The [`EventSwitch`](EventSwitch.md) class is for standard on/off inputs. By default, like the [`EventButton`](EventButton.md) the switch is wired between the pin and GND. When the switch is closed (LOW) its state will be 'ON' and when open (HIGH) it will be 'OFF' although this behaviour can be reversed with the `reverseOnOff()` method.

A quick explainer: The switch pin is configured with the microcontroller's internal [`INPUT_PULLUP` resistor](https://en.wikipedia.org/wiki/Pull-up_resistor). In this configuration, without anything attached, the pin will read as HIGH because it has been 'pulled up' to the voltage of the board (either 3.3V or 5V). When you attach a switch (or button) that is wired from the pin to GND, if the switch is closed (or the button pressed), the weak pullup resistor is overcome by the closed switch 'pulling' the pin to GND (0V) or LOW.

It is possible to use a pull down resistor with an EventButton by specifying the pin mode in the [GPIOAdapter constructor](https://stutchbury.github.io/InputEvents/api/classGpioPinAdapter.html).


![button](../images/switch.png)


## Basic Usage


```cpp
#include <EventSwitch.h>
// Create an EventSwitch input
EventSwitch mySwitch(18);
// Create a callback handler function
void onSwitchEvent(InputEventType et, EventSwitch& es) {
    Serial.print("Switch event fired: ");
    Serial.println(es.isOn() ? "ON" : "OFF");
}
void setup() {
    Serial.begin(9600);
    mySwitch.begin();
    // Link the switch's callback to function defined above
    mySwitch.setCallback(onSwitchEvent);
}
void loop() {
    // Call 'update' for every EventSwitch
    mySwitch.update();
}
```

See [example Switch.ino](../examples/Switch/Switch.ino) for a slightly more detailed sketch.


## API Docs

See EventSwitch's [Doxygen generated API documentation](https://stutchbury.github.io/InputEvents/api/classEventSwitch.html) for more information.




