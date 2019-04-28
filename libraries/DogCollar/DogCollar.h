//
//    FILE: DogCollar.h
//  AUTHOR: Flash89y
// VERSION: 0.1
// PURPOSE: replacing Remote of dog collar training device using 
//          3-pin 433 MHz sender module (5V/GND/DATA) with an arduino 
//     URL: https://github.com/flash89y/Arduino/tree/master/libraries/DogCollar
//

#ifndef _DOGCOLLAR_h_
#define _DOGCOLLAR_h_

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

enum class CollarMode
{
    Shock,
    Vibe,
    Beep,
    Blink
};

enum class CollarChannel
{
    CH1,
    CH2,
    BOTH
};

class DogCollar
{   
public:
        DogCollar(int transmitPin, String uniqueKey, int repeatedSending = 3);
        ~DogCollar();
        void sendCollar(CollarChannel ch, CollarMode mode, uint8_t str);
        void sendLastCmd();
        void setMode(CollarMode mode);
        void setChannel(CollarChannel ch);
        void setStrength(uint8_t str);
        bool keepAlive();
private:
        
        CollarChannel lastChannel;
        CollarMode lastMode;
        uint8_t lastStrength;
        int repeatNumber;
        char *magiccode;
        size_t lenMagicCode;
        int pin;
        unsigned long beacons[8];
        
        const int seqModeLength = 8;
        const int seq1Length = 4;
        const int seqChLength = 6;
        const int seqModePreLength = 8;
        const int seq2Length = 34;
        const int seqStrengthLength = 14;
        const int seq3Length = 11;

        const String key;
        
        uint64_t lastMillis;
        uint64_t currentMillis;
        const uint64_t millisDifferenceAllowed;
        
        void fillSequences();
};

#endif // _DOGCOLLAR_h_
