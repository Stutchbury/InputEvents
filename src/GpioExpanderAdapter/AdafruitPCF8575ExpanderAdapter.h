#ifndef AdafruitPCF8575ExpanderAdapter_h
#define AdafruitPCF8575ExpanderAdapter_h

#include <Adafruit_PCF8575.h>
#include "Arduino.h"
#include "GpioExpanderAdapter/GpioExpanderAdapter.h"
#include "PinAdapter/PinAdapter.h"

/**
 * @brief An implementation of the GpioExpanderAdapter For Adafruit's PCF8575 library
 *
 * Details: https://learn.adafruit.com/adafruit-pcf8575
 * 
 */
class AdafruitPCF8575ExpanderAdapter : public GpioExpanderAdapter {

public:

    /**
     * @brief Construct a AdafruitPCF8575ExpanderAdapter. An Adafruit_PCF8575 instance will be created for you.
     * 
     */
    AdafruitPCF8575ExpanderAdapter() {}

    /**
     * @brief Construct a AdafruitPCF8575ExpanderAdapter with an already created Adafruit_PCF8575 instance.
     * 
     * @param _pcf 
     */
    AdafruitPCF8575ExpanderAdapter(Adafruit_PCF8575& _pcf)
        : pcf(_pcf) 
        {}

    /**
     * @brief The default begin for a GPIOExpanderAdapter will use the default I2C address for the PCF8575
     * 
     */
    void begin() override {
        pcf.begin(); //Use defaults
        delayMicroseconds(400);
        pinStates = pcf.digitalReadWord();
    }
    
    /**
     * @brief Use this method *instead of* the default begin() to specify a different I2C addreess.
     * 
     * @param i2c_addr The I2C address to use for the PCF8575
     * @param wire Optional
     * @return true 
     * @return false 
     */
    bool begin(uint8_t i2c_addr, TwoWire *wire = &Wire) {
        bool ret = pcf.begin(i2c_addr, wire);
        delayMicroseconds(400);
        pinStates = pcf.digitalReadWord();
        return ret;
    }
    

    /**
     * @brief Update the state of all input pins (called from loop() before updating EventButtons)
     * 
     */
    void update() override {
        pinStates = pcf.digitalReadWord();
    }

    /**
     * @brief Attach a pin and set its pin mode
     * 
     * @param pin The pin number on the expander
     * @param mode Default INPUT_PULLUP
     */
    void attachPin(byte pin, int mode = INPUT_PULLUP) override {
          pcf.pinMode(pin, mode);
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
        return pcf.digitalRead(pin);
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
        pcf.digitalWrite(pin, state);
    }

    /**
     * @brief The PCF8575 can write a pins state
     * 
     * @return true 
     */
    bool canWrite() { return true; }

    virtual ~AdafruitPCF8575ExpanderAdapter() = default;

private:

    Adafruit_PCF8575 pcf;
    uint16_t pinStates = 0;
};


#endif
