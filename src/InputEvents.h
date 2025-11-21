/** 
 * @file
 * @brief Contains InputEventType enums and some defines.
 *
 * GPLv2 Licence https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
 * 
 * Copyright (c) 2024 Philip Fletcher <philip.fletcher@stutchbury.com>
 * 
 */

#ifndef INPUT_EVENTS_H
#define INPUT_EVENTS_H

#include <Arduino.h>

#ifndef FUNCTIONAL_SUPPORTED
    #if defined(__has_include) // Check if __has_include is supported
        #if __has_include(<functional>)
            #define FUNCTIONAL_SUPPORTED
        #endif
    #endif
#endif

/// \cond DO_NOT_DOCUMENT
/*
 * Some platforms stupidly use #define for DISABLED (ahem, espressif), so replace it with a constexpr variable
 * Many thanks to @kfoltman https://github.com/kfoltman for the workaround.
 */
#ifdef DISABLED
constexpr auto ACTUAL_DISABLED_VALUE=DISABLED;
#undef DISABLED
constexpr auto DISABLED=ACTUAL_DISABLED_VALUE;
#endif
/// \endcond


/**
 * @brief The size of the InputEventType enum
 * 
 */
//constexpr size_t NUM_EVENT_TYPE_ENUMS = 20;

/**
 * @brief A list of all events that can be fired by InputEvents classes.
 */
enum class InputEventType : uint8_t {
    NONE,               ///<  0 Used for initialising/testing
    ENABLED,            ///<  1 Fired by all imputs
    DISABLED,           ///<  2 Fired by all imputs
    IDLE,               ///<  3 Fired by all imputs
    PRESSED,            ///<  4 Fired by EventButton, EventEncoderButton and EventTouchScreen
    RELEASED,           ///<  5 Fired by EventButton, EventEncoderButton and EventTouchScreen
    CLICKED,            ///<  6 Fired by EventButton, EventEncoderButton and EventTouchScreen
    DOUBLE_CLICKED,     ///<  7 Fired by EventButton, EventEncoderButton and EventTouchScreen
    MULTI_CLICKED,      ///<  8 Fired by EventButton, EventEncoderButton and EventTouchScreen
    LONG_CLICKED,       ///<  9 Fired by EventButton, EventEncoderButton and EventTouchScreen
    LONG_PRESS,         ///< 10 Fired by EventButton, EventEncoderButton and EventTouchScreen
    CHANGED,            ///< 11 Fired by EventEncoder and EventAnalog
    CHANGED_PRESSED,    ///< 12 Fired by EventEncoderButton
    CHANGED_RELEASED,   ///< 13 Fired by EventEncoderButton
    CHANGED_X,          ///< 14 Fired Bby EventJoystick
    CHANGED_Y,          ///< 15 Fired Bby EventJoystick
    ON,                 ///< 16 Fired by EventSwitch
    OFF,                ///< 17 Fired by EventSwitch
    DRAGGED,            ///< 18 Fired by EventTouchScreen 
    DRAGGED_RELEASED,   ///< 19 Fired by EventTouchScreen
    COUNT               ///< Not used - replaces NUM_EVENT_TYPE_ENUMS 
};

#endif