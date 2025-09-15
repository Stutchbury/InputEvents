/*
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef EVENT_INPUT_BASE_H
#define EVENT_INPUT_BASE_H

#include <Arduino.h>

#include "InputEvents.h"

#ifdef FUNCTIONAL_SUPPORTED
    #include <functional>
#endif


/**
 * @brief The common base for InputEvents input classes.
 * @details Specifies a number of virtual methods and implements common methods for enable, timeout, event blocking and user ID/values.
 * 
 */
class EventInputBase {


    protected:

    uint8_t input_id = 0; ///< Input ID, not used internally
    uint8_t input_value = 0; ///< Input value, not used internally
    bool _enabled = true; ///< Input enabled flag
    bool idleFired = true; ///< True if input IDLE event has fired
    unsigned long lastEventMs = millis(); ///< number of milliseconds since the last event
    unsigned long idleTimeout = 10000; ///< The idle timeout in milliseconds


    public:

    /**@{
     * @name Common Methods
     */

    /**
     * @brief Must be called from setup().
     * 
     */
    virtual void begin() = 0;

    /**
     * @brief Unset the callback. Must be overriden in derived class & then base method called.
     */
    virtual void unsetCallback();

    /**
     * @brief Returns true if the callback is set.
     */
    bool isCallbackSet() { return callbackIsSet; }

    /**
     * @brief Update the state of the input.
     * 
     * @details *Must* be called from within <code>loop()</code>
     */
    void update();

    /**
     * @brief Returns true if input is enabled.
     * 
     * @return true Input is enabled
     * @return false  Input is not enabled
     */

    bool isEnabled() { return _enabled; }
    
    /**
     * @brief Enable or disable an input
     * 
     * @param e Pass false to disable (default is true)
     */
    void enable(bool e = true);

    /**
     * @brief Resets the 'user' state of the input: silently enables if disabled, clears any blocked events, sets inputId & inputValue back to 0 and prevents IDLE event firing.
     * 
     * @details Does not reset configuration or encoder position.
     * 
     */
    void resetState();

    ///@}

    ///@{
    /**
     * @name Idle methods
     * @details These methods allow you to set and get the state of the idle timer
     */
    /**
     * @brief Set the idle timeout in milliseconds (default 10000, 10 seconds) 
     * 
     * @param timeoutMs The number of milliseconds after which the IDLE event will fire
     */
    void setIdleTimeout(unsigned int timeoutMs=10000) { idleTimeout = timeoutMs; }

    /** 
     * @brief Returns the number of ms since any event was fired for this input
     */
    unsigned long msSinceLastEvent() { return millis() - lastEventMs; }

    /**
     * @brief Return true if no activity for  longer than setIdleTimeout - irrespective of whether the 
     * idle (or changed) callback has been fired
     * 
     * @return true Idle timer has ended
     * @return false  Not idle
     */
    bool isIdle() { return (millis() - lastEventMs) > idleTimeout; }

    /**
     * @brief Reset the idle timer. The IDLE event will fire setIdleTimeout ms
     * after this is called.
     * @details This is normally done automatically every time an event is fired but can be called to further delay and idle event
     */
    void resetIdleTimer();
    ///@}

    ///@{
    /**
     * @name Blocking and Allowing events
     * @details All events are fired by default but you can block or allow certain events.
     */
    /**
     * @brief Stop an event from firing.
     * 
     * @param et InputEventType The event you want to block
     */
    void blockEvent(InputEventType et);

    /**
     * @brief Allow a a previously blocked event tto fire.
     * 
     * @param et InputEventType The event you want to allow
     */
    void allowEvent(InputEventType et);

    /**
     * @brief Stop all events from firing - usually used in conjunction with allowEvent()
     */
    void blockAllEvents();

    /**
     * @brief Clear all blocked events.
     */
    void allowAllEvents() { memset(excludedEvents, 0, sizeof(excludedEvents)); } // Reset the bitmask to 0 

    /**
     * @brief Returns true if the event is not blocked.
     */
    bool isEventAllowed(InputEventType et);
    ///@}

    ///@{
    /**
     * @name Input ID and Value
     * @details These methods *are not* used internally and are for developer use to identify a particular input, eg so multiple buttons can share a callback function
     * or to set a vlaue for a multi-function button.
     * Expected to be used with enums.
     */
    /**
     * @brief Set the input ID (for use by user, not used internally). Not unique, default is 0.
     */
    void setInputId(uint8_t id) { input_id = id; }

    /**
     * @brief Get the input ID (for use by user, not used internally). Not unique, default is 0.
     */
    uint8_t getInputId() {return input_id; }

    /**
     * @brief Set the input value (for use by user, not used internally). Not unique, default is 0.
     */
    void setInputValue(uint8_t val) { input_value = val; }

    /**
     * @brief Get the input value (for use by user, not used internally). Not unique, default is 0.
     */
    uint8_t getInputValue() { return input_value; }
    ///@}


protected:
    bool callbackIsSet = false; ///< Required because in C/C++ callback has to be defined in derived classes... :-/

    /**
     * Returns true if an event can be invoked and if so, will also
     * reset the idle timeout timer if events are not
     * ENABLED, DISABLED or IDLE.
     * If you don't want to reset the idle timer, use isEventAllowed()
     * The assumption is you *will* invoke() if this returns true.
     */
    bool isInvokable(InputEventType et);

    /**
     * @brief To be overriden by derived classes.
     * 
     * @param et Enum of type <code>InputEventType</code>
     */
    virtual void invoke(InputEventType et) = 0;

    /**
     * @brief Can be ovrriden by derived classes but base method must be called.
     */
    virtual void onEnabled();
    /**
     * @brief Can be ovrriden by derived classes but base method must be called.
     */
    virtual void onDisabled();
    /**
     * @brief Can be ovrriden by derived classes but base method must be called.
     */
    virtual void onIdle();




private:
    //uint8_t excludedEvents[4] = {0};
    uint8_t excludedEvents[(static_cast<uint8_t>(InputEventType::COUNT) + 7) / 8] = {0};

/// \cond DO_NOT_DOCUMENT
#ifndef FUNCTIONAL_SUPPORTED
public:
    void setOwner(void *own) { owner = own; }
    void *getOwner() { return owner; }

protected:
    void *owner = nullptr;
#endif
/// \endcond


};

#endif