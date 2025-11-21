#ifndef INPUT_EVENTS_SLOW_EXPANDER_ENCODER_ADAPTER_H
#define INPUT_EVENTS_SLOW_EXPANDER_ENCODER_ADAPTER_H


#include "BaseTableEncoderAdapter.h"
#include "GpioExpanderAdapter/GpioExpanderAdapter.h"


/**
 * @brief A very basic IEncoderAdapter for slow GPIO Expanders.
 * 
 * @details This encoder adapter only checks for a single state change instead of the usual four in a quadrature encoder.
 * This may provide more reliable events for where encoder movement is slow and/or the loop() cannot keep up. 
 * 
 */
class SlowExpanderEncoderAdapter : public IEncoderAdapter {


public:

    /**
     * @brief Construct a ExpanderEncoderAdapter
     * 
     * @param encoderPinA The pin on the expander biard for the encoder A pin
     * @param encoderPinB The pin on the expander biard for the encoder B pin
     * @param expander Passed by reference because the `GpioExpanderAdapter::update()` is always called from `loop()` or interrupt
     */
    SlowExpanderEncoderAdapter( uint8_t encoderPinA, uint8_t encoderPinB, GpioExpanderAdapter& expander ) 
        : expanderAdapter(&expander)  
        { 
            _pinA = encoderPinA;
            _pinB = encoderPinB;
        }

    bool begin() override {
        expanderAdapter->attachPin(_pinA);
        expanderAdapter->attachPin(_pinB);
        return true;
    }

    /**
     * @brief Get the current position of the encoder
     * 
     * @return int32_t 
     */
    virtual int32_t getPosition() override { 
        update();
        return _position;
    }


    virtual void setPosition(int32_t pos) { 
        _position = pos; 
    }

    /**
     * @brief Update position using the table
     * 
     * 
     */
    virtual void update() {
        uint8_t stateA = expanderAdapter->read(_pinA);
        uint8_t stateB = expanderAdapter->read(_pinB);

        // Detect rising edge on pin A
        // We are only updating once per four state changes, so multiply position
        // because EventEncoder expects all positions to be counted.
        if (prevStateA == LOW && stateA == HIGH) {
            if (stateB == LOW) {
                _position -= _positionMultiplier;
            } else {
                _position += _positionMultiplier;
            }
        }
        prevStateA = stateA;
        prevStateB = stateB;
    }




protected:
    

private:
    GpioExpanderAdapter* expanderAdapter;
    uint8_t _pinA = HIGH, _pinB = HIGH, prevStateA = HIGH, prevStateB = HIGH;
    int32_t _position = 0;
    uint8_t _positionMultiplier = 4; ///< Default to 4 to emulate a quadrature encoder
};



#endif