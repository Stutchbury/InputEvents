# The InputEvents Library

![Picture of button, encoder, potentiometer, joystick and switch](images/all-inputs.png)


**InputEvents** takes the hassle out of handling buttons, switches, encoders, joysticks, and more — whether you’re just starting out or building advanced projects. 

Instead of writing your own debouncing, timing, or multi-click logic, InputEvents turns all those complex, messy details into simple, reliable [events](docs/InputEventTypes.md) like clicked, long press, double-clicked, or encoder turned. 

You get clean, *maintainable* code, fewer bugs, and ***the freedom to focus on your actual project*** - just four lines of code and a function or class method gives you easy access to all events for an input. It works across Arduino, ESP32, ESP8266, Teensy or any microcontroller that is supported by the Arduino framework.

Although this library is primarily for physical inputs, all it really cares about is the pin(s). So if you have a digital input, use an [`EventSwitch`](docs/EventSwitch.md) or [`EventButton`](docs/EventButton.md) and for an analog input, use an [`EventAnalog`](docs/EventAnalog.md). The [`EventEncoder`](docs/EventEncoder.md) could be connected to any encoder but I have not (yet) attached it to a motor...


**Since v1.6.0** ([Release Notes](https://github.com/Stutchbury/InputEvents/releases/tag/v1.6.0)), InputEvents includes the EncoderAdapter so there are no longer any explicit dependencies, ***however***, if you are using `EventEncoder` or `EventEncoderButton` directly attached to GPIO pins, by default, you will need to install Paul Stoffregen's [Encoder](https://github.com/paulstoffregen/Encoder) library. Other encoder libraries can be used via a simple encoder adapter.

Encoders can now be connected to a GPIO expander but this should only be used if you really have to!

**v1.5.2** ([Release Notes](https://github.com/Stutchbury/InputEvents/releases/tag/v1.5.2)) added adapters for Adafruit's and Rob Tillaart's GPIO Expander libraries (MCP23017, PCF8574 & PCF8575). Thank you to [@HubertJH](https://github.com/hubertjh) for helping with the testing - much appreciated.

**Since v1.5.0** ([Release Notes](https://github.com/Stutchbury/InputEvents/releases/tag/v1.5.0)), buttons and switches can be attached to GPIO Expanders to be used with `EventButton` and `EventSwitch`. The button of the `EventEncoderButton` can also be connected via the GPIO Expander but the encoder itself will continue to rely on the underlying encoder library pin connections. Many thanks to [@leon-gh](https://github.com/leon-gh) for his contributon to this - much appreciated.

Please see [GPIO Expander Adapter docs](docs/README.md#gpio-expander-adapter) for more details.

**Since v1.4.0** InputEvents uses a default (but optional) built in debouncer. If you really want to, this can be substituted with a Debouncer of your choice via a DebounceAdapter. Continued thanks to Thomas Fredericks and his [Bounce2](https://github.com/thomasfredericks/Bounce2) library which inspired the original EventButton library!

Paul Stoffregen's [Encoder](https://github.com/paulstoffregen/Encoder) library is the default for [`EventEncoder`](docs/EventEncoder.md) and [`EventButtonEncoder`](docs/EventEncoderButton.md) classes, so many, many thanks for releasing the code.

Please see [Encoder Adapter docs](docs/README.md#encoder-adapter) on using encoder libraries and [additional notes](docs/README.md#notes-on-using-paul-stoffregens-encoder-library) on using PJRC's Encoder library with InputEvents.


Also a huge thanks to [@kfoltman](https://github.com/kfoltman) for genty guiding me towards 'better' solutions to issues I was having and the 'interesting' approaches I was trying.

I am standing on the shoulders of giants.


**Not heard of the term 'event' programming?** Here's a quick primer: [Event Programming 101](docs/EventProgramming101.md). It is by far the easiest way to get your project working with external controls.

## Other InputEvents Libraries

There are two other libraries in the InputEvents family:

1. [InputEventsTouchUI](https://github.com/Stutchbury/InputEventsTouchUI) - allows a touch screen to fire button events plus DRAG and DRAGGED and also includes a UI abstraction layer for GUI elements like regions, widgets and icons.
2. [SerialMessageEvents](https://github.com/Stutchbury/SerialMessageEvents) - takes character (char) input from any serial source via a serial adapter, creating a message from the stream, pass it to your message consumer class or method and optionally pass decoded key/value pairs to your key/value consumer.

## SUPPORT

Feedback and [bug reports](https://github.com/Stutchbury/InputEvents/issues) are welcome or if you have any questions, drop by for a chat on [Discord](https://discord.gg/GDcEcWPKKm) ![Discord Logo](images/Discord-Symbol-Blurple-36x27.png) 

> If you're in a hurry, please see the [full Doxygen generated API documentation](https://stutchbury.github.io/InputEvents/api/index.html).


## INSTALLATION

### Arduino IDE
Install the `InputEvents` and `Encoder` (if you are using `EventEncoder` or `EventEncoderButton`) libraries via the Arduino IDE Libary Manager. 

### PlatformIO
Add the following to your `platformio.ini` file:

```
lib_deps = 
	paulstoffregen/Encoder@^1.4.4
	stutchbury/InputEvents@^1.5.2
```

Paul Stoffregen's Encoder library is the default for `EventEncoder` and `EventEncoderButton` but is not an explicit dependency so needs to be installed separately.


## BASIC USAGE

```cpp
#include <EventButton.h>
// Create a button 
EventButton myButton(<YOUR_BUTTON_PIN>);
// Create a callback function
void onButtonCallback(InputEventType et, EventButton& eb) {
  Serial.println("A button event was fired!");
}
void setup() {
    myButton.begin();
    // Link the button's callback to function defined above
    myButton.setCallback(onButtonCallback);
}
void loop() {
    // Call the button's update() method
    myButton.update();
}
```

That's it! A function and four lines of code and you have a button connected and ready to process events.

>Note: By default, buttons and switches are wired between the GPIO pin & GND using the MCU's internal `INPUT_PULLUP` resistors. Please see [`EventButton`](docs/EventButton.md)/[`EventSwitch`](docs/EventSwitch.md)/[`PinAdapter`](https://stutchbury.github.io/InputEvents/api/classGpioPinAdapter.html) docs if you need to use pull down resistors.


## Input Classes
There is one class for each type of physical input. All classes have some common methods for handling enable/disable and idle timeout (detailed below).

An overview of the events each input type fires is in the [InputEventType section](#inputeventtype) below with details provided in each the documentation page for each input type.

### [EventButton](docs/EventButton.md)

The [`EventButton`](docs/EventButton.md) class is used with monentary switches or inputs, usually refered to as 'buttons'. 

### [EventEncoder](docs/EventEncoder.md)

The [`EventEncoder`](docs/EventEncoder.md) class is used for plain encoders such as an MPG or motor encoder. If you use this with an encoder button, no button events will be fired!

### [EventAnalog](docs/EventAnalog.md)

The [`EventAnalog`](docs/EventAnalog.md) class is generally used for potentiometers but anything that normally connects to an analog pin will suffice. The analog input value is redefined as a limited number of increments or 'slices' so events are fired at specified thresholds rather than 1024 times over the range of the input.

### [EventSwitch](docs/EventSwitch.md)

The [`EventSwitch`](docs/EventSwitch.md) class is for plain on/off switches or inputs.

### [EventEncoderButton](docs/EventEncoderButton.md)

The [`EventEncoderButton`](docs/EventEncoderButton.md) class contains an [`EventEncoder`](docs/EventEncoder.md) and an [`EventButton`](docs/EventButton.md). Certain button events are changed if the encoder is turned while pressed. See [InputEventType section](#inputeventtype) below for an overview.

### [EventJoystick](docs/EventJoystick.md)

The [`EventJoystick`](docs/EventJoystick.md) class contains two `EventAnalog(s)`, enabling very easy use of joysticks with 'interesting' resistance values across their range. The joystick will automatically adjust the extent of the analog range, adjusting slices accordingly. Both X and Y axis can be accessed and configured directly if required. 


## [InputEventTypes](docs/InputEventTypes.md)

The `InputEventType` is an [`enum class`](https://en.cppreference.com/w/cpp/language/enum) using a `uint8_t` to identify the type of event. Some are common to all inputs, others are specific to a type of input.

Full details about all event types are [here](docs/InputEventTypes.md).

----

Please see the [documentation pages](docs) or the [full Doxygen generated API docs](https://stutchbury.github.io/InputEvents/api/index.html) for more detailed information or chat on [Discord](https://discord.gg/GDcEcWPKKm) if you have any questions.