/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "EventEncoder.h"

#ifndef EXCLUDE_EVENT_ENCODER

#ifdef FUNCTIONAL_SUPPORTED
    #include <functional>
#endif

/**
 * Construct a rotary encoder
 */
// EventEncoder::EventEncoder(uint8_t pin1, uint8_t pin2) 
//     : encoderPin1(pin1), encoderPin2(pin2) { }

EventEncoder::EventEncoder(EncoderAdapter *encoderAdapter) {
    encoder = encoderAdapter;
}

EventEncoder::~EventEncoder() {
    delete encoder;
}

void EventEncoder::begin() {
    encoder->begin(); // = new Encoder(encoderPin1, encoderPin2); 
}

void EventEncoder::invoke(InputEventType et) {
    if ( isInvokable(et) ) {
        callbackFunction(et, *this);
    }    
}


void EventEncoder::onEnabled() {
    //Reset the encoder so we don't trigger other events
    //idleFlagged = true;
    //encoder->write(encoderPosition*positionDivider);
    encoder->setPosition(currentPosition*positionDivider);
    invoke(InputEventType::ENABLED);
}

void EventEncoder::unsetCallback() {
    callbackFunction = nullptr;
    EventInputBase::unsetCallback();
}

void EventEncoder::update() {
    // @TODO Do we store the current position when disabled and update if re-enabled?
    if ( _enabled ) {
        //encoder udate (fires encoder rotation callbacks)
        if ( (millis() - rateLimitCounter) >= rateLimit ) { 
                readIncrement();
            if ( encoderIncrement !=0 ) {
                currentPosition += encoderIncrement;
                invoke(InputEventType::CHANGED);
            }
            rateLimitCounter = millis();
        }
        EventInputBase::update();
    }
}

void EventEncoder::readIncrement() {
    long newPosition = floor(encoder->getPosition()/positionDivider);
    encoderIncrement = newPosition - oldPosition;
    oldPosition = newPosition;
}

#else 

#pragma message("Info: EventEncoder and EventEncoderButton excluded from your build. Install the EncoderAdapter library, an Encoder library and #include the required adapter class (eg <PjrcEncoderAdapter.h>) to resolve.")

#endif
