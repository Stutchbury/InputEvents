#ifndef GpioPinAdapter_h
#define GpioPinAdapter_h

#include <Arduino.h>
#include "PinAdapter.h"

/**
 * @brief This is the default PinAdapter for regular GPIO pins.
 * 
 */
class GpioPinAdapter : public PinAdapter {

    public:
    /**
     * @brief Construct a new Gpio Pin Adapter 
     * 
     * @details By default this PinAdapter will set the pinMode to `INPUT_PULLUP`
     * 
     * If you pass mode value as `HIGH`, you must also set `EventButton::setPressedState()`,
     * `EventEncoderButton::setPressedState()` or `EventSwitch::setOnState()` to HIGH 
     * 
     * If your board does not support `INPUT_PULLDOWN`, pass `INPUT` and use an external resistor.
     * 
     * @param pin The regular GPIO pin number
     * @param pinMode Defaults to INPUT_PULLUP.
     */
    GpioPinAdapter(byte pin, uint8_t mode = INPUT_PULLUP)
    : buttonPin(pin),
      _pinMode(mode)
    { }

    void begin() {
        pinMode(buttonPin, _pinMode); //Set pullup first
        // Top tip From PJRC's Encoder - without this delay the
        // long-press doesn't fire on first press.
        // allow time for a passive R-C filter to charge
        // through the pullup resistors, before anything reads
        // the initial state
        delayMicroseconds(2000); //Delay
    }

    bool read() {
        return digitalRead(buttonPin);
    }

    private:
    byte buttonPin;
    uint8_t _pinMode = INPUT_PULLUP;

};

#endif