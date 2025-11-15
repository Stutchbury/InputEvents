#ifndef INPUT_EVENTS_ADAFRUIT_PCF8574_EXPANDER_ADAPTER_H
#define INPUT_EVENTS_ADAFRUIT_PCF8574_EXPANDER_ADAPTER_H

#include <Arduino.h>
#include <new>
#include <Adafruit_PCF8574.h>
#include "GpioExpanderAdapter/GpioExpanderAdapter.h"
#include "PinAdapter/PinAdapter.h"

/**
 * @brief An implementation of the GpioExpanderAdapter For Adafruit's PCF8574 library
 *
 * Details: https://learn.adafruit.com/adafruit-pcf8574
 * 
 */
class AdafruitPCF8574ExpanderAdapter : public GpioExpanderAdapter {

public:

    /**
     * @brief Construct a AdafruitPCF8574ExpanderAdapter. An Adafruit_PCF8574 instance will be created for you.
     * 
     * @details This will create and Adafruit_PCF8574 on the stack - creating one on the heap causes a crash as 
     * the Adafruit lib tries to delete an object that doesn't yet exist for ESP32 in the begin(). 
     * 
     */
    AdafruitPCF8574ExpanderAdapter() {
        // We have to create the instance on the stack because Adafruit does 'clever' stuff with Wire and
        // tries to delete an instance that doesn't yet exist...
        //NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        pcf = new (&pcfStorage) Adafruit_PCF8574();
    }

    /**
     * @brief Construct a AdafruitPCF8574ExpanderAdapter with an already created Adafruit_PCF8574 instance.
     * 
     * @param _pcf 
     */
    AdafruitPCF8574ExpanderAdapter(Adafruit_PCF8574& _pcf)
        : pcf(&_pcf) 
        {}

    /**
     * @brief The default begin for a GPIOExpanderAdapter will use the default I2C address for the PCF8574
     * 
     */
    void begin() override {
        pcf->begin(); //Use defaults
        delayMicroseconds(400);
        pinStates = pcf->digitalReadByte();
    }
    
    /**
     * @brief Use this method *instead of* the default begin() to specify a different I2C addreess and optional Wire.
     * 
     * @param i2c_addr The I2C address to use for the PCF8574
     * @param wire Optional
     * @return true 
     * @return false 
     */
    bool begin(uint8_t i2c_addr, TwoWire *wire = &Wire) {
        bool ret = pcf->begin(i2c_addr, wire);
        delayMicroseconds(400);
        pinStates = pcf->digitalReadByte();
        return ret;
    }
    

    /**
     * @brief Update the state of all input pins (called from loop() before updating EventButtons)
     * 
     */
    void update() override {
        pinStates = pcf->digitalReadByte();
    }

    /**
     * @brief Attach a pin and set its pin mode
     * 
     * @param pin The pin number on the expander
     * @param mode Default INPUT_PULLUP
     */
    void attachPin(byte pin, int mode = INPUT_PULLUP) override {
          pcf->pinMode(pin, mode);
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
        return pcf->digitalRead(pin);
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
        pcf->digitalWrite(pin, state);
    }

    /**
     * @brief The PCF8574 can write a pins state
     * 
     * @return true 
     */
    bool canWrite() { return true; }

    virtual ~AdafruitPCF8574ExpanderAdapter() = default;

private:

    alignas(Adafruit_PCF8574) uint8_t pcfStorage[sizeof(Adafruit_PCF8574)]; ///< Used with placement new to avoid heap allocation.
    Adafruit_PCF8574* pcf;
    uint8_t pinStates = 0;
};


#endif
