#ifndef GpioExpanderAdapter_h
#define GpioExpanderAdapter_h

#include "Arduino.h"
#include "PinAdapter/PinAdapter.h"

/**
 * @brief The interface specification for GPIO expander.
 *
 * Supports 32 pins on a single expander.
 */
class GpioExpanderAdapter {
  public:
    /**
     * @brief Initialize the expander. (Idempotent)
     *
     */
    virtual void begin() = 0;

    /**
     * @brief Read the state of pins on the expander
     *
     * @return pin state packed in 32-bits
     */
    virtual void update() = 0;

    /**
     * @brief Returns the state of a pin on the expander
     *
     * @return true/HIGH
     * @return false/LOW
     */
    bool read(byte pin) {
        if (updateOnRead)
            update();
        return bitRead(pins, pin);
    }

    /** @brief Use it to configure individual pin mode, if expander allows it.
     * Not all of them do.
     */
    virtual void attachPin(byte pin, int mode = INPUT_PULLUP) = 0;

    /** @brief Use it to configure individual pin mode, if expander allows it.
     * Not all of them do.
     */
    virtual void attachPin(PinAdapter *pin) = 0;

    /**
     * @brief Configures the expander that calling read() will also update it.
     * Used in slow polling apps. Default false
     */
    void setUpdateOnRead() { updateOnRead = true; }

    /**
     * @brief Returns true if expander is configured to perform update() on
     * read(), otherwise false.
     */
    bool isUpdateOnRead() { return updateOnRead; }

    virtual ~GpioExpanderAdapter() = default;

  protected:
    bool updateOnRead;
    uint32_t pins;
};

#endif