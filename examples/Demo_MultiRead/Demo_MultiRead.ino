/*
 * Demo code to read UHF Tags - Multiple Polling operation
 * 
 * Interfacing Details:
 * IO44/U0RXD - UHF_TXD 
 * IO43/U0TXD - UHF_RXD 
 * IO40       - UHF_EN 
 * IO48       - Buzzer
 * IO3        - User LED
 */
#include "RAINY_UHF.h"  // include Rainy UHF library

#define UHF_ENABLE 40 // Rainy UHF Enable pin

RAINY_UHF uhf;   // create Rainy UHF module instance 

const int buzzerPin = 48;    // buzzer at GPIO2
const int userLED = 3; // programmable led for status

void beep(int note, int duration){  // need note frequency and time duration (in millis)
  digitalWrite(userLED, HIGH);
  tone(buzzerPin, note, duration);
  delay(duration);
  digitalWrite(userLED, LOW);
}

void setup() {
    pinMode(userLED, OUTPUT);  // set User led pin as OUTPUT
    pinMode(UHF_ENABLE, OUTPUT);     // Initialize pin as OUTPUT
    digitalWrite(UHF_ENABLE, HIGH);  // HIGH - Enable UHF module, LOW -Disable Module
    
    Serial.begin(115200); // Initialize SerialTerimal-ESP32 communication 
    uhf.begin();          // Initialize UHF-ESP32 UART communication 

    Serial.println("Ready!");
    
    uhf.stopRead();    // Need to run only if previously multi read command execute
}


void loop(){
  std::vector<byte> multiread;  // variable to hold single polling response from Rainy UHF
  multiread = uhf.multipleReadTags();   // perform Multi polling operation

  Serial.print("Length: ");
  Serial.println(multiread.size());
  Serial.print("Response: \n");
  for (byte& hexData : multiread){
    Serial.print(hexData,HEX);       // print reponse data
    Serial.write(32);            // add space between each hex byte value
  }
  
  Serial.println();
  delay(20);
}
