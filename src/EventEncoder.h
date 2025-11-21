/*
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "InputEvents.h"

#ifndef EVENT_ENCODER_H
#define EVENT_ENCODER_H

#include "Arduino.h"
#include "EventInputBase.h"
#include <EncoderAdapter/IEncoderAdapter.h>

/**
 * @brief The EventEncoder class is for quadrature encoder inputs providing the position & encoder increment, event rate limiting without losing steps (eg for easy acceleration or to reduce events sent over Serial). 

 * @details  It is effectively an event wrapper around a low level encoder library. By default, Paul Stoffregen's [Encoder library](https://www.pjrc.com/teensy/td_libs_Encoder.html) is used but adapters can easily be created for others (and more will be added).

The following InputEventTypes are fired by EventEncoder:
  - InputEventType::ENABLED - fired when the input is enabled.
  - InputEventType::DISABLED - fired when the input is disabled.
  - InputEventType::IDLE - fired after no other event (except <code>ENABLED</code> & <code>DISABLED</code>) has been fired for a specified time. Each input can define its own idle timeout. Default is 10 seconds.
  - InputEventType::CHANGED - fired on each change of position
 */
class EventEncoder : public EventInputBase {

    protected:

    #if defined(FUNCTIONAL_SUPPORTED)
        /**
         * @brief If <code>std::function</code> is supported, this creates the callback type.
         */
        typedef std::function<void(InputEventType et, EventEncoder &ie)> CallbackFunction;
    #else
        /**
         * @brief Used to create the callback type as pointer if <code>std::function</code> is not supported.
         */
        typedef void (*CallbackFunction)(InputEventType et, EventEncoder &);
    #endif

    /**
     * @brief The callback function member.
     */
    CallbackFunction callbackFunction = NULL;

    /**
     * @brief Read and set the increment during update()
     */
    void readIncrement();

public:

    ///@{ 
    /** 
     * @name Constructor and Destructor
     */

    /**
     * @brief Construct an EventEncoder input from an IEncoderAdapter
     * 
     * @details Please see [EncoderAdapter Notes](https://github.com/Stutchbury/InputEvents/blob/main/docs/README.md#encoder-adapter-notes)
     * 
     * > Note: The EncoderAdapter's begin() method will be called from the EventEncoder's begin() method.
     * 
     * @param encoderAdapter Pass a previously created IEncoderAdapter by reference.
     */
    EventEncoder(IEncoderAdapter *encoderAdapter);

    /**
     * @brief Destroy the EventEncoder input
     */
    ~EventEncoder();
    ///@}

    ///@{ 
    /** 
     * @name Common Methods
     * @details These methods are common to all InputEvent classes.
     * 
     * Additional methods for input enable, timeout, event blocking and user ID/value are also inherited from the EventInputBase class.
     */

    /**
     * @brief Initialise the EventEncoder
     * 
     * @details *Must* be called from within <code>setup()</code>
     */
    void begin();

    /**
     * @brief Set the Callback function.
     * 
     * @param f A function of type <code>EventEncoder::CallbackFunction</code> type.
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
     * @param method The class method of type <code>EventButton::CallbackFunction</code> type.
     */
    #if defined(FUNCTIONAL_SUPPORTED)
    // Method to set callback with instance and class method
    template <typename T>
    void setCallback(T* instance, void (T::*method)(InputEventType, EventEncoder&)) {
        // Wrap the method call in a lambda
        callbackFunction = [instance, method](InputEventType et, EventEncoder &ie) {
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
     * @brief Update the state from the underlying encoder library.
     * 
     * @details Must be called from within <code>loop()</code>
     */
    void update();
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
    int16_t increment() { return encoderIncrement; }

    /**
     * @brief The current position of the encoder. Can be reset with resetPosition()
     */
    long position() { return currentPosition; }
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
     */
    void setRateLimit(long ms=0) { rateLimit = ms; }

    /**
     * @brief Quadrature encoders usually have four states for each 'click' of the 
     * rotary control, although some have just 2. 
     * @details Normally we only want to fire an event once per click so the default
     * positionDivider is 4.
     * You can set this to any positive integer eg 8 would increment the
     * position every 2 clicks. 
     * Affects pressed+turning for EventEncoderButton too.
     */
    void setPositionDivider(uint8_t divider=4) {
        if ( divider > 0 ) {
                positionDivider = divider;
            }
        }

    /**
     * @brief Get the currently set position divider value
     * 
     * @return uint8_t 
     */
    uint8_t getPositionDivider() { return positionDivider; }


    /**
     * @brief Reset the counted position of the encoder. 
     * @details Note: Some underlying encoder libraries may only allow a 'reset' to 0, not the setting of a specific value.
     */
    void resetPosition(long pos = 0) { currentPosition = pos; }
    ///@}

protected:

    void invoke(InputEventType et) override;
    void onEnabled() override;

private:

    IEncoderAdapter *encoder;

    uint8_t positionDivider = 4;
    int32_t currentPosition  = 0;
    int32_t oldPosition  = 0;
    unsigned int rateLimit = 0;
    unsigned long rateLimitCounter = 0;   
    int encoderIncrement  = 0;

};

#endif
