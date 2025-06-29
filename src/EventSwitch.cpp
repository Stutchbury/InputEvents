/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "EventSwitch.h"


EventSwitch::EventSwitch(byte pin, bool useDefaultDebouncer /*=true*/)
    : pinAdapter(new GpioPinAdapter(pin))
    { 
        if ( useDefaultDebouncer ) {
            debouncer = new FoltmanDebounceAdapter(pinAdapter);
        }
    }

EventSwitch::EventSwitch(PinAdapter* _pinAdapter, bool useDefaultDebouncer /*=true*/)
    : pinAdapter(_pinAdapter) 
    { 
        if ( useDefaultDebouncer ) {
            debouncer = new FoltmanDebounceAdapter(pinAdapter);
        }
    }

EventSwitch::EventSwitch(PinAdapter* _pinAdapter, DebounceAdapter* debounceAdapter) 
    : pinAdapter(_pinAdapter),
      debouncer(debounceAdapter)
    { 
        debouncer->setPinAdapter(pinAdapter);
    }

    
void EventSwitch::begin() {
    pinAdapter->begin();
    if ( debouncer ) {
        debouncer->begin();
    }
    changedState(); //Use to read/set inital state
    stateChanged = false;
}

void EventSwitch::unsetCallback() {
    callbackFunction = nullptr;
    EventInputBase::unsetCallback();
}

void EventSwitch::update() {
    if (_enabled) {
        if (changedState()) {
            if (turningOn()) {
                //previousState = HIGH;
                invoke(InputEventType::ON);
            } else if (turningOff()) {
                invoke(InputEventType::OFF);
                //previousState = LOW;
            }
            stateChanged = false;
        }
        EventInputBase::update();
    }
}

void EventSwitch::invoke(InputEventType et) {
    if ( isInvokable(et) ) {
        callbackFunction(et, *this);
    }    
}

void EventSwitch::setDebouncer(DebounceAdapter* debounceAdapter) {
    debouncer = debounceAdapter;
    if (debouncer) { //Can pass nullptr to unset?
        debouncer->setPinAdapter(pinAdapter);
        debouncer->begin();
    }
}

bool EventSwitch::changedState() {
    if ( debouncer ) {
        currentPinState = debouncer->read();
    } else {
        currentPinState = pinAdapter->read();
    }
    if ( changedPinState() && currentPinState != currentState ) {
            changeState(currentPinState);
    }
    return stateChanged;
}

bool EventSwitch::changedPinState() {
    if ( currentPinState == previousPinState ) return false;
    previousPinState = currentPinState;
    return true;
}

void EventSwitch::changeState(bool newState) {
    previousState = currentState;
    currentState = newState;
    stateChanged = true;
    durationOfPreviousState = millis() - stateChangeLastTime;
    stateChangeLastTime = millis();
}

bool EventSwitch::setDebounceInterval(unsigned int intervalMs) { 
    if ( debouncer ) {
        debouncer->setDebounceInterval(intervalMs);
        return true;
    }
    return false;
}

uint32_t EventSwitch::currentDuration() { return (millis() - stateChangeLastTime); }
