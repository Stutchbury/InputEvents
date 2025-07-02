#ifndef HC165ExpanderAdapter_h
#define HC165ExpanderAdapter_h

#include "Arduino.h"
#include "GpioExpanderAdapter/GpioExpanderAdapter.h"
#include "PinAdapter/PinAdapter.h"

/**
 * @brief An implementation of the GpioExpanderAdapter using 74HC165 shift register(s)
 *
 * Datasheet:
 * https://www.ti.com/lit/ds/symlink/sn74hc165.pdf
 *
 * Supports up to 4 cascaded 74HC165 shift registers, returning 32 pin states (read only)
 * 
 * pin 0 is pin A on first 74HC165, pin 8 is pin A on second 74HC165, etc.
 */
class HC165ExpanderAdapter : public GpioExpanderAdapter {
    static const int cascadeDefaultLength = 1; // single 74HC164 is default
    static const int cascadeMaxLength = 4;     // max number of 74HC164 supported by this implementation
    static const int piPinCount = 8;           // number of parallel input pins
  public:

    /**
     * @brief Construct a new HC165ExpanderAdapter 
     * 
     * @param dataPin Serial data
     * @param clockPin Used to shift to next bit
     * @param shldPin Shift/Load input
     * @param cascadeLength The number of 74HC165s chained together (1-4, default is 1)
     */
    HC165ExpanderAdapter(byte dataPin, byte clockPin, byte shldPin, byte cascadeLength = cascadeDefaultLength)
        : dataPin(dataPin), 
          clockPin(clockPin), 
          shldPin(shldPin), 
          cascadeLength(cascadeLength)
        {
            cascadeLength = cascadeLength > cascadeMaxLength ? cascadeMaxLength : cascadeLength;
        }

    /**
     * @brief Initialize the pin expander.
     *
     */
    void begin() override {
        pinMode(dataPin, INPUT);
        pinMode(clockPin, OUTPUT);
        pinMode(shldPin, OUTPUT);
    }

    /**
     * @brief Read the state of pins on the 74HC165 cascade
     *
     */
    void update() override {
        // Step 1: Sample
        digitalWrite(shldPin, LOW);
        digitalWrite(shldPin, HIGH);

        // Step 2: Read & Shift
        for (int i = 0; i < cascadeLength * piPinCount; i++) {
            int pin = digitalRead(dataPin);
            if (pin == HIGH)
                bitSet(pins, i);
            else
                bitClear(pins, i);

            // Shift out the next bit
            digitalWrite(clockPin, HIGH);
            digitalWrite(clockPin, LOW);
        }
    }

    /**
     * @brief Returns the state of a pin on the expander
     *
     * @return true/HIGH
     * @return false/LOW
     */
    bool read(byte pin) override {
        return bitRead(pins, pin);
    }

    /**
     * @brief  pinMode not supported by 74HC165 so do nothing
     * 
     * @param pin 
     * @param mode 
     */
    void attachPin(byte pin, int mode = INPUT_PULLUP) override {}

    ~HC165ExpanderAdapter() {}

  private:
    byte dataPin;
    byte clockPin;
    byte shldPin;
    int cascadeLength;
    uint32_t pins = 0;
};

#endif