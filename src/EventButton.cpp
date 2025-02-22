/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#include "EventButton.h"


EventButton::EventButton(byte pin)
    : buttonPin(pin), bounce(new Bounce()) { }

void EventButton::begin() {
    pinMode(buttonPin, INPUT_PULLUP); //Set pullup first
    // Top tip From PJRC's Encoder - without this delay the
    // long-press doesn't fire on first press.
    // allow time for a passive R-C filter to charge
    // through the pullup resistors, before reading
    // the initial state
    delayMicroseconds(2000); //Delay
    bounce->attach(buttonPin, INPUT_PULLUP); //then attach button
}

void EventButton::unsetCallback() {
    callbackFunction = nullptr;
    EventInputBase::unsetCallback();
}

void EventButton::update() {
    if (_enabled) {
        //button update (fires pressed/released callbacks)
        if (bounce->update()) {
            _buttonState = bounce->read();
            if (bounce->fell()) {
                previousState = HIGH;
                //onPressed();
                invoke(InputEventType::PRESSED);

            } else if (bounce->rose()) {
                if (previousState == HIGH) { //Why would previousState *not* be high?
                    clickFired = false;
                    clickCounter++;
                    prevClickCount = clickCounter;
                }
                previousState = LOW;
                invoke(InputEventType::RELEASED);
            }
        }
        //fire long press callbacks
        if (LOW == bounce->read()) {
            resetIdleTimer();
            if (bounce->currentDuration() > (uint16_t)(longClickDuration + (longPressCounter * longPressInterval ))) {
                longPressCounter++;
                if ((repeatLongPress || longPressCounter == 1) ) {
                    invoke(InputEventType::LONG_PRESS);
                }
            }
        }
        //fire button click callbacks
        if (!clickFired && _buttonState == HIGH && bounce->currentDuration() > multiClickInterval) {
            clickFired = true;
            if (bounce->previousDuration() > longClickDuration) {
                clickCounter = 0;
                prevClickCount = 1;
                invoke(InputEventType::LONG_CLICKED);
                longPressCounter = 0;
            } else {
                if ( clickCounter == 1 ) {
                    invoke(InputEventType::CLICKED);
                } else if (clickCounter == 2 ) {
                    invoke(InputEventType::DOUBLE_CLICKED);
                } else {
                    invoke(InputEventType::MULTI_CLICKED);
                }
                clickCounter = 0;
            }
        }
        EventInputBase::update();
    }
}


void EventButton::invoke(InputEventType et) {
    if ( isInvokable(et) ) {
        callbackFunction(et, *this);
    }    
}

void EventButton::onDisabled() {
    //Reset button state
    clickCounter = 0;
    longPressCounter = 0;
    invoke(InputEventType::DISABLED);
}


bool EventButton::buttonState() { return bounce->read(); }

bool EventButton::isPressed() { return buttonState() == LOW; }

void EventButton::setDebounceInterval(unsigned int intervalMs) { bounce->interval(intervalMs); }

uint32_t EventButton::currentDuration() { return bounce->currentDuration(); }

uint32_t EventButton::previousDuration() { return bounce->previousDuration(); }
