//
//    FILE: DogCollar-SimpleTest.ino
//  AUTHOR: Flash89y
// VERSION: 0.1
// PURPOSE: replacing Remote of dog collar training device using 
//      3-pin 433 MHz sender module (5V/GND/DATA) with an arduino 
//     URL: https://github.com/flash89y/Arduino/tree/master/libraries/DogCollar2
//
// Released to the public domain
//

#include <DogCollar2.h>

#define PIN_TRANSMITTER 7
// REPLACE this key with result from sketch "examples/GetUniqueKey.ino"
String uniqueKeyOfDevice = "2112122112122112211212122121121221";
DogCollar dg(PIN_TRANSMITTER,uniqueKeyOfDevice);

void setup()
{
    Serial.begin(115200);
    Serial.println("DogCollar library demo - Simple Test");
}

void loop()
{
    // trying different modes of Channel 1-coded device in a loop...
    Serial.println("CollarChannel::CH1, CollarMode::Beep, Strength:100");
    delay(1000);
    dg.sendCollar(CollarChannel::CH1, CollarMode::Beep, 100);

    delay(5000);

    Serial.println("CollarChannel::BOTH, CollarMode::Vibe, Strength:100");
    delay(1000);
    dg.sendCollar(CollarChannel::BOTH, CollarMode::Vibe, 100);

    delay(5000);

    Serial.println("CollarChannel::CH1, CollarMode::Shock, Strength:20");
    delay(1000);
    dg.sendCollar(CollarChannel::CH1, CollarMode::Shock, 20);

    delay(5000);

    Serial.println("CollarChannel::CH2, CollarMode::Blink, Strength:100");
    delay(1000);
    dg.sendCollar(CollarChannel::CH2, CollarMode::Blink, 100);

    delay(5000);
}
