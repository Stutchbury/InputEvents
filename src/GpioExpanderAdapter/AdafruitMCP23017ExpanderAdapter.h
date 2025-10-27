// #ifdef __ADAFRUIT_MCP23XXX_H__

#ifndef AdafruitMCP23017ExpanderAdapter_h
#define AdafruitMCP23017ExpanderAdapter_h

#include <Adafruit_MCP23X17.h>
#include "Arduino.h"
#include "GpioExpanderAdapter/GpioExpanderAdapter.h"
#include "PinAdapter/PinAdapter.h"

/**
 * @brief An implementation of the GpioExpanderAdapter For Adafruit's MCP23017 library
 *
 * Details: https://learn.adafruit.com/adafruit-mcp23017-i2c-gpio-expander *
 * 
 */
class AdafruitMCP23017ExpanderAdapter : public GpioExpanderAdapter {

public:

    /**
     * @brief Construct a AdafruitMCP23017ExpanderAdapter. An Adafruit_MCP23X17 instance will be created for you.
     * 
     */
    AdafruitMCP23017ExpanderAdapter() {}

    /**
     * @brief Construct a AdafruitMCP23017ExpanderAdapter with an already created Adafruit_MCP23X17 instance.
     * 
     * @param _mcp 
     */
    AdafruitMCP23017ExpanderAdapter(Adafruit_MCP23X17& _mcp)
        : mcp(_mcp) 
        {}

    /**
     * @brief The default begin for a GPIOExpanderAdapter will use the default I2C address for the MCP23017
     * 
     */
    void begin() override {
        mcp.begin_I2C(); //The default
        delayMicroseconds(400);
        pinStates = mcp.readGPIOAB();
    }
    
    /**
     * @brief Use this method *instead of* the default begin() to specify a different I2C addreess and optional Wire.
     * 
     * @param i2c_addr The I2C address to use for the MCP23017
     * @param wire Optional
     * @return true 
     * @return false 
     */
    bool begin(uint8_t i2c_addr, TwoWire *wire = &Wire) {
        bool ret = mcp.begin_I2C(i2c_addr, wire);
        delayMicroseconds(400);
        pinStates = mcp.readGPIOAB();
        return ret;
    }
    

    /**
     * @brief Update the state of all input pins (called from loop() before updating EventButtons)
     * 
     */
    void update() override {
        pinStates = mcp.readGPIOAB();
    }

    /**
     * @brief Attach a pin and set its pin mode
     * 
     * @param pin The pin number on the expander
     * @param mode Default INPUT_PULLUP
     */
    void attachPin(byte pin, int mode = INPUT_PULLUP) override {
          mcp.pinMode(pin, mode);
    }

    /**
     * @brief 
     * @brief Returns the state of a pin on the expander
     * 
     * @param pin The pin number on the expander board
     * @return true/HIGH
     * @return false/LOW
     */
    bool read(byte pin) override {
        return bitRead(pinStates, pin);
    }

    /**
     * @brief Update the expander over I2C and return a pin state. Not recommended, use a single uptate() and then multiple pin read()s in loop().
     * 
     * @param pin The pin number on the expander board
     * @return true/HIGH
     * @return false/LOW
     */
    bool updateAndRead(byte pin) {
        return mcp.digitalRead(pin);
    }

    /**
     * @brief Write bool state to a pin
     * 
     * @details Sets the pin state directlu over I2C
     * 
     * @param pin The pin number on the expander
     * @param state true/false or HIGH/LOW
     */
    void write(byte pin, bool state) {
        mcp.digitalWrite(pin, state);
    }

    /**
     * @brief The MCP23017 can write a pins state
     * 
     * @return true 
     */
    bool canWrite() { return true; }

    virtual ~AdafruitMCP23017ExpanderAdapter() = default;

private:

    Adafruit_MCP23X17 mcp;
    uint16_t pinStates = 0;
};


#endif
// #endif
