//
//    FILE: DogCollar-KeepAlive.ino
//  AUTHOR: Flash89y
// VERSION: 0.1
// PURPOSE: replacing Remote of dog collar training device using 
//      3-pin 433 MHz sender module (5V/GND/DATA) with an arduino 
//     URL: <none>

#include <DogCollar.h>

#define PIN_TRANSMITTER 7
// find out this key with RFControl library...
String uniqueKeyOfDevice = "2112122112122112211212122121121221";
DogCollar dg(PIN_TRANSMITTER,uniqueKeyOfDevice);

void setup()
{
    Serial.begin(115200);
    Serial.println("DogCollar library demo - keep alive test");
}

void loop()
{
    // sends blink command to prohibit deep sleep of receiver,
    // else it will go to sleep if no movement is detected for 2 minutes
    // IMPORTANT: be sure to call this function at least every 30 seconds
    if(dg.keepAlive())
    {
        Serial.println("keep alive signal was sent!");
    }
    else
    {
        Serial.println("no keep alive signal neccessary...");
    }
    
    // here is the user code section...
    // EXAMPLE: sending a "vibe"-command when entered something on 
    // the serial terminal
    delay(100);
    if(Serial.available())
    {
        // read first byte
        byte incomingByte;
        incomingByte = Serial.read();
        // flush the read buffer
        while(Serial.available()) Serial.read();
        
        if(incomingByte == 'v')
            dg.sendCollar(CollarChannel::CH1, CollarMode::Vibe, 100);
        else if(incomingByte == 's')
            dg.sendCollar(CollarChannel::CH1, CollarMode::Shock, 100);
        else if(incomingByte == 'b')
            dg.sendCollar(CollarChannel::CH1, CollarMode::Beep, 100);
        else
            Serial.println("command unknown: send {v,s,b} -> v=vibe, s=shock, b=beep");
    }
}
