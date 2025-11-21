#ifndef INPUT_EVENTS_EXPANDER_ENCODER_ADAPTER_H
#define INPUT_EVENTS_EXPANDER_ENCODER_ADAPTER_H


#include "BaseTableEncoderAdapter.h"
#include "GpioExpanderAdapter/GpioExpanderAdapter.h"

class ExpanderEncoderAdapter : public BaseTableEncoderAdapter {


public:

    /**
     * @brief Construct a ExpanderEncoderAdapter
     * 
     * @param encoderPinA The pin on the expander biard for the encoder A pin
     * @param encoderPinB The pin on the expander biard for the encoder B pin
     * @param expander Passed by reference because the `GpioExpanderAdapter::update()` is always called from `loop()` or interrupt
     * @param externalUpdate Default is false and this class's update() is called in getPosition(). If true, ExpanderEncoderAdapter::update() must be called called from interrupt or loop().
     */
    ExpanderEncoderAdapter( uint8_t encoderPinA, uint8_t encoderPinB, GpioExpanderAdapter& expander, bool externalUpdate=false ) 
        : expanderAdapter(&expander)  
        { 
            _pinA = encoderPinA;
            _pinB = encoderPinB;
            _externalUpdate = externalUpdate;
        }

    bool begin() override {
        expanderAdapter->attachPin(_pinA);
        expanderAdapter->attachPin(_pinB);
        return true;
    }

protected:
    
    uint8_t readPin(uint8_t pin) const override {
        return expanderAdapter->read(pin);
    }


private:
    GpioExpanderAdapter* expanderAdapter;

};



#endif