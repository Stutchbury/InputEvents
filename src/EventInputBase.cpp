/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "EventInputBase.h"

void EventInputBase::unsetCallback() {
    callbackIsSet = false;
    #ifndef FUNCTIONAL_SUPPORTED
        setOwner(nullptr);
    #endif
}

void EventInputBase::update() {
    //fire idle timeout callback
    if ( _enabled && !idleFired && msSinceLastEvent() > idleTimeout) {
        idleFired = true;
        onIdle();
    }
}

void EventInputBase::onEnabled() { invoke(InputEventType::ENABLED); }

void EventInputBase::onDisabled() { invoke(InputEventType::DISABLED); }

void EventInputBase::onIdle() { invoke(InputEventType::IDLE); }


void EventInputBase::resetIdleTimer() { 
    lastEventMs = millis(); 
    idleFired = false;
}

void EventInputBase::resetState() {
    _enabled = true;
    input_id = 0;
    input_value = 0;
    allowAllEvents();
    idleFired = true;
}

void EventInputBase::blockEvent(InputEventType et) {
    uint8_t index = static_cast<uint8_t>(et) >> 3;    // Find the index of the array (byte position)
    uint8_t position = static_cast<uint8_t>(et) & 7; // Find the position within the byte (bit position)
    excludedEvents[index] |= (1 << position);  // Set the corresponding bit
}

void EventInputBase::allowEvent(InputEventType et) {
    uint8_t index = static_cast<uint8_t>(et) >> 3;
    uint8_t position = static_cast<uint8_t>(et) & 7;
    excludedEvents[index] &= ~(1 << position); // Clear the corresponding bit
}

void EventInputBase::blockAllEvents() {
    //for (uint8_t i = 0; i < NUM_EVENT_TYPE_ENUMS; ++i) {  
    for (uint8_t i = 0; i < static_cast<uint8_t>(InputEventType::COUNT); ++i) {  
        blockEvent(static_cast<InputEventType>(i));
    }
}

bool EventInputBase::isEventAllowed(InputEventType et) {
    uint8_t index = static_cast<uint8_t>(et) >> 3;
    uint8_t position = static_cast<uint8_t>(et) & 7;
    return (excludedEvents[index] & (1 << position)) == 0; // Check if the corresponding bit is set
}

bool EventInputBase::isInvokable(InputEventType et) {
    if ( callbackIsSet && isEventAllowed(et) ) {
        if ( et > InputEventType::IDLE ) { //Check if exent is not NONE, ENABLE, DISABLED or IDLE
            resetIdleTimer();    
        }
        return true;
    }
    return false;
}

void EventInputBase::enable(bool e ) {
    _enabled = e;
    if ( e ) {
        idleFired = true;
        onEnabled();
    } else {
        onDisabled();
    }
}
