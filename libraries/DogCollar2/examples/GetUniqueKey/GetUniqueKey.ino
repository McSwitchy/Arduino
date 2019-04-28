//
//    FILE: GetUniqueKey.ino
//  AUTHOR: Flash89y
// VERSION: 0.1
// PURPOSE: getting to know unique key (34 Bytes) of dog collar sender
//          which is needed for sender program to send the same key.
//          for this sketch you need a
//          4-pin 433 MHz receiver module (5V/DATA/DATA/GND) with an arduino 
//     URL: https://github.com/flash89y/Arduino/tree/master/libraries/DogCollar
//
// Released to the public domain
//

#include <RFControl.h>

// choose pin 2 OR pin 3 if you have Arduino Uno, Nano, Mini or other 328-based
// see: https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
#define RECEIVE_PIN 3       // interrupt pin (pin 2 = INT0, pin 3 = INT1) of arduino connected 
                            // to any of both pins "DATA" on 433MHz receiver

// here is the key...
const int keyOffset = 18;
const int keyLen = 34;

// buffers to get the code stored into
unsigned int *timingsBuffer;
unsigned int timings_size;
unsigned int pulse_length_divider;
unsigned long timing;
unsigned int buckets[8];
char *timingsCompressed;

void setup() {
  Serial.begin(115200);
  Serial.println("startReceiving()");
  // convert pin to interrupt (pin 2 = INT0, pin 3 = INT1)
  RFControl::startReceiving(RECEIVE_PIN-2); // sets as input and attaches interrupt
}

void loop() {
  if(RFControl::hasData()) {
    pulse_length_divider = RFControl::getPulseLengthDivider();
    RFControl::getRaw(&timingsBuffer, &timings_size);

//    // uncompressed
//    for(int i=0; i < timings_size; i++) {
//      timing = timingsBuffer[i] * pulse_length_divider;
//      Serial.print(timing);
//      Serial.write('\t');
//      if((i+1)%16 == 0) {
//        Serial.write('\n');
//      }
//    }
    
    // compressed
    RFControl::compressTimings(buckets, timingsBuffer, timings_size);
    timingsCompressed = new char[timings_size];
//    Serial.print("buckets: ");
    for(int i=0; i < 8; i++) {
      unsigned long bucket = buckets[i] * pulse_length_divider;
//      Serial.print(bucket);
//      Serial.print(", ");
//      Serial.write(' ');
    }
    //Serial.print("\ntimings: ");
    for(int i=0; i < timings_size; i++) {
      timingsCompressed[i] = '0' + static_cast<char>(timingsBuffer[i]);
      //Serial.write('0' + timings[i]);
    }

    // get final outputs
    Serial.println("timings_size = " + String(timings_size));
    Serial.println("timingsCompressed = " + String(timingsCompressed));
    if(String(timingsCompressed).length() > keyOffset+keyLen)
    {
      Serial.println("uniqueKey = " + String(timingsCompressed).substring(keyOffset,keyOffset+keyLen));
    }
    else
    {
      Serial.println("received code is too short to contain full message, unknown device!");
    }
    // reset timings to maybe get next message
    delete timingsCompressed;
    
    //Serial.print("\n\n");
    RFControl::continueReceiving();
  }
}

