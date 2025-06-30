#ifndef HC165ExpanderAdapter_h
#define HC165ExpanderAdapter_h

#include "Arduino.h"
#include "GpioExpanderAdapter/GpioExpanderAdapter.h"
#include "PinAdapter/PinAdapter.h"

/**
 * @brief Implementation of pin expander using 74HC165 shift register(s)
 *
 * Datasheet:
 * https://www.ti.com/lit/ds/symlink/sn74hc165.pdf
 *
 * Supports up to 4 cascaded 74HC165 shift registers, returning 32 pin states.
 * pin 0 is pin A on first 74HC165, pin 8 is pin A on second 74HC165, etc.
 */
class HC165ExpanderAdapter : public GpioExpanderAdapter {
    static const int cascadeDefaultLength = 1; // single 74HC164 is default
    static const int cascadeMaxLength = 4;     // max number of 74HC164 supported by this implementation
    static const int piPinCount = 8;           // number of parallel input pins
  public:
    HC165ExpanderAdapter(byte dataPin, byte clockPin, byte shldPin, byte cascadeLength = cascadeDefaultLength,
                         bool updateOnRead = false)
        : dataPin(dataPin), clockPin(clockPin), shldPin(shldPin), initialized(false), cascadeLength(cascadeLength),
          updateOnRead(updateOnRead) {
        cascadeLength = cascadeLength > cascadeMaxLength ? cascadeMaxLength : cascadeLength;
    }

    /**
     * @brief Initialize the pin expander.
     *
     */
    void begin() {
        if (initialized)
            return;

        pinMode(dataPin, INPUT);
        pinMode(clockPin, OUTPUT);
        pinMode(shldPin, OUTPUT);

        initialized = true;
    }

    /**
     * @brief Read the state of pins on the 74HC165 cascade
     *
     * @return pin state packed in 32-bits
     */
    void update() {
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

    // not supported by 74HC165
    void attachPin(byte pin, int mode = INPUT_PULLUP) {}

    // not supported by 74HC165
    void attachPin(PinAdapter *pin) {}

    ~HC165ExpanderAdapter() {}

  private:
    bool initialized;
    byte dataPin;
    byte clockPin;
    byte shldPin;
    int cascadeLength;
    bool updateOnRead;
};

#endif