# InputEvents Library Documentation


## Classes and Events

This is a list of the primary input classes. Please follow the links for further details.
- [Common Events and Class Methods](Common.md)
- [EventAnalog](EventAnalog.md)
- [EventButton](EventButton.md)
- [EventEncoder](EventEncoder.md)
- [EventEncoderButton](EventEncoderButton.md)
- [EventJoystick](EventJoystick.md)
- [EventSwitch](EventSwitch.md)
- [All InputEventTypes](InputEventTypes.md)

----

## [Event Programming 101](EventProgramming101.md)
If you're new to the concept of event programming, [here is a short primer](EventProgramming101.md). It will make coding your project *so* much easier!

----

## Adapters

Adapters are a very thin layer of abstraction used by InputEvents to provide a common API for third party libraries and boards.

There are currently adapter for encoders, input pins and GPIO expander boards.

### Encoder Adapters

Since v1.6.0, the EncoderAdapter library (introduced in v1.2.1) has been merged into Input Events. You can use almose any adapter library via a very thin adapter class.

Currently an encoder adapter is provided for PJRC's Encoder and for encoders or encoder buttons connected via a GPIO Expander. More will be added or you can write your own.

See the [PJRC one](https://github.com/Stutchbury/InputEvents/blob/main/src/EncoderAdapter/PjrcEncoderAdapter.h) as an example of how simple it is! If you want to share your adapter, I will add it the the EncoderAdapter library. Pull requests are welcome!

These changes mean that rather than passing the encoder pins to the `EventEncoder` or `EventEncoderButton` constructors, we pass an EncoderAdapter that has been previously constructed from the pins.

You must include both the appropriate encoder library and its adapter in your sketches.

eg:

```
#include <Encoder.h> //PJRC's lib
#include <EncoderAdapter/PjrcEncoderAdapter.h>
```
and then:
```
PjrcEncoderAdapter encoderAdapter(2,3); //Should be interrupt pins
EventEncoder myEncoder(&encoderAdapter);
```

----

#### Notes on using Paul Stoffregen's Encoder Library

> Paul Stoffregen's Encoder Library remains the default but you can now use different encoder libraries with `InputEvents`. See Encoder Adapter Notes above.

Please take care to read the pin requirements in the [official documantation page](https://www.pjrc.com/teensy/td_libs_Encoder.html) (TLDR; use interupt capable pins). The github repository is [here](https://github.com/paulstoffregen/Encoder).

This library is very good (and very optimised) for the boards it supports, but it doesn't support all microcontrollers.

> The latest formal release of the Encoder library is 1.4.4 from 11th Dec 2023. There have been a number of updates to the git repo since then without a formal release. If your board does not work with the 1.4.4 relese, it may work if you manually install the github source.

Paul's Encoder library supports far more boards than I have available but I have tested with:

- **Teensy** - 4.0 and 4.1.
- **Arduino UNO** - the base line AVR type board, only 2 interrupt pins!
- **ESP8266** - D1 Mini & Adafruit Feather 8266. Lots of compiler deprecation warnings for Encoder but compiles OK. Only one analog pin, so no joystick.
- **ESP32** - D1 Mini32, Adafruuit & other random ones. No issues since v1.0.2.


----

### GPIO Expander Adapters

Since v1.5.0 (since v1.6.0 for encoders)

Using a GPIO Expander Adapter and the `ExpanderPinAdapter`, you can connect `EventButton`, `EventSwitch`, `EventEncoder` and `EventEncoderButton` to GPIO Expander boards.

Most GPIO Expander Adapters use an underlying library so this will need to be installed but the `#include` is done by the adapter.

These boards usually connect via SPI or I2C. The adapters are `update()`d in your `loop()` before any button or switch `update()` calls otherwise a slow I2C or SPI call would have to be made for every button/switch.

> The `RobTillartPCF8575ExpanderAdapter` will likely require a `Wire.begin()` in your sketch as this is not done in the underlying library.

> Some I2C boards require external pullup resistors on the SDA and SCL pins: https://support.arduino.cc/hc/en-us/articles/11153357842588-I2C-and-pull-up-resistors

## Testing

I'm investigating how to write a unit test suite but mocking input pins (particularly for the encoder) is currently a little beyond my paygrade. Pull requests welcome.

In the meantime, these are my physical test rigs:

A custom 'hat' For Arduino UNO, ESP8266 and ESP32:

![Test Rig](../images/test-rig.jpg)

A slightly more formal PCB test rig for testing multiple boards (Adafruit Feathers of any flavour, Teensy 4.0 and STM32 Black Pill) - also useful for testing GPIO Expanders and a couple of different touch displays (for the [InputEventsTouchUI library](https://github.com/Stutchbury/InputEventsTouchUI)):

![PCB Test Rig](../images/new-test-rig.jpg)

and, of course, a [Manualmatic Pendant](https://github.com/Stutchbury/Manualmatic-Pendant) for the Teensy 4.1:

![Manualmatic Pendant](../images/manualmatic.jpg)
