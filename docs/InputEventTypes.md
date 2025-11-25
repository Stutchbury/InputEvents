## InputEventType

The `InputEventType` is an [`enum class`](https://en.cppreference.com/w/cpp/language/enum) using a `uint8_t` to identify the type of event. Some are common to all input, others are specific to a type of input:


- **Common to all Event Input classes**
  - `ENABLED` - fired when the input is enabled.
  - `DISABLED` - fired when the input is disabled.
  - `IDLE` - fired after no other event (except `ENABLED` & `DISABLED`) has been fired for a specified time. Each input can define its own idle timeout, default is 10 seconds.
 
- **`EventButton`**, **`EventEncoderButton`** and **`EventTouchScreen`** (experimental) classes
  - `PRESSED` - fired after a button is pressed
  - `RELEASED` - fired after a button is released but if an [`EventEncoderButton`](docs/EventEncoderButton.md) is pressed and turned, this is translated to a `CHANGED_RELEASED` event.
  - `CLICKED` - fired after `RELEASED` if not `LONG_CLICKED` and button is pressed and released once.
  - `DOUBLE_CLICKED` - fired after `RELEASED` if not `LONG_CLICKED` and button is pressed and released twice.
  - `MULTI_CLICKED` - fired after `RELEASED` if not `LONG_CLICKED` and button is pressed and released more than twice (no limit!). The method clickCount() returns the number of clicks.
  - `LONG_CLICKED` - fired *after* a long press.
  - `LONG_PRESS` - fired *during* a long press (hence change of tense). Will repeat by default but this can be turned off.
  Note: If an [`EventEncoderButton`](docs/EventEncoderButton.md) is turned *while* pressed or [`EventTouchScreen`](https://github.com/Stutchbury/EventTouchScreen) is dragged, `RELEASED`, `CLICKED`, `LONG_CLICKED` and `LONG_PRESS` are not fired.
- **`EventEncoder`** and **`EventAnalog`** classes
  - `CHANGED` fired if an encoder or analog is moved.
- **`EventEncoderButton`** class
  - `CHANGED_PRESSED` - fired when the encoder is turned *while* pressed.
  - `CHANGED_RELEASED` - fired when button is released after turning *while* pressed.
- **`EventJoystick`** class
  - `CHANGED_X` - fired when the X axis of a joystick is moved.
  - `CHANGED_Y` - fired when the Y axis of a joystick is moved.
- **`EventSwitch`** class
  - `ON` - fired when the switch is turned on.
  - `OFF` - fired when the switch is turned on.
- **`EventTouchScreen`** class ([in the InputEventsTouchUI library](https://github.com/Stutchbury/InputEventsTouchUI))
  - `DRAGGED` - fired *while* touched and dragging.
  - `DRAGGED_RELEASED` - fired *after* drag is released (ie not touched).
