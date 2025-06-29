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
    virtual uint32_t update() = 0;

    /**
     * @brief Returns the state of pins on the expander
     *
     * @return pin state packed in 32-bits
     */
    virtual uint32_t read() = 0;

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
    virtual void setUpdateOnRead() = 0;

    /**
     * @brief Returns true if expander is configured to perform update() on
     * read(), otherwise false.
     */
    virtual bool isUpdateOnRead() = 0;

    virtual ~GpioExpanderAdapter() = default;
};

#endif