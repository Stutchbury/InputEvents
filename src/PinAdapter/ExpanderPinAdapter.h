#ifndef INPUT_EVENTS_EXPANDER_PIN_ADAPTER_H
#define INPUT_EVENTS_EXPANDER_PIN_ADAPTER_H

#include "Arduino.h"
#include "PinAdapter.h"
#include "GpioExpanderAdapter/GpioExpanderAdapter.h"

/**
 * @brief Implementation of a PinAdapter that reads from a GpioExpanderAdapter
 *
 */
class ExpanderPinAdapter : public PinAdapter {
    public:

    /**
     * @brief Construct a new ExpanderPinAdapter with a GpioExpanderAdapter and optional pin mode (default INPUT_PULLUP)
     * 
     * @param pin The pin number
     * @param expander A reference to a GpioExpanderAdapter
     * @param mode The pin mode. Not all GpioExpanderAdapters support setting pinMode but should be set to reflect the physical wiring on the pin.
     */
    ExpanderPinAdapter(byte pin, GpioExpanderAdapter& expander, int mode = INPUT_PULLUP)
        : pin(pin),
          mode(mode),
          expanderAdapter(&expander)
        {}

    /**
     * @brief Attach the pin to the GpioExpanderAdapter which will set the pinMode if required/possible
     * 
     * @return * void 
     */
    void begin() override {
        if ( !expanderAdapter ) return;
        expanderAdapter->attachPin(pin, mode);
    }

    /**
     * @brief Return the state of this pin. If the GpioPinAdapter does not exist will return 'OFF' state (this should never happen!)
     * 
     * @return true Actual pin state aka HIGH
     * @return false Actual pin state aka LOW
     */
    bool read() override { 
        if ( !expanderAdapter ) return (mode == INPUT_PULLUP); //For safety, this should return the 'OFF' state based on pinMode
        return expanderAdapter->read(pin); 
    }

    private:
    byte pin;
    int mode;
    GpioExpanderAdapter* expanderAdapter;
};

#endif
