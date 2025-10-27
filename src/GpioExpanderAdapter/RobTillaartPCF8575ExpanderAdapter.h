// #ifdef __ADAFRUIT_MCP23XXX_H__

#ifndef RobTillaartPCF8575ExpanderAdapter_h
#define RobTillaartPCF8575ExpanderAdapter_h

#include "Arduino.h"
#include <PCF8575.h>
#include "GpioExpanderAdapter/GpioExpanderAdapter.h"
#include "PinAdapter/PinAdapter.h"

/**
 * @brief An implementation of the GpioExpanderAdapter RobTillaart's PCF8575 library
 *
 * Note: You may need to call Wire.begin() in your sketch (if not already called) as the underlying library does not do so.
 * 
 * Repository: https://github.com/RobTillaart/PCF8575
 * 
 */
class RobTillaartPCF8575ExpanderAdapter : public GpioExpanderAdapter {

public:

    /**
     * @brief Construct a RobTillaartPCF8575ExpanderAdapter using the default I2C address
     * 
     */
    RobTillaartPCF8575ExpanderAdapter() 
        : pcf(0x20) 
        {}

    /**
     * @brief Construct a RobTillaartPCF8575ExpanderAdapter using a specified I2C address and otional Wire.
     * 
     * @param deviceAddress 
     * @param wire 
     */
    RobTillaartPCF8575ExpanderAdapter(const uint8_t deviceAddress, TwoWire *wire = &Wire) 
        : pcf(deviceAddress, wire) 
        {}

    /**
     * @brief Construct a RobTillaartPCF8575ExpanderAdapter using an already created PCF8575 instance.
     * 
     * @param _pcf 
     */
    RobTillaartPCF8575ExpanderAdapter(PCF8575& _pcf)
        : pcf(_pcf)
        {}

    /**
     * @brief The default begin for a GPIOExpanderAdapter
     * 
     */
    void begin() override {
        pcf.begin();
        delayMicroseconds(400);
        pinStates = pcf.read16();
    }

    /**
     * @brief Update the state of all input pins (called from loop() before updating EventButtons)
     * 
     */
    void update() override {
        pinStates = pcf.read16();
    }

    /**
     * @brief Attach a pin and set its pin mode
     * 
     * @param pin The pin number on the expander
     * @param mode Default INPUT_PULLUP
     */
    void attachPin(byte pin, int mode = INPUT_PULLUP) override {

    }

    /**
     * @brief Returns the state of a pin on the expander
     * 
     * @param pin The pin number on the expander
     * @return true/HIGH
     * @return false/LOW
     */
    bool read(byte pin) override {
        return bitRead(pinStates, pin);
        //return pcf.read(pin);
    }

    /**
     * @brief Update the expander over I2C and return a pin state. Not recommended, use a single uptate() and then multiple pin read()s in loop().
     * 
     * @param pin The pin number on the expander board
     * @return true/HIGH
     * @return false/LOW
     */
    bool updateAndRead(byte pin) {
        return pcf.read(pin);
    }

    /**
     * @brief Write bool state to a pin
     * 
     * @param pin The pin number on the expander
     * @param state true/false or HIGH/LOW
     */
    void write(byte pin, bool state) {
    }

    /**
     * @brief The MCP23017 can write a pins state
     * 
     * @return true 
     */
    bool canWrite() { return true; }

    virtual ~RobTillaartPCF8575ExpanderAdapter() = default;

private:

    PCF8575 pcf;
    uint16_t pinStates = 0;
};


#endif
// #endif
