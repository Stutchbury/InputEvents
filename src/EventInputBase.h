/**
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



class EventInputBase {


    protected:

    uint8_t input_id = 0;
    uint8_t input_value = 0;
    bool _enabled = true;
    bool idleFlagged = true;
    unsigned long lastEventMs = millis();
    unsigned long idleTimeout = 10000;


    public:

    /**
     * Must be called from setup().
     */
    virtual void begin() = 0;

    /**
     * Unset the callback. Must be overriden in derived class & then base method called.
     */
    virtual void unsetCallback();

    /**
     * Returns true if the callback is set.
     */
    bool isCallbackSet() { return callbackIsSet; }

    void update();

    bool isEnabled() { return _enabled; }
    
    void enable(bool e = true);


    /**
     * Stop an event from firing.
     */
    void blockEvent(InputEventType et);

    /**
     * Allow an event that has previously been excluded.
     */
    void allowEvent(InputEventType et);

    /**
     * Stop all events from firing - usually used in conjunction with allowEvent()
     */
    void blockAllEvents();

    /**
     * Clear all blocked events.
     */
    void allowAllEvents() { memset(excludedEvents, 0, sizeof(excludedEvents)); } // Reset the bitmask to 0 

    /**
     * Returns true if the event is not excluded.
     */
    bool isEventAllowed(InputEventType et);

    /**
     * Set the button identifier (not unique, defaults to 0)
     */
    void setInputId(uint8_t id) { input_id = id; }

    /**
     * Set the input value (set by user, not used internally).
     */
    void setInputValue(uint8_t val) { input_value = val; }


    /**
     * Get the button identifier (not unique, defaults to 0)
     */
    uint8_t getInputId() {return input_id; }

    /**
     * Get the input value (set by user, not used internally).
     */
    uint8_t getInputValue() { return input_value; }


    /**
     * Set the idle timeout in ms (default 10000) 
     */
    void setIdleTimeout(unsigned int timeoutMs=10000) { idleTimeout = timeoutMs; }

    /** 
     * Returns the number of ms since any event was fired for this input
     */
    unsigned long msSinceLastEvent() { return millis() - lastEventMs; }

    /**
     * @brief Return true if no activity for  longer than 
     * setIdleTimeout - irrespective of whether the 
     * idle (or changed) callback has been fired
     * 
     * @return true 
     * @return false 
     */
    bool isIdle() { return (millis() - lastEventMs) > idleTimeout; }

    /**
     * Reset the idle timer. The IDLE event will fire setIdleTimeout ms
     * after this is called.
     * This is normally done automatically every time an event is fired.
     */
    void resetIdleTimer();

protected:
    bool callbackIsSet = false; //Required because in C/C++ callback has to be defined in derived classes... :-/

    /**
     * Returns true if an event can be invoked and if so, will also
     * reset the idle timeout timer if events are not
     * ENABLED, DISABLED or IDLE.
     * If you don't want to reset the idle timer, use isEventAllowed()
     * The assumption is you *will* invoke() if this returns true.
     */
    bool isInvokable(InputEventType et);

    virtual void invoke(InputEventType et) = 0;

    virtual void onEnabled();
    virtual void onDisabled();
    virtual void onIdle();




private:
    uint8_t excludedEvents[2] = {0};



#ifndef FUNCTIONAL_SUPPORTED
public:
    void setOwner(void *own) { owner = own; }
    void *getOwner() { return owner; }

protected:
    void *owner = nullptr;
#endif


};

#endif