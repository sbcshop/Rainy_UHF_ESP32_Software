
 /** Demo Code to Perform Tag Memory Write Operation   
 * Interfacing Details:
 * IO44/U0RXD - UHF_TXD 
 * IO43/U0TXD - UHF_RXD 
 * IO40       - UHF_EN 
 * IO48       - Buzzer
 * IO3        - User LED


  Memory Bank : only writable memory can be used for memory write operation
    1 - EPC  --> Read/Write
    3 - USER --> Read/Write

  2Byte = 1 Word for Rainy UHF Module
  For Ease Write operation we have set 8 Word size (16 Byte) data size in library, So follow below configuration:

  User Memory => 
  - For Data write in user memory you can create random data of length 8 Word
  example,
  random data => 00000000000000000000000000000012
  memory_data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12}

  EPC Memory =>
  - Use Tag Memory Read [Memory Bank - 1] operation first to get EPC Frame for modification
  - For example, Response for Memory Bank 1 (Response May vary for Different Tags):
    Response: aa 01 39 00 1f 0e 34 00 00 00 00 00 00 00 20 23 01 03 09 12 84 69 34 00 00 00 00 00 00 00 20 23 01 03 09 12 80 dd 
    For Write Operation, frame required for changing EPC of Tag: 1f 0e 34 00 00 00 00 00 00 00 20 23 01 03 09 12 
    => [CheckSum + PC][OLD EPC] : [1f 0e 34 00] [00 00 00 00 00 00 20 23 01 03 09 12] 
    Modified With new EPC to pass as parameter: [1f 0e 34 00] [11 22 33 44 55 66 77 88 99 11 22 33]
    epc_memory_data = {0x1f, 0x0e, 0x34, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0x11, 0x22, 0x33}   
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

bool flag = false;

void loop(){
  delay(1000);
  //std::vector<byte> memory_data = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12};  // change with your data 
  std::vector<byte> epc_memory_data = {0x9B, 0xD9, 0x34, 0x00, 0xC7, 0x05, 0x9A, 0xFB, 0x0A, 0x99, 0xBD, 0x25, 0xEF, 0x78, 0x53, 0xD6}; // change with your EPC value
  std::vector<byte> epc_select = {0xC7, 0x05, 0x9A, 0xFB, 0x0A, 0x99, 0xBD, 0x25, 0xEF, 0x78, 0x53, 0xD6}; // change with your EPC value
  
  int  memoryBank = 1;  // 1 - EPC , 3 - USER, select suitable option
  
  uhf.selectTag(epc_select);
  
  while (flag == false){
    Serial.println("working");
   // uncomment for suitable memory write operation
   //flag = uhf.writeTagMemory(memory_data,memoryBank);
   flag = uhf.writeTagMemory(epc_memory_data,memoryBank);   
  }
  
  delay(500);
}
