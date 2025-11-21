#ifndef INPUT_EVENTS_BASE_TABLE_ENCODER_ADAPTER_H
#define INPUT_EVENTS_BASE_TABLE_ENCODER_ADAPTER_H

#include "IEncoderAdapter.h"
#include <Arduino.h>

/**
 * @brief A base class for encoder adapters that uses a quadrature encoder table to determine posion changes from pins.
 * 
 */
class BaseTableEncoderAdapter : public IEncoderAdapter {
public:

    /**
     * @brief Get the current position of the encoder
     * 
     * @return int32_t 
     */
    virtual int32_t getPosition() override { 
        if ( !_externalUpdate ) update();
        return _position;
    }


    virtual void setPosition(int32_t pos) { 
        _position = pos; 
    }

    /**
     * @brief Update position using the table
     * 
     */
    virtual void update() {
        uint8_t state = (readPin(_pinA) << 1) | readPin(_pinB);
        uint8_t idx = (_prevState << 2) | state;
        _position += table[idx];
        _prevState = state;
    }

    virtual ~BaseTableEncoderAdapter() {}

protected:

    /**
     * @brief Concrete implementations must provide a method to read pin values
     * 
     * @param pin 
     * @return uint8_t 
     */
    virtual uint8_t readPin(uint8_t pin) const = 0;

    // Quadrature lookup table
    static constexpr int8_t table[16] = {
        0, 1, -1, 0,
        -1, 0, 0, 1,
        1, 0, 0, -1,
        0, -1, 1, 0
    };

    uint8_t _pinA, _pinB;
    uint8_t _prevState = 0;
    int32_t _position = 0;
    bool _externalUpdate = false; ///< Some implementations may allow the update via an interupt.

};



#endif