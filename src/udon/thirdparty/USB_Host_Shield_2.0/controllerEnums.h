/* Copyright (C) 2013 Kristian Lauszus, TKJ Electronics. All rights reserved.

 This software may be distributed and modified under the terms of the GNU
 General Public License version 2 (GPL2) as published by the Free Software
 Foundation and appearing in the file GPL2.TXT included in the packaging of
 this file. Please note that GPL2 Section 2[b] requires that all works based
 on this software must also be made publicly available under the terms of
 the GPL2 ("Copyleft").

 Contact information
 -------------------

 Kristian Lauszus, TKJ Electronics
 Web      :  http://www.tkjelectronics.com
 e-mail   :  kristianl@tkjelectronics.com
 */

#ifndef _controllerenums_h
#define _controllerenums_h

#if defined(ESP32)
#undef PS
#endif

/**
 * This header file is used to store different enums for the controllers,
 * This is necessary so all the different libraries can be used at once.
 */

/** Enum used to turn on the LEDs on the different controllers. */
enum LEDEnum {
        OFF = 0,
#ifndef RBL_NRF51822
        LED1 = 1,
        LED2 = 2,
        LED3 = 3,
        LED4 = 4,
#endif
        LED5 = 5,
        LED6 = 6,
        LED7 = 7,
        LED8 = 8,
        LED9 = 9,
        LED10 = 10,
        /** Used to blink all LEDs on the Xbox controller */
        ALL = 5,
};

/** Used to set the colors of the Move and PS4 controller. */
enum class ColorsEnum {
        /** r = 255, g = 0, b = 0 */
        Red = 0xFF0000,
        /** r = 0, g = 255, b = 0 */
        Green = 0xFF00,
        /** r = 0, g = 0, b = 255 */
        Blue = 0xFF,

        /** r = 255, g = 235, b = 4 */
        Yellow = 0xFFEB04,
        /** r = 0, g = 255, b = 255 */
        Lightblue = 0xFFFF,
        /** r = 255, g = 0, b = 255 */
        Purple = 0xFF00FF,
        Purble = 0xFF00FF,

        /** r = 255, g = 255, b = 255 */
        White = 0xFFFFFF,
        /** r = 0, g = 0, b = 0 */
        Off = 0x00,
};

enum class RumbleEnum {
        RumbleHigh = 0x10,
        RumbleLow = 0x20,
};

/** This enum is used to read all the different buttons on the different controllers */
enum class ButtonEnum {
        /**@{*/
        /** Directional Pad Buttons - available on most controllers */
        UP = 0,
        RIGHT = 1,
        DOWN = 2,
        LEFT = 3,
        /**@}*/

        /**@{*/
        /** Playstation buttons */
        TRIANGLE,
        CIRCLE,
        CROSS,
        SQUARE,

        SELECT,
        START,

        L3,
        R3,

        L1,
        R1,
        L2,
        R2,

        PS,
        /**@}*/

        /**@{*/
        /** PS3 Move Controller */
        MOVE, // Covers 12 bits - we only need to read the top 8
        T, // Covers 12 bits - we only need to read the top 8
        /**@}*/

        /**@{*/
        /** PS Buzz controllers */
        RED,
        YELLOW,
        GREEN,
        ORANGE,
        BLUE,
        /**@}*/

        /**@{*/
        /** PS4 buttons - SHARE and OPTIONS are present instead of SELECT and START */
        SHARE,
        OPTIONS,
        TOUCHPAD,
        /**@}*/

        /**@{*/
        /** PS5 buttons */
        CREATE,
        MICROPHONE,
        /**@}*/

        /**@{*/
        /** Xbox buttons */
        A,
        B,
        X,
        Y,

        BACK,
        // START,  // listed under Playstation buttons

        LB,
        RB,
        LT,
        RT,

        XBOX,
        SYNC,

        BLACK, // Available on the original Xbox controller
        WHITE, // Available on the original Xbox controller
        /**@}*/

        /**@{*/
        /** Xbox One S buttons */
        VIEW,
        MENU,
        /**@}*/

        /**@{*/
        /** Wii buttons */
        PLUS,
        TWO,
        ONE,
        MINUS,
        HOME,
        Z,
        C,
        // B,  // listed under Xbox buttons
        // A,  // listed under Xbox buttons
        /**@}*/

        /**@{*/
        /** Wii U Pro Controller */
        L,
        R,
        ZL,
        ZR,
        /**@}*/

        /**@{*/
        /** Switch Pro Controller */
        CAPTURE,
        /**@}*/
};

inline constexpr int8_t ButtonIndex(ButtonEnum key) {
    // using a chained ternary in place of a switch for constexpr on older compilers
    return
        (key == ButtonEnum::UP || key == ButtonEnum::RED) ? 0 :
        (key == ButtonEnum::RIGHT || key == ButtonEnum::YELLOW) ? 1 :
        (key == ButtonEnum::DOWN || key == ButtonEnum::GREEN) ? 2 :
        (key == ButtonEnum::LEFT || key == ButtonEnum::ORANGE) ? 3 :
        (key == ButtonEnum::SELECT || key == ButtonEnum::SHARE || key == ButtonEnum::BACK || key == ButtonEnum::VIEW || key == ButtonEnum::BLUE || key == ButtonEnum::CREATE || key == ButtonEnum::CAPTURE) ? 4 :
        (key == ButtonEnum::START || key == ButtonEnum::OPTIONS || key == ButtonEnum::MENU || key == ButtonEnum::PLUS) ? 5 :
        (key == ButtonEnum::L3 || key == ButtonEnum::TWO) ? 6 :
        (key == ButtonEnum::R3 || key == ButtonEnum::ONE) ? 7 :
        (key == ButtonEnum::L2 || key == ButtonEnum::LT || key == ButtonEnum::MINUS || key == ButtonEnum::BLACK) ? 8 :
        (key == ButtonEnum::R2 || key == ButtonEnum::RT || key == ButtonEnum::HOME || key == ButtonEnum::WHITE) ? 9 :
        (key == ButtonEnum::L1 || key == ButtonEnum::LB || key == ButtonEnum::Z) ? 10 :
        (key == ButtonEnum::R1 || key == ButtonEnum::RB || key == ButtonEnum::C) ? 11 :
        (key == ButtonEnum::TRIANGLE || key == ButtonEnum::B) ? 12 :
        (key == ButtonEnum::CIRCLE || key == ButtonEnum::A) ? 13 :
        (key == ButtonEnum::CROSS || key == ButtonEnum::X) ? 14 :
        (key == ButtonEnum::SQUARE || key == ButtonEnum::Y) ? 15 :
        (key == ButtonEnum::L || key == ButtonEnum::PS || key == ButtonEnum::XBOX) ? 16 :
        (key == ButtonEnum::R || key == ButtonEnum::MOVE || key == ButtonEnum::TOUCHPAD || key == ButtonEnum::SYNC) ? 17 :
        (key == ButtonEnum::ZL || key == ButtonEnum::T || key == ButtonEnum::MICROPHONE) ? 18 :
        (key == ButtonEnum::ZR) ? 19 :
        -1;  // not a match
}

/** Joysticks on the PS3 and Xbox controllers. */
enum class AnalogHatEnum {
        /** Left joystick x-axis */
        LeftHatX = 0,
        /** Left joystick y-axis */
        LeftHatY = 1,
        /** Right joystick x-axis */
        RightHatX = 2,
        /** Right joystick y-axis */
        RightHatY = 3,
};

/**
 * Sensors inside the Sixaxis Dualshock 3, Move controller and PS4 controller.
 * <B>Note:</B> that the location is shifted 9 when it's connected via USB on the PS3 controller.
 */
enum class SensorEnum {
        /** Accelerometer values */
        aX = 50, aY = 52, aZ = 54,
        /** Gyro z-axis */
        gZ = 56,
        gX, gY, // These are not available on the PS3 controller

        /** Accelerometer x-axis */
        aXmove = 28,
        /** Accelerometer z-axis */
        aZmove = 30,
        /** Accelerometer y-axis */
        aYmove = 32,

        /** Gyro x-axis */
        gXmove = 40,
        /** Gyro z-axis */
        gZmove = 42,
        /** Gyro y-axis */
        gYmove = 44,

        /** Temperature sensor */
        tempMove = 46,

        /** Magnetometer x-axis */
        mXmove = 47,
        /** Magnetometer z-axis */
        mZmove = 49,
        /** Magnetometer y-axis */
        mYmove = 50,
};

/** Used to get the angle calculated using the PS3 controller and PS4 controller. */
enum class AngleEnum {
        Pitch = 0x01,
        Roll = 0x02,
};

#endif
