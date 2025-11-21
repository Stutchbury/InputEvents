
/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "EventEncoderButton.h"

EventEncoderButton::EventEncoderButton(IEncoderAdapter *encoderAdapter, byte buttonPin, bool useDefaultDebouncer /*=true*/)
    : encoder(encoderAdapter), button(buttonPin, useDefaultDebouncer) {
        setCallbacks();
    }

EventEncoderButton::EventEncoderButton(IEncoderAdapter *encoderAdapter, PinAdapter* _pinAdapter, bool useDefaultDebouncer /*=true*/)
    : encoder(encoderAdapter), button(_pinAdapter, useDefaultDebouncer) {
        setCallbacks();
    }

EventEncoderButton::EventEncoderButton(IEncoderAdapter *encoderAdapter, PinAdapter* _pinAdapter, DebounceAdapter* debounceAdapter)
    : encoder(encoderAdapter), button(_pinAdapter, debounceAdapter) {
        setCallbacks();
    }

void EventEncoderButton::setCallbacks() {
    #ifdef FUNCTIONAL_SUPPORTED
    encoder.setCallback([&](InputEventType et, EventEncoder &enc) { onInputCallback(et, enc); });
    button.setCallback([&](InputEventType et, EventButton &btn) { onInputCallback(et, btn); });
    #else
    encoder.setOwner(this);
    encoder.setCallback(EventEncoderButton::encoderCallback);
    button.setOwner(this);
    button.setCallback(EventEncoderButton::buttonCallback);
    #endif
}

void EventEncoderButton::begin() {
    encoder.begin();
    button.begin();
}

void EventEncoderButton::unsetCallback() {
    callbackFunction = nullptr;
    EventInputBase::unsetCallback();
}

void EventEncoderButton::update() {
    encoder.update();
    button.update();
}

void EventEncoderButton::invoke(InputEventType et) {
    if ( isInvokable(et) ) {
        callbackFunction(et, *this);
    }    
}

void EventEncoderButton::onEnabled() {
    encoder.enable();
    button.enable();
    invoke(InputEventType::ENABLED);
}

void EventEncoderButton::onDisabled() {
    encoder.enable(false);
    button.enable(false);
    invoke(InputEventType::DISABLED);
}

void EventEncoderButton::onInputCallback(InputEventType et, EventInputBase & ie) {
    //Only fire ENABLED and DISABLED events from EventEncoderButton
    if ( et == InputEventType::ENABLED || et == InputEventType::DISABLED) return;

    //Only fire IDLE if both the encoder and button are idle
    if ( et == InputEventType::IDLE && !(encoder.isIdle() && button.isIdle()  ) ) return;

    if ( et == InputEventType::CHANGED ) {
        if (!onEncoderChanged() ) return; 
        // Convert CHANGED to CHANGED_PRESSED if button is pressed
        if ( encodingPressed ) {
            et = InputEventType::CHANGED_PRESSED;
            encodingPressedCount++;
        }
    }    
    if ( encodingPressed ) {
        //Stop LONG_PRESS    
        if (et == InputEventType::LONG_PRESS) {
            return;
        }
        // Convert RELEASED to CHANGED_RELEASED
        if ( et == InputEventType::RELEASED && encodingPressedCount != 0 ) {
            et = InputEventType::CHANGED_RELEASED;
        }
        //Stop CLICKED and LONG_CLICKED (it will only be one of them)
        if (  et == InputEventType::CLICKED || et == InputEventType::LONG_CLICKED )  {
            encodingPressed = false;
            encodingPressedCount = 0;
            return;
        }
    }

    //Translate the callback
    invoke(et);    
}

/*
 * Set current/previous increment, check limits and return true only if increment != 0
 * This will only be called if encoder has CHANGED (ie encoder.increment will never be zero )
 */
bool EventEncoderButton::onEncoderChanged() {
    currentIncrement = encoder.increment();
    if ( button.isPressed() ) {
        previousPressedPosition = currentPressedPosition;
        currentPressedPosition += currentIncrement;
        encodingPressed = true;
        // Do not check position limits unless both min/max are non-zero
        if ( minPressedPos == 0 && maxPressedPos == 0 ) {
            return true; //Increment will always be non-zero
        }
        if ( currentPressedPosition > maxPressedPos ) {
            currentPressedPosition = (wrapMinMaxPressedPos ? minPressedPos : maxPressedPos);
            currentIncrement = currentPressedPosition - previousPressedPosition;
        } else if ( currentPressedPosition < minPressedPos ) {
            currentPressedPosition = (wrapMinMaxPressedPos ? maxPressedPos : minPressedPos);
            currentIncrement = currentPressedPosition - previousPressedPosition;
        }
    } else {
        previousPosition = currentPosition;
        currentPosition += currentIncrement;
        // Do not check position limits unless both min/max are non-zero
        if ( minPos == 0 && maxPos == 0 ) {
            return true; //Increment will always be non-zero
        }
        if ( currentPosition > maxPos ) {
            currentPosition = (wrapMinMaxPos ? minPos : maxPos);
            currentIncrement = currentPosition - previousPosition;
        } else if ( currentPosition < minPos ) {
            currentPosition = (wrapMinMaxPos ? maxPos : minPos);
            currentIncrement = currentPosition - previousPosition;
        }
    }
    return (currentIncrement != 0);
}

void EventEncoderButton::setMinPosition(int32_t minPosition) { 
    minPos = minPosition;
    if ( currentPosition < minPos ) {
        currentPosition = minPos;
        previousPosition = currentPosition;
    }
}

void EventEncoderButton::setMaxPosition(int32_t maxPosition) {
    maxPos = maxPosition; 
    if ( currentPosition > maxPos ) {
        currentPosition = maxPos;
        previousPosition = currentPosition;
    }
}

void EventEncoderButton::setMinPressedPosition(int32_t minPressedPosition) { 
    minPressedPos = minPressedPosition; 
    if ( currentPressedPosition < minPressedPos ) {
        currentPressedPosition = minPressedPos;
        previousPressedPosition = currentPressedPosition;
    }
}

void EventEncoderButton::setMaxPressedPosition(int32_t maxPressedPosition) { 
    maxPressedPos = maxPressedPosition; 
    if ( currentPressedPosition > maxPressedPos ) {
        currentPressedPosition = maxPressedPos;
        previousPressedPosition = currentPressedPosition;
    }
}

void EventEncoderButton::setRateLimit(long ms) { encoder.setRateLimit(ms); }

void EventEncoderButton::setPositionDivider(uint8_t divider /*=4*/) { encoder.setPositionDivider(divider); }

uint8_t EventEncoderButton::getPositionDivider() {return encoder.getPositionDivider(); }

bool EventEncoderButton::isPressed() { return button.isPressed(); }

void EventEncoderButton::setDebouncer(DebounceAdapter* debounceAdapter) { button.setDebouncer(debounceAdapter); }

bool EventEncoderButton::setDebounceInterval(uint16_t intervalMs) { return button.setDebounceInterval(intervalMs); }

void EventEncoderButton::setPressedState(bool state /*= LOW*/) { button.setPressedState(state); }

void EventEncoderButton::setMultiClickInterval(uint16_t intervalMs) { button.setMultiClickInterval(intervalMs); }

void EventEncoderButton::setLongClickDuration(uint16_t longDurationMs) { button.setLongClickDuration(longDurationMs); }

void EventEncoderButton::enableLongPressRepeat(bool repeat /*=true*/) { button.enableLongPressRepeat(repeat); }

void EventEncoderButton::setLongPressInterval(uint16_t intervalMs) { button.setLongPressInterval(intervalMs); }

unsigned char EventEncoderButton::clickCount() { return button.clickCount(); }

uint32_t EventEncoderButton::currentDuration() { return button.currentDuration(); }

uint32_t EventEncoderButton::previousDuration() { return button.previousDuration(); }

uint8_t EventEncoderButton::longPressCount() { return button.longPressCount(); }

void EventEncoderButton::resetState() {
    button.resetState();
    encoder.resetState();
}


