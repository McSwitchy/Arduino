//
//    FILE: DogCollar2.cpp
//  AUTHOR: Flash89y
// VERSION: 0.1
// PURPOSE: replacing Remote of dog collar training device using 
//          3-pin 433 MHz sender module (5V/GND/DATA) with an arduino
//          this is the second version (DogCollar2) with advantages:
//     URL: https://github.com/flash89y/Arduino/tree/master/libraries/DogCollar2
//  THANKS: thank you smouldery for making a nice memory friendly
//          implementation I could combine with previous "DogCollar"
//          link: https://github.com/smouldery/shock-collar-control/

#include "DogCollar2.h"

DogCollar::DogCollar(int transmitPin, String uniqueKey, int repeatedSending) : millisDifferenceAllowed(60000),
    key("01101101011100110"), delayStart(1464), delayLong(752), delayShort(244), delayBetween(10560)
{
    lastMillis = 0;
    repeatNumber = repeatedSending;
    pin = transmitPin;
    pinMode(pin,OUTPUT);
    keyIsOK = false;

    Serial.begin(115200);
    // convert key read out with "GetUniqueKey.ino" into the syntax
    // of magiccode used in this library ("21" -> "0" and "12" -> "1")
    if(uniqueKey.length() == lenKey)
    {
        for(int i=0;i<lenKey/2;i++)
        {
            if(uniqueKey.charAt(2*i) == '1' && uniqueKey.charAt(2*i+1) == '2')
            {
                key.setCharAt(i,'1');
            }
            else if(uniqueKey.charAt(2*i) == '2' && uniqueKey.charAt(2*i+1) == '1')
            {
                key.setCharAt(i,'0');
            }
        }
        // key could be right because length is set correctly
        // if it is correct for the dog collars can't be garanteed here
        // but should be if got with the "GetUniqueKey.ino" sketch and
        // entered in instatiation of class DogCollar in your sketch
        keyIsOK = true;
    }
}

DogCollar::~DogCollar()
{

}

void DogCollar::transmitCode()
{
    digitalWrite(pin, HIGH);
    delayMicroseconds(delayStart);
    digitalWrite(pin, LOW);
    delayMicroseconds(delayLong);

    for (int n = 0; n < magiccode.length() ; n++)
    {
        if (magiccode.charAt(n) == '1') // transmit a one
        {
            digitalWrite(pin, HIGH);
            delayMicroseconds(delayLong);
            digitalWrite(pin, LOW);
            delayMicroseconds(delayShort);
        }
        else // transmit a zero
        {
            digitalWrite(pin, HIGH);
            delayMicroseconds(delayShort);
            digitalWrite(pin, LOW);
            delayMicroseconds(delayLong);
        }
    }
}

void DogCollar::sendCollar(CollarChannel ch, CollarMode mode, uint8_t str)
{
    if(keyIsOK)
    {
        // Serial.println(String(F("key to use: ")) + key);
    }
    else
    {
        Serial.println("invalid key entered, check with RFControl library!");
        return;
    }
    
    if(ch == CollarChannel::BOTH)
    {
        for(int i=0; i<repeatNumber; i++)
        {
            fillSequences(CollarChannel::CH1,mode,str);
            delayMicroseconds(delayBetween);
            transmitCode();
            
            fillSequences(CollarChannel::CH2,mode,str);
            delayMicroseconds(delayBetween);
            transmitCode();
        }
    }   
    else
    {
        fillSequences(ch,mode,str);
        for(int i=0; i<repeatNumber; i++)
        {
            transmitCode();
            delayMicroseconds(delayBetween);
        }
    }
}

void DogCollar::fillSequences(CollarChannel ch, CollarMode mode, uint8_t str)
{
    // which channel
    if(ch == CollarChannel::CH2)
    {
        channelPartOne = "111";
        channelPartTwo = "000";
    }
    else // (ch == CollarChannel::CH1) or (ch == CollarChannel::BOTH)
    {
        channelPartOne = "000";
        channelPartTwo = "111";
    }
    
    // which mode
    if(mode == CollarMode::Beep)
    {
        modePartOne = "0100";
        modePartTwo = "1101";
    }
    else if(mode == CollarMode::Blink)
    {
        modePartOne = "1000";
        modePartTwo = "1110";
    }
    else if(mode == CollarMode::Shock)
    {
        modePartOne = "0001";
        modePartTwo = "0111";
    }
    else // (mode == CollarMode::Vibe)
    {
        modePartOne = "0010";
        modePartTwo = "1011";
    }

    // which strength
    strength = "";
    str = constrain(str,1,100);
    int zeros = String(str, BIN).length();
    for (int i = 0; i < 7 - zeros; i++)
    {
        strength = strength + "0";
    }
    strength = strength + String(str, BIN);

    magiccode = "1" + channelPartOne + modePartOne + key + strength + modePartTwo + channelPartTwo + "00";

    // massive debug outputs ahead
    //~ Serial.println(String("channelPartOne = ") + channelPartOne);
    //~ Serial.println(String("modePartOne = ") + modePartOne);
    //~ Serial.println(String("key = ") + key);
    //~ Serial.println(String("strength = ") + strength);
    //~ Serial.println(String("modePartTwo = ") + modePartTwo);
    //~ Serial.println(String("channelPartTwo = ") + channelPartTwo);
    //~ Serial.println(String("magiccode = ") + magiccode);
    //~ Serial.println(String("magiccode.length() = ") + magiccode.length());
}

bool DogCollar::keepAlive()
{
    currentMillis = millis();
    if(currentMillis > (lastMillis + millisDifferenceAllowed))
    {
        lastMillis = millis();
        // let the led blink on both channels to prohibit deep sleep
        sendCollar(CollarChannel::BOTH, CollarMode::Blink, 100);
        return true;
    }
    return false;
}
