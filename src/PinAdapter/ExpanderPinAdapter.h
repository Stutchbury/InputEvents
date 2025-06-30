#ifndef ExpanderPinAdapter_h
#define ExpanderPinAdapter_h

#include "Arduino.h"
#include "GpioExpanderAdapter/GpioExpanderAdapter.h"
#include "PinAdapter.h"

/**
 * @brief Implements a PinAdapter accessible via GpioExpanderAdapter
 *
 */
class ExpanderPinAdapter : public PinAdapter {
  public:
    ExpanderPinAdapter(byte pin, GpioExpanderAdapter *expander, int mode = INPUT_PULLUP)
        : pin(pin), expander(expander), mode(mode), initialized(false) {}
    void begin() {
        if (initialized)
            return;

        expander->begin();
        expander->attachPin(pin, mode);
        initialized = true;
    }
    bool read() { return expander->read(pin); }

  private:
    bool initialized;
    byte pin;
    int mode;
    GpioExpanderAdapter *expander;
};

#endif
