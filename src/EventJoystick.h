/*
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */


#ifndef EVENT_JOYSTICK_H
#define EVENT_JOYSTICK_H

#include "Arduino.h"
#include "EventAnalog.h"


/**
 * @brief The EventJoystick class contains two EventAnalog inputs configured as a joystick.

The following InputEventTypes are fired by EventJoystick:
  - InputEventType::ENABLED - fired when the input is enabled.
  - InputEventType::DISABLED - fired when the input is disabled.
  - InputEventType::IDLE - fired after no other event (except <code>ENABLED</code> & <code>DISABLED</code>) has been fired for a specified time. Each input can define its own idle timeout. Default is 10 seconds.
  - InputEventType::CHANGED_X - fired on each change of increment on the X axis.
  - InputEventType::CHANGED_Y - fired on each change of increment on the Y axis.

*/
class EventJoystick : public EventInputBase {

protected:

    #if defined(FUNCTIONAL_SUPPORTED)
        /**
         * @brief If <code>std::function</code> is supported, this creates the callback type.
         */
        typedef std::function<void(InputEventType et, EventJoystick &ie)> CallbackFunction;
    #else
        /**
         * @brief Used to create the callback type as pointer if <code>std::function</code> is not supported.
         */
        typedef void (*CallbackFunction)(InputEventType et, EventJoystick &);
    #endif

    /**
     * @brief The callback function member.
     */
    CallbackFunction callbackFunction = nullptr;

    /**
     * @brief Override of the <code>EventInputBase::invoke()</code> virtual method.
     * 
     * @param et Enum of type <code>InputEventType</code>
     */
    void invoke(InputEventType et) override;

    void onEnabled() override;
    void onDisabled() override;
    void onIdle() override { /* Do nothing. Fire idle callback from either x or y but only if both are idle from onInputCallback() */ }

public:

    ///@{ 
    /** 
     * @name Constructor 
     */
    /**
     * @brief Construct anEventJoystick input
     * @details The analogPinX and analogPinY parameters *must* be an analog pins. For ESP32 avoid using pins attached to ADC2 (GPIO 0, 2, 4, 12-15, 25-27) as these are shared by the WiFi module.
     * 
     * For most boards the default adcBits in this constructor will work fine but if your board has an ADC (analog to digital converter) resolution that is higher than the standard Arduino 10 bits, pass the resolution (in bits) of your board.
     * 
     * @param analogX *must* be an analog pin
     * @param analogY *must* be an analog pin
     * @param adcBits ADC width in bits. Default is 10.
     */
    EventJoystick(byte analogX, byte analogY, uint8_t adcBits=10);
    ///@}




    ///@{ 
    /** 
     * @name Common Methods
     * @details These methods are common to all InputEvent classes.
     * 
     * Additional methods for input enable, timeout, event blocking and user ID/value are also inherited from the EventInputBase class.
     */

    /**
     * @brief Initialise the EventButton
     * 
     * @details *Must* be called from within <code>setup()</code>
     */
    void begin();

    /**
     * @brief Set the Callback function.
     * 
     * @param f A function of type <code>EventJoystick::CallbackFunction</code> type.
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
     * @param method The class method of type <code>EventJoystick::CallbackFunction</code> type.
     */
    #if defined(FUNCTIONAL_SUPPORTED)
    // Method to set callback with instance and class method
    template <typename T>
    void setCallback(T* instance, void (T::*method)(InputEventType, EventJoystick&)) {
        // Wrap the method call in a lambda
        callbackFunction = [instance, method](InputEventType et, EventJoystick &ie) {
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
     * @brief Update the state from both X and Y pin inputs.
     * 
     * @details *Must* be called from within <code>loop()</code>
     */
    void update();

    /**
     * @brief Resets the state of the input: silently enables if disabled, clears any blocked events, sets inputId & inputValue back to 0 and prevents IDLE event firing.
     * 
     * @details Does not reset configuration.
     * 
     */
    void resetState();

    ///@}

    ///@{ 
    /** 
     * @name Axis Members
     * @details The underlying X and Y EventAnalog inputs are exposed to allow access to their state.
     */

    /**
     * @brief The EventAnalog for the X axis
     */
    EventAnalog x;

    /**
     * @brief The EventAnalog for the Y axis
     */
    EventAnalog y;
    ///@}


    ///@{
    /**
     * @name Getting the State
     * @details These methods return the current state of the input (as set during the last update()
     * 
     * The state of the EventAnalog for each axis can be obtained via the public members x and y.
     */

    /**
     * @brief Returns true of the position of either EventAnalog
     * axis has changed since previous update()
     * 
     * @return true 
     * @return false 
     */
    bool hasChanged();

    /**
     * @brief Returns true if enabled (or if either of the EventAnalg axis are enabled)
     */
    bool isEnabled();

    /**
     * @brief Return true if no activity on both EventAnalog
     * axis for  longer than setIdleTimeout() - irrespective of 
     * whether the idle (or changed) callback has been fired
     * 
     * @return true 
     * @return false 
     */
    bool isIdle();
    ///@}


    ///@{
    /**
     * @name Setting Required Increments
     * @details These methods are used to set the the number of increments to be 'sliced' for both axis
     */


    /**
     * @brief Split the analog range into this number of slices. The default is 10. 
     * @details  Note: This library is intended to reduce the usual 0-1024 range of analogRead() to a much smaller, more manageable number of 'slices'. 
     * Much higher numbers may produce variable results as a slice width is the ADC resolution (1024) divided by the number of increments. 
     * 
     * An InputEventType::CHANGED_X or CHANGED_Y event will be fired each time the increment changes. 
     * 
     * @param numIncr The number of desired increments. Should not be more than 20% of the ADC max value.
     */
    void setNumIncrements(uint8_t numIncr=10);

    /**
     * @brief Normally increments are set with setNumIncrements() but you can also set the negative and positive sides of 'centre normal' individually.
     * 
     * @param numIncr The number of increments required on the negative side.
     */
    void setNumNegativeIncrements(uint8_t numIncr=10);

    /**
     * @brief Normally increments are set with setNumIncrements() but you can also set the negative and positive sides of 'centre normal' individually.
     * 
     * @param numIncr The number of increments required on the positive side.
     */
    void setNumPositiveIncrements(uint8_t numIncr=10);
    ///@}

    ///@{
    /**
     * @name Setting Analog start value and centre & outer boundaries
     * @details These methods are used to set start value and centre & outer boundaries of the input.
     */

    /**
     * @brief Will set the X and Y start values as the current position. 
     * @details This calls the the X & Y EventAnalog::setStartPosition() to set the joystick in its 'rest' position.
     * 
     * Called from begin().
     */
    void setStartValues();

    /**
     * @brief Used to create a central 'dead zone' on the joystick.
     * @details It is very difficult to press the joystick button without moving the joystick so with this we can
     * create a central deadzone.
     * 
     * @param width - the analog value 
     */
    void setCentreBoundary(uint16_t width=200);
    /**
     * @brief Used to allow maximum increments to be reached 'on the diagonal'
     * @details Create an outer 'deadzone' where joysticks are notoriously inconsistent or cannot be reach on a diagonal.
     * 
     * @param width - the analog value 
     */

    void setOuterBoundary(uint16_t width=100);
    ///@}


   ///@{
    /**
     * @name Other Configuration Settings
     */
    /**
     * @brief Limit the rate at which events are fired.
     * @details When moving a joystich fast it can generate a lot of events, so you can rate limit.
     * 
     * @param ms Number of milliseconds between events
     */
     void setRateLimit(uint16_t ms);

    /**
     * @brief If enableAutoCalibrate is set to true (the default), will
     * do auto calibration, setting the minValue and maxValue for both X and Y axis
     * 
     * @details This will be done even when input is disabled (ie no callbacks fired)
     */
    void enableAutoCalibrate(bool allow=true);
    ///@}


protected:
    /**
     * Convert the EventAnalog callbacks to EventJoystic callbacks
     */
    void onInputCallback(InputEventType et, EventInputBase & ie);
    /**
     * Convert the EventAnalog X CHANGED callbacks to CHANGED_X
     */
    void onInputXCallback(InputEventType et, EventInputBase & ie);
    /**
     * Convert the EventAnalog Y CHANGED callbacks to CHANGED_Y
     */
    void onInputYCallback(InputEventType et, EventInputBase & ie);

private:


#ifndef FUNCTIONAL_SUPPORTED
private:
    static void analogXCallback(InputEventType et, EventAnalog &enc) {
        EventJoystick *instance = static_cast<EventJoystick *>(enc.getOwner());
        if (instance) instance->onInputXCallback(et, enc);
    }

    static void analogYCallback(InputEventType et, EventAnalog &enc) {
        EventJoystick *instance = static_cast<EventJoystick *>(enc.getOwner());
        if (instance) instance->onInputYCallback(et, enc);
    }
#endif


};


#endif

