/*
 * Demo Code to Perform Tag Memory Read   
 * Interfacing Details:
 * IO44/U0RXD - UHF_TXD 
 * IO43/U0TXD - UHF_RXD 
 * IO40       - UHF_EN 
 * IO48       - Buzzer
 * IO3        - User LED
 * 
 * Memory Bank 
    1 - EPC  --> Read/Write
    2 - TID  --> Only readable
    3 - USER --> Read/Write
   
   Example Below Response for Memory Bank 1 (Response May vary for Different Tags):
   Response: aa 01 39 00 1f 0e 34 00 00 00 00 00 00 00 20 23 01 03 09 12 84 69 34 00 00 00 00 00 00 00 20 23 01 03 09 12 80 dd 
   For Write Operation, frame required for changing EPC of Tag: 1f 0e 34 00 00 00 00 00 00 00 20 23 01 03 09 12 
   => [CheckSum + PC][OLD EPC] : [1f 0e 34 00] [00 00 00 00 00 00 20 23 01 03 09 12] 
   Modified With new EPC to pass as parameter: [1f 0e 34 00] [10 00 00 00 00 00 20 23 01 03 09 44]  
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
}


void loop(){
  std::vector<byte> memoryData;     // Variable to Tag memory data
 
  byte memoryBank = 1;                          // 1 - EPC , 2 - TID , 3 - USER 
  memoryData = uhf.readTagMemory(memoryBank);   // Perform memory read operation

  // Print complete Response
  Serial.print("Response: ");    
  for (byte& hexData : memoryData){
    Serial.print(hexData,HEX);       // print response data
    Serial.write(32);            // add space between each hex byte value
  }

  Serial.println();
 
  String infoFrame = "";
  if (memoryData.size() > 10) { // Ensure we have a valid response
    beep(1245, 500);
    int startIndex, endIndex;
    
    if (memoryBank == 1){
        startIndex = 4;
        endIndex = 20;
        Serial.print("EPC Frame [Checksum + PC + EPC]: ");
    }
      
    if (memoryBank == 2){
        startIndex = 20;
        endIndex = 32;
        Serial.print("TID Frame: ");
    }

    if (memoryBank == 3){
        startIndex = 20;
        endIndex = 36;
        Serial.print("Data Frame: ");
    }
     
    for (int i = startIndex; i < endIndex; i++) { 
        char hexStr[3];  // Buffer for hex conversion
        sprintf(hexStr, "%02X", memoryData[i]); // Convert byte to hex string
        infoFrame += String(hexStr) + " ";
    }
     
    Serial.println(infoFrame); 
    delay(50);
  }
  
  delay(500);
}
