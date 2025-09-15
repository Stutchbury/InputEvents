/*
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "InputEvents.h"


#ifndef EXCLUDE_EVENT_ENCODER

#ifndef EVENT_ENCODER_BUTTON_H
#define EVENT_ENCODER_BUTTON_H


#include "Arduino.h"
#include "EventEncoder.h"
#include "EventButton.h"

/**
 * @brief The EventEncoderButton class combines the EventEncoder class with the EventButton class and  is used for the common encoder buttons.
 * @details This class provides all the EventEncoder & EventButton events with the addition of <code>CHANGED_PRESSED</code> & <code>CHANGED_RELEASED</code> which is fired when the encoder is turned while pressed. 

    The following InputEventTypes are fired by EventEncoderButton:
    - InputEventType::ENABLED - fired when the input is enabled.
    - InputEventType::DISABLED - fired when the input is disabled.
    - InputEventType::IDLE - fired after no other event (except <code>ENABLED</code> & <code>DISABLED</code>) has been fired for a specified time. Each input can define its own idle timeout. Default is 10 seconds.
    - InputEventType::PRESSED - fired after (as) a button is pressed
    - InputEventType::RELEASED - fired after a button is released.
    - InputEventType::CLICKED - fired after <code>RELEASED</code> if not <code>LONG_CLICKED</code> and button is pressed and released once.
    - InputEventType::DOUBLE_CLICKED - fired after <code>RELEASED</code> if not <code>LONG_CLICKED</code> and button is pressed and released twice.
    - InputEventType::MULTI_CLICKED - fired after <code>RELEASED</code> if not <code>LONG_CLICKED</code> and button is pressed and released more than twice (no limit!). The method clickCount() returns the number of clicks.
    - InputEventType::LONG_PRESS - fired *during* a long press (hence change of tense). Will repeat by default but this can be turned off.
    - InputEventType::LONG_CLICKED - fired *after* a long press.
    - InputEventType::CHANGED - fired on each change of position (if min/max limits are not exceeded and wrapping if false)
    - InputEventType::CHANGED_PRESSED - fired on each change of position *while* pressed (if min/max limits are not exceeded and wrapping if false)
    - InputEventType::CHANGED_RELEASED - fired after a button is released if the encoder was turned while pressed *and* a PRESSED_CHANGED event has fired.

    > Note: When the encoder is not pressed and turned, its button behaves just like a regular EventButton but if the encoder is turned *while* pressed, the following changes to events occur:
    > - on release, the button's <code>RELEASED</code> event is translated to a <code>CHANGED_RELEASED</code> event if a <code>PRESSED_CHANGED</code> event has been fired
    > - the <code>LONG_PRESS</code>, <code>CLICKED</code> and <code>LONG_CLICKED</code> are not fired.

 * 
 */
class EventEncoderButton : public EventInputBase {

    protected:

    #if defined(FUNCTIONAL_SUPPORTED)
        /**
         * @brief If <code>std::function</code> is supported, this creates the callback type.
         */
        typedef std::function<void(InputEventType et, EventEncoderButton &ie)> CallbackFunction;
    #else
        /**
         * @brief Used to create the callback type as pointer if <code>std::function</code> is not supported.
         */
        typedef void (*CallbackFunction)(InputEventType et, EventEncoderButton &);
    #endif

    /**
     * @brief The callback function member.
     */
    CallbackFunction callbackFunction = nullptr;

public:

    ///@{
    /** 
     * @name Constructors
     */
    /**
     * @brief Construct an EventEncoderButton input from an [EncoderAdapter](https://github.com/Stutchbury/EncoderAdapter) and a pin.
     * 
     * @details Please see [EncoderAdapter Notes](https://github.com/Stutchbury/InputEvents/blob/main/docs/README.md#encoder-adapter-notes)
     * 
     * > Note: The EncoderAdapter's begin() method will be called from the EventEncoderButton's begin() method.
     * 
     * @param encoderAdapter Pass a previously created [EncoderAdapter](https://github.com/Stutchbury/EncoderAdapter) by reference.
     * @param buttonPin The pin for the button
     */
    EventEncoderButton(EncoderAdapter *encoderAdapter, byte buttonPin, bool useDefaultDebouncer=true);

    /**
     * @brief Construct a new EventButton with a PinAdapter and optionally use the default debouncer
     * 
     * @param encoderAdapter Pass a previously created [EncoderAdapter](https://github.com/Stutchbury/EncoderAdapter) by reference.
     * @param pinAdapter 
     */
    EventEncoderButton(EncoderAdapter *encoderAdapter, PinAdapter* _pinAdapter, bool useDefaultDebouncer=true);

    /**
     * @brief Construct a new EventButton with a PinAdapter and a DebounceAdapter
     * 
     * @param encoderAdapter Pass a previously created [EncoderAdapter](https://github.com/Stutchbury/EncoderAdapter) by reference.
     * @param pinAdapter 
     * @param debounceAdapter 
     */
    EventEncoderButton(EncoderAdapter *encoderAdapter, PinAdapter* _pinAdapter, DebounceAdapter* debounceAdapter);

    ///@}

    ///@{ 
    /** 
     * @name Common Methods
     * @details These methods are common to all InputEvent classes.
     * 
     * Additional methods for input enable, timeout, event blocking and user ID/value are also inherited from the EventInputBase class.
     */
    /**
     * @brief Initialise the EventEncoderButton
     * 
     * @details *Must* be called from within <code>setup()</code>
     */
    void begin();

    /**
     * @brief Set the Callback function.
     * 
     * @param f A function of type <code>EventEncoderButton::CallbackFunction</code> type.
     */
    void setCallback(CallbackFunction f) {
        callbackFunction = f;
        callbackIsSet = true;
    }

    /**
     * @brief Set the Callback function to a class method.
     * 
     * @details Note: This method is only available if <code>std:function</code> is supported.
     * 
     * 
     * @param instance The instance of a class implementing a CallbackFunction method.
     * @param method The class method of type <code>EventEncoderButton::CallbackFunction</code> type.
     */
    #if defined(FUNCTIONAL_SUPPORTED)
    // Method to set callback with instance and class method
    template <typename T>
    void setCallback(T* instance, void (T::*method)(InputEventType, EventEncoderButton&)) {
        // Wrap the method call in a lambda
        callbackFunction = [instance, method](InputEventType et, EventEncoderButton &ie) {
            (instance->*method)(et, ie); // Call the member function on the instance
        };
        callbackIsSet = true;
    }
    #endif

    /**
     * @brief Unset a previously set callback function or method.
     * 
     * @details Must be called before the set function or method is destoyed.
     */
    void unsetCallback() override;

    /**
     * @brief Update the state from the underlying encoder library and button pin
     * 
     * @details *Must* be called from within <code>loop()</code>
     */
    void update();

    /**
     * @brief Resets the state of the input: silently enables if disabled, clears any blocked events, sets inputId & inputValue back to 0 and prevents IDLE event firing.
     * 
     * @details Does not reset configuration or encoder position.
     * 
     */
    void resetState();

    ///@}

    ///@{
    /**
     * @name Getting the State
     * @details These methods return the current state of the input (as set during the last update() )
     */

    /**
     * @brief Returns a positive (CW) or negative (CCW) integer. 
     * @details The returned value normally 1 or -1 but if your 
     * loop() has lots of processing, your Arduino is slow or you setRateLimit()
     * this will report the actual number of increments made by the encoder since
     * the update().
     */
    int16_t increment() { return currentIncrement; }

    /**
     * @brief The current position of the encoder. Can be reset with resetPosition()
     */
    int32_t position() { return currentPosition; }

    /**
     * @brief The current pressed position of the encoder. Can be reset with resetPressedPosition()
     */
    int32_t pressedPosition() { return currentPressedPosition; }

    /**
     * @brief The number of clicks that have been fired in the MULTI_CLICKED event. 
     * @details This is also set do CLICK and DOUBLE_CLICKED and is reset to zero after any CLICKED event is fired.
     * 
     * @return uint8_t Number of clicks
     */
    uint8_t clickCount();


    /**
     * @brief The number of times the long press has occurred during a button press. 
     * @details This is oncremented even if the setLongPressRepeat is false, so can be read in the LONC_CLICKED event. This is reset to zero after `LONG_CLICKED` is fired.

     * @return uint8_t A count of long press occurences
     */
    uint8_t longPressCount();

    /**
     * @brief Returns true if button is pressed
     */
    bool isPressed();

    /**
     * @brief Duration in milliseconds of the current button state
     * 
     * @return Current state duration in milliseconds
     */
    uint32_t currentDuration();

    /**
     * @brief Duration in milliseconds of the previous button state
     * 
     * @return Previous state duration in milliseconds
     */
    uint32_t previousDuration();

    ///@}

    ///@{
    /**
     * @name Setting limits for position() and pressedPosition().
     * 
     * @details High and low limits can be set for both position() and pressedPosition().
     * 
     * - If either setMinPosition() or setMaxPosition() are non zero, the limits will be applied to position()
     * - If either setMinPressedPosition() or setMaxPressedPosition() are non zero, the limits will be applied to pressedPosition()
     * 
     * There are two behaviours defined by wrapMinMaxPosition() and wrapMinMaxPressedPosition(). The default (false) is to not wrap.
     * 
     */

    /**
     * @brief Set a lower limit for the encoder position.
     * 
     * @details If position() is lower than the passed value, then it will be set to minPosition.
     * 
     * @param minPosition The lowest position required
     */
    void setMinPosition(int32_t minPosition=0);

    /**
     * @brief Set an upper limit for the encoder position.
     * 
     * @details If position() is higher than the passed value, then it will be set to maxPosition.
     * 
     * @param maxPosition The highest position required
     */
    void setMaxPosition(int32_t maxPosition=0);

    /**
     * @brief Wrap the max -> min -> max ... (default is to not wrap) 
     * 
     * @details If pressedPosition() is lower than the passed value, then it will be set to minPressedPosition.
     * 
     * @param wrap Pass true to wrap and false for default behaviour.
     * 
     * 
     */
    void wrapMinMaxPosition(bool wrap) { wrapMinMaxPos = wrap; }

    /**
     * @brief Set a lower limit for the encoder pressed position.
     * 
     * @details If pressedPosition() is higher than the passed value, then it will be set to maxPressedPosition.
     * 
     * @param minPressedPosition The lowest pressed position required
     */
    void setMinPressedPosition(int32_t minPressedPosition=0);
    /**
     * @brief Set an upper limit for the encoder pressed position.
     * 
     * @details If either setMinPressedPosition() or setMaxPressedPosition() are non zero, the limits will be applied. 
     * 
     * If wrapMinMaxPressedPosition() is set to true, the pressedPosition() will be wrapped from max to min or min to max.
     * 
     * @param maxPressedPosition The highest pressed position required
     */
    void setMaxPressedPosition(int32_t maxPressedPosition=0);

    /**
     * @brief Wrap the max -> min -> max ... (default is to not wrap) 
     * 
     * @details If wrapMinMaxPressedPosition() is set to false (the default), the pressedPosition() will be stopped at the min or max limit.
     * 
     * If wrapMinMaxPressedPosition() is set to true, the pressedPosition() will be wrapped from max -> min -> max ...
     * 
     * @param wrap Pass true to wrap and false for default behaviour.
     */
    void wrapMinMaxPressedPosition(bool wrap) { wrapMinMaxPressedPos = wrap; }
    ///@}

    ///@{
    /**
     * @name Other Configuration Settings
     */

     /**
     * @brief Encoder callbacks are normally fired on every loop() but for MPG
     * style encoders this can fire a huge number of events (that may 
     * swamp a serial connection).
     * @details The encoder interupts will sitll be called but this will limit 
     * the call back firing to every set ms - read the 
     * EncoderButton.increment() for lossless counting of encoder.
     * Set to zero (default) for no rate limit.
     * 
     * @param ms Number of milliseconds between each update()
     */
    void setRateLimit(long ms);

    /**
     * @brief Quadrature encoders usually have four states for each 'click' of the 
     * rotary control, although some have just 2. 
     * @details Normally we only want to fire an event once per click so the default
     * positionDivider is 4.
     * You can set this to any positive integer eg 8 would increment the
     * position every 2 clicks. 
     * Affects pressed+turning too.
     */
    void setPositionDivider(uint8_t divider=4);

    /**
     * @brief Get the currently set position divider value
     * 
     * @return uint8_t 
     */
    uint8_t getPositionDivider();


    /**
     * @brief Reset the counted position of the EventEncoderButton. 
     * @details Note: This does not reset the position on the  underlying encoder libraries may only allow a 'reset' to 0, not the setting of a specific value.
     * 
     * It also does not check if the passed position is within any min/max limits you may have set.
     */
    void resetPosition(int32_t pos = 0) { 
        currentPosition = pos; 
        previousPosition = currentPosition;
    }

    /**
     * @brief Reset the counted pressed position of the EventEncoderButton. 
     * @details Note: This only resets the EventEncoderButton pressedPosition, it does not change the underlying encoder library position.
     * 
     * It also does not check if the passed position is within any min/max limits you may have set.
     */
    void resetPressedPosition(int32_t pos) { 
        currentPressedPosition = pos; 
        previousPressedPosition = currentPressedPosition;
    }

    /**
     * @brief Choose whether to repeat the long press callback. (true by default)
     * 
     * @param repeat Pass true to repeat, false to not repeat.
     */
    void enableLongPressRepeat(bool repeat=true);

    /**
     * @brief Set the number of milliseconds that define the *first* long click duration.
     * @details The LONG_PRESS event will fire after this duration, then repeat at the interval setLongPressInterval(ms) (default 500ms) if enableLongPressRepeat() is true. After the button is released, a LONG_CLICK event will fire.
     * 
     * @param longDurationMs Default 750ms
     */
    void setLongClickDuration(uint16_t longDurationMs=750);

    /**
     * @brief Set the number of milliseconds that define the *subbsequent* long click intervals.
     * @details If enableLongPressRepeat() is true a LONG_PRESS event will fire every intervalMs after the first LONG_PRESS has fired.
    * 
     * @param intervalMs The interval in milliseconds (default is 500ms).
     */
    void setLongPressInterval(uint16_t intervalMs=500);

    /**
     * @brief Set the multi click interval.
     * 
     * @param intervalMs The interval in milliseconds between double, triple or multi clicks
     */
    void setMultiClickInterval(uint16_t intervalMs=250);

    /**
     * @brief Set the debouncer.
     * **Note:** When planning to use `setDebouncer()` you must ensure `useDefaultDebouncer` is set to `false` in the button or switch constructor. *Previously set debouncers are not deleted*.     * 
     * @param debounceAdapter 
     */
    void setDebouncer(DebounceAdapter* debounceAdapter);

    /**
     * @brief Set the DebounceAdapter debounce interval. Default is 10ms
     * 
     * @return true If the debounce interval has been updated
     * @return false If the debouncer interval has not been updated (ie no debouncer set)
     */
    bool setDebounceInterval(uint16_t intervalMs=10);

    /**
     * @brief Set the pin state that represents 'pressed' for the button. By default this is `LOW` (ie pulled down for pressed).
     * 
     * @details If you set this value to `HIGH`, you must also pass `INPUT_PULLDOWN` to the GpioPinAdapter constructor. 
     * 
     * If your board does not support `INPUT_PULLDOWN`, pass `INPUT` and use an external resistor.
     * 
     * @param state Either LOW (default) or HIGH
     */
    void setPressedState(bool state = LOW);
    ///@}

protected:
    void invoke(InputEventType et) override;
    void onEnabled() override;
    void onDisabled() override;
    void onIdle() override {/* Do nothing. Fire idle callback from either encoder or button but only if both are idle.*/ }

    EventEncoder encoder; ///< the EventEncoder instance
    EventButton button; ///< the EventButton onstance
    /**
     * Convert the encoder & button callbacks to a EventEncoderButton callbacks
     */
    void onInputCallback(InputEventType et, EventInputBase & ie);

private:
    int16_t currentIncrement = 0;

    int32_t currentPosition  = 0;
    int32_t previousPosition  = 0;

    int32_t currentPressedPosition  = 0;
    int32_t previousPressedPosition  = 0;

    bool encodingPressed = false;
    uint16_t encodingPressedCount = 0;     

    int32_t minPos=0;
    int32_t maxPos=0;
    bool wrapMinMaxPos = false;

    int32_t minPressedPos=0;
    int32_t maxPressedPos=0;
    bool wrapMinMaxPressedPos = false;

    void setCallbacks();
    bool onEncoderChanged();

/// \cond DO_NOT_DOCUMENT
#ifndef FUNCTIONAL_SUPPORTED
private:
    static void encoderCallback(InputEventType et, EventEncoder &enc) {
        EventEncoderButton *instance = static_cast<EventEncoderButton *>(enc.getOwner());
        if (instance) instance->onInputCallback(et, enc);
    }

    static void buttonCallback(InputEventType et, EventButton &btn) {
        EventEncoderButton *instance = static_cast<EventEncoderButton *>(btn.getOwner());
        if (instance) instance->onInputCallback(et, btn);
    }
#endif
/// \endcond

};


#endif
#endif

