//
//    FILE: DogCollar.h
//  AUTHOR: Flash89y
// VERSION: 0.1
// PURPOSE: replacing Remote of dog collar training device
//     URL: https://github.com/flash89y/Arduino/tree/master/libraries/DogCollar
//REQUIRES: RFControl library (https://github.com/pimatic/RFControl)
//
// HISTORY:
// 0.1 initial release (02.07.2018 - US date format)
//

#include "DogCollar.h"
#include <RFControl.h>

DogCollar::DogCollar(int transmitPin, String uniqueKey, int repeatedSending) : millisDifferenceAllowed(60000), key(uniqueKey)
{
    lastMillis = 0;
    repeatNumber = repeatedSending;
    pin = transmitPin;
    lenMagicCode = 85;
    magiccode = new char[lenMagicCode];
    
    setChannel(CollarChannel::CH1);
    setMode(CollarMode::Blink);
    setStrength(100);
    
    beacons[0] = 1464;
    beacons[1] = 752;
    beacons[2] = 244;
    beacons[3] = 10560;
    beacons[4] = 0;
    beacons[5] = 0;
    beacons[6] = 0;
    beacons[7] = 0;

    fillSequences();
}

DogCollar::~DogCollar()
{
    delete magiccode;
}

void DogCollar::sendCollar(CollarChannel ch, CollarMode mode, uint8_t str)
{
    setMode(mode);
    setStrength(str);
    magiccode[lenMagicCode-1]='\0';

    Serial.println(magiccode);

    if(ch == CollarChannel::BOTH)
    {
        setChannel(CollarChannel::CH1);
        RFControl::sendByCompressedTimings(pin, beacons, magiccode, repeatNumber);
        delay(20);
        setChannel(CollarChannel::CH2);
        RFControl::sendByCompressedTimings(pin, beacons, magiccode, repeatNumber);
    }   
    else
    {
        RFControl::sendByCompressedTimings(pin, beacons, magiccode, repeatNumber);
    }
}

void DogCollar::sendLastCmd()
{
    RFControl::sendByCompressedTimings(pin, beacons, magiccode, repeatNumber);
}

void DogCollar::setMode(CollarMode mode)
{
    lastMode = mode;
    // write in mode
    if(mode == CollarMode::Shock)
    {
        magiccode[10] = '2';
        magiccode[11] = '1';
        magiccode[12] = '2';
        magiccode[13] = '1';
        magiccode[14] = '2';
        magiccode[15] = '1';
        magiccode[16] = '1';
        magiccode[17] = '2';

        magiccode[66] = '2';
        magiccode[67] = '1';
        magiccode[68] = '1';
        magiccode[69] = '2';
        magiccode[70] = '1';
        magiccode[71] = '2';
        magiccode[72] = '1';
        magiccode[73] = '2';
    }
    else if(mode == CollarMode::Vibe)
    {
        magiccode[10] = '2';
        magiccode[11] = '1';
        magiccode[12] = '2';
        magiccode[13] = '1';
        magiccode[14] = '1';
        magiccode[15] = '2';
        magiccode[16] = '2';
        magiccode[17] = '1';

        magiccode[66] = '1';
        magiccode[67] = '2';
        magiccode[68] = '2';
        magiccode[69] = '1';
        magiccode[70] = '1';
        magiccode[71] = '2';
        magiccode[72] = '1';
        magiccode[73] = '2';
    }
    else if(mode == CollarMode::Beep)
    {
        magiccode[10] = '2';
        magiccode[11] = '1';
        magiccode[12] = '1';
        magiccode[13] = '2';
        magiccode[14] = '2';
        magiccode[15] = '1';
        magiccode[16] = '2';
        magiccode[17] = '1';
        
        magiccode[66] = '1';
        magiccode[67] = '2';
        magiccode[68] = '1';
        magiccode[69] = '2';
        magiccode[70] = '2';
        magiccode[71] = '1';
        magiccode[72] = '1';
        magiccode[73] = '2';
    }
    else //if(mode == CollarMode::Blink)
    {
        magiccode[10] = '1';
        magiccode[11] = '2';
        magiccode[12] = '2';
        magiccode[13] = '1';
        magiccode[14] = '2';
        magiccode[15] = '1';
        magiccode[16] = '2';
        magiccode[17] = '1';

        magiccode[66] = '1';
        magiccode[67] = '2';
        magiccode[68] = '1';
        magiccode[69] = '2';
        magiccode[70] = '1';
        magiccode[71] = '2';
        magiccode[72] = '2';
        magiccode[73] = '1';
    }
}
    
void DogCollar::setChannel(CollarChannel ch)
{
    lastChannel = ch;
    // write in channel
    if(ch == CollarChannel::CH1)
    {
        magiccode[4] = '2';
        magiccode[5] = '1';
        magiccode[6] = '2';
        magiccode[7] = '1';
        magiccode[8] = '2';
        magiccode[9] = '1';

        magiccode[74] = '1';
        magiccode[75] = '2';
        magiccode[76] = '1';
        magiccode[77] = '2';
        magiccode[78] = '1';
        magiccode[79] = '2';
    }
    else // CH2
    {
        magiccode[4] = '1';
        magiccode[5] = '2';
        magiccode[6] = '1';
        magiccode[7] = '2';
        magiccode[8] = '1';
        magiccode[9] = '2';

        magiccode[74] = '2';
        magiccode[75] = '1';
        magiccode[76] = '2';
        magiccode[77] = '1';
        magiccode[78] = '2';
        magiccode[79] = '1';
    }
}

void DogCollar::setStrength(uint8_t str)
{
    // write in stength { 0 ... 100}
  char bitsZero[] = {'2','1'};
  char bitsOnes[] = {'1','2'};
  if (str >100) str = 100;
  
  lastStrength = str;

  if(str & 0b1000000)
  {
    magiccode[52] = bitsOnes[0];
    magiccode[53] = bitsOnes[1];
  }
  else
  {
    magiccode[52] = bitsZero[0];
    magiccode[53] = bitsZero[1];
  }
  if(str & 0b0100000)
  {
    magiccode[54] = bitsOnes[0];
    magiccode[55] = bitsOnes[1];
  }
  else
  {
    magiccode[54] = bitsZero[0];
    magiccode[55] = bitsZero[1];
  }
  if(str & 0b0010000)
  {
    magiccode[56] = bitsOnes[0];
    magiccode[57] = bitsOnes[1];
  }
  else
  {
    magiccode[56] = bitsZero[0];
    magiccode[57] = bitsZero[1];
  }
  if(str & 0b0001000)
  {
    magiccode[58] = bitsOnes[0];
    magiccode[59] = bitsOnes[1];
  }
  else
  {
    magiccode[58] = bitsZero[0];
    magiccode[59] = bitsZero[1];
  }
  if(str & 0b0000100)
  {
    magiccode[60] = bitsOnes[0];
    magiccode[61] = bitsOnes[1];
  }
  else
  {
    magiccode[60] = bitsZero[0];
    magiccode[61] = bitsZero[1];
  }
  if(str & 0b0000010)
  {
    magiccode[62] = bitsOnes[0];
    magiccode[63] = bitsOnes[1];
  }
  else
  {
    magiccode[62] = bitsZero[0];
    magiccode[63] = bitsZero[1];
  }
  if(str & 0b0000001)
  {
    magiccode[64] = bitsOnes[0];
    magiccode[65] = bitsOnes[1];
  }
  else
  {
    magiccode[64] = bitsZero[0];
    magiccode[65] = bitsZero[1];
  }
}

void DogCollar::fillSequences(){

    String sequence1 = String(F("0112"));
    String sequence2 = key;
    String sequence3 = String(F("1212122123V"));
    const int seq2Offset = 18;
    const int seq3Offset = 74;

    // write in default timings
    for(int i=0; i<seq1Length; i++)
    {
    magiccode[i] = sequence1[i];
    }
    for(int i=0; i<seq2Length; i++)
    {
    magiccode[i+seq2Offset] = sequence2[i];
    }
    for(int i=0; i<seq3Length; i++)
    {
    magiccode[i+seq3Offset] = sequence3[i];
    }
}

bool DogCollar::keepAlive()
{
    currentMillis = millis();
    if(currentMillis > (lastMillis + millisDifferenceAllowed))
    {
        // let the led blink on both channels to prohibit deep sleep
        sendCollar(CollarChannel::CH1, CollarMode::Blink, 100);
        delay(50);
        sendCollar(CollarChannel::CH2, CollarMode::Blink, 100);
        lastMillis = currentMillis;
        return true;
    }
    return false;
}
