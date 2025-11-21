/**
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2025 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENT_PJRC_ENCODER_ADAPTER_H
#define INPUT_EVENT_PJRC_ENCODER_ADAPTER_H

// Only create the adapter class if PJRC's Encoder is already included.
#ifdef Encoder_h_

#include <EncoderAdapter/IEncoderAdapter.h>

/**
 * @brief An adapter for PJRC's Encoder.
 * 
 * @details One of the best (and most popular) encoder libraries. Initially written for Teensy but now suppoers many other boards.
 * Repo: https://github.com/PaulStoffregen/Encoder
 * Unfortunately the releases are a little behind the repository updates, so if your board is not working with the 
 * latest library release, try downloading from github and installing locally.
 * 
 */
class PjrcEncoderAdapter : public IEncoderAdapter {

public:

    /**
     * @brief Construct a new Pjrc Encoder Adapter object. An Encoder instance will be created for you.
     * 
     * @param encoderPin1 The first encoder pin (sometimes known as pin A)
     * @param encoderPin2 The second encoder pin (sometimes known as pin B)
     */
    PjrcEncoderAdapter( uint8_t encoderPin1, uint8_t encoderPin2 )
        {
            pinA = encoderPin1;
            pinB = encoderPin2;
        };

    /**
     * @brief Destructor to ensure the PJRC encoder is deleted.
     * 
     */
    ~PjrcEncoderAdapter() {
        delete encoder;
    }


    /**
     * @brief In Encoder < v1.4.4 begin is not used but when v1.4.5 is released we will use it to initialise the encoder in setup for better compatibility with some boards.
     * 
     * @return true 
     * @return false 
     */
    bool begin(void) {
        encoder = new Encoder(pinA, pinB); 
        // PJRC's Encoder appears to have begin() functionality on its way but something has gone wrong with the versioning 
        // https://github.com/PaulStoffregen/Encoder/issues/106#issuecomment-2621583974
        //encoder->begin(pinA, pinB);
        return true;
    }

    /**
     * @brief Get the current position as reported by the Encoder class.
     * 
     * @return int32_t The position. May be positive or negative.
     */
    int32_t getPosition(void) {
        return encoder->read();
    }

    /**
     * @brief Set the a new position of the encoder. This can be any value in the int32_t range.
     * 
     * @param pos The desired new position.
     */
    void setPosition(int32_t pos) {
        encoder->write(pos);
    }

private:


    Encoder *encoder;

    uint8_t pinA;
    uint8_t pinB;

};



#endif
#endif
