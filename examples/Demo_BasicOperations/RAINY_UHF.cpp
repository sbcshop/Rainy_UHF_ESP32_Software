#include "RAINY_UHF.h"

byte RAINY_UHF::serialBuffer[1024]; 

/**
  * @brief Constructor to initialize RAINY UHF module
  * @param rxPin: The RX pin for UART communication
  * @param txPin: The TX pin for UART communication
  * @param enablePin: The pin used to enable/disable the RAINY UHF module
  */
RAINY_UHF::RAINY_UHF() {
    this->uhfSerial = &Serial2;  // Using Serial2 for ESP32
}

/**
  * @brief Initializes the UHF module with a specified baud rate
  * @param baudRate: The UART baud rate (default is 115200)
  * @param rxPin: The ESP32 UART Rx pin connected RAINY UHF TXD pin 
  * @param txPin: The ESP32 UART TX pin connected RAINY UHF RXD pin 
  * 
  */
void RAINY_UHF::begin(long baudrate, int rxPin, int txPin) {
    uhfSerial->begin(baudrate, SERIAL_8N1, rxPin, txPin);
    while (!uhfSerial);  // Wait until the Serial is ready (useful for native USB boards)
    Serial.println(F("UHF Module Initialized!"));
}


/**
  * @brief Calculates the checksum for a given data packet
  * @param dat: Pointer to the data array
  * @param len: Length of the data array
  * @return The calculated checksum byte
  */
byte RAINY_UHF::calculateChecksum(byte* dataArray,int len){
  byte checksum;
  for(int i=0;i<len;i++){
    checksum=checksum+dataArray[i];
  }
  //Serial.println(checksum,HEX);
  return checksum; 
}


/**
  * @brief Reads a UART response from the UHF module
  * @return A vector of byte representing the response data
  */
std::vector<byte> RAINY_UHF::readCommand() {
  uhfSerial->flush();
  std::vector<byte> response;
  int len = uhfSerial->available();
  if (len > 0){
    int bytesRead = uhfSerial->readBytes(serialBuffer, len);
    for (int i = 0; i < bytesRead; i++){
      response.push_back(serialBuffer[i]);
    }
    return response;
  }
  else {
    response = {};
    Serial.println("NO MODULE RESPONSE");
    return response;
  }
    
}

/**
  * @brief Retrieves the hardware version of the UHF module
  * @return A string containing the hardware version
  */
String RAINY_UHF::hardwareVersion() {
    uhfSerial->flush();
    while (uhfSerial->available() > 0) {
        uhfSerial->read();  // Read and discard old data
    }
    uhfSerial->write(HARDWARE_VERSION_CMD, sizeof(HARDWARE_VERSION_CMD)); // Send command
    delay(100);
    int len = uhfSerial->available();
    if (len > 0){
      int bytesRead = uhfSerial->readBytes(serialBuffer, len);
      String store ="";
      if (serialBuffer[2]==0x03 && serialBuffer[0]==0XAA){
        for (int i = 0; i < bytesRead; i++){
          store+=(char)serialBuffer[i];
        }
        Serial.flush();
        return store.substring(6,store.length()-2);
      }
      else{
        Serial.flush();
        return "Version Read Failed, Check Connection!";
      }
    } 
}

/**
  * @brief Stops ongoing Tag reading, run only after Multiread operations
  * @return True if the stop command was successful, false otherwise
  */
bool RAINY_UHF::stopRead(){
  uhfSerial->write(STOP_READ_CMD,sizeof(STOP_READ_CMD));
  multireadFlag=true;
  delay(100);
  
  std::vector<byte> response ;
  response = readCommand();
  if (response[2] == 0X28){
    Serial.println("STOP read successfully");
    return true;
  }
  Serial.println("STOP read failed");
  return false;
  
}

/**
  * @brief Performs a single tag read operation
  * @return A vector of byte containing EPC data
  */
std::vector<byte> RAINY_UHF::singleReadTag(){
    uhfSerial->write(SINGLE_READ_CMD, sizeof(SINGLE_READ_CMD));
    delay(100);
    std::vector<byte> tagdata=readCommand();
    
    if (tagdata.size() > 24) {
      tagdata.resize(24);  
    }
    
    if(tagdata[2]==0X22 && tagdata[0]==0Xaa){
      return tagdata;
    }
    else if (tagdata[2]==0Xff) {
      Serial.println("tag reading failed");
      return {};
    }
    else {
      return tagdata;
    }
}

/**
  * @brief Performs a multiple tag read operation
  * @return A vector of byte containing EPC data from multiple tags
  */
std::vector<byte> RAINY_UHF::multipleReadTags() {
    if (multireadFlag == true) {
        uhfSerial->write(MULTIPLE_READ_CMD, sizeof(MULTIPLE_READ_CMD)); // Required to execute only ONCE, then keep reading response
        multireadFlag = false;
    }
    delay(100);
    std::vector<byte> tagdata = readCommand();
    
    for (int i = 0; i < tagdata.size(); i++) {  
        if (tagdata[i] == 0Xaa) {
            if (i + 2 < tagdata.size() && tagdata[i+2] == 0X22) { 
                return tagdata;
            } 
            else{
                Serial.print("no card detected");
                return {};  
            }
        }
    }

    return {};  
}

/**
  * @brief Sets the region of operation for the UHF module
  * @param region: The desired region (e.g., "US" or "EU")
  * @return True if the operation was successful, false otherwise
  */
bool RAINY_UHF::setRegion(String region){
  if (region=="EU"){
    uhfSerial->write(REGION_EU,sizeof(REGION_EU));
  }
  else if (region=="US"){
    uhfSerial->write( REGION_US,sizeof(REGION_US));
  }
  delay(100);
  std::vector<byte> response= readCommand();
  
  if (response.size()>0 && response[2] == 0X07){
    Serial.println(region+" region set successfully");
    return true;
  }
  Serial.println(region+" region set failed");
  return false;
}

/**
  * @brief Sets the transmission power level
  * @param power: Desired power level
  * @return True if the operation was successful, false otherwise
  */
bool RAINY_UHF::setTransmitPower(int power) {
    static const byte SET_TRANSMIT_POWER_CMD[]={0XB6,0X00,0X02};
    int powerValue = power * 100;
    byte highByte = (powerValue >> 8) & 0xFF;
    byte lowByte = powerValue & 0xFF;
   
    std::vector<byte> command;
    
    command.insert(command.end(), SET_TRANSMIT_POWER_CMD, SET_TRANSMIT_POWER_CMD + sizeof(SET_TRANSMIT_POWER_CMD));
    command.push_back(highByte);
    command.push_back(lowByte);
    
    byte checksum = calculateChecksum(command.data(), command.size());
    command.push_back(checksum);
    command.insert(command.end(), END_BYTE, END_BYTE + sizeof(END_BYTE));
    command.insert(command.begin(), START_BYTE, START_BYTE + sizeof(START_BYTE));
    
    uhfSerial->write(command.data(), command.size());
    delay(100);
    std::vector<byte> response = readCommand();
    
    if (response.size() > 2 && response[2] == 0Xb6) {
        Serial.println("Transmission power set successfully");
        return true;
    }
    
    Serial.println("Transmission power set failed");
    return false;
}

/**
  * @brief Retrieves the current transmission power setting
  * @return A string representing the power level
  */
String RAINY_UHF::getTransmitPower(){
  uhfSerial->write(GET_TRANSMIT_POWER_CMD,sizeof(GET_TRANSMIT_POWER_CMD));
  delay(100);
  std::vector<byte> response= readCommand();

  if (response[2] == 0Xb7){
    char hexStr[5];  // Enough space for 4 hex digits + null terminator
        sprintf(hexStr, "%02X%02X", response[5], response[6]);
        int decimalValue = strtol(hexStr, NULL, 16);   // Convert hex string to decimal
        
        return String(decimalValue / 100) + " dBm";
  }
  return "Transmission Power Setting Failed!";
}


/**
  * @brief Selects a specific tag using its EPC
  * @param epc: A vector containing the EPC data
  * @return True if the operation was successful, false otherwise
  */
bool RAINY_UHF:: selectTag(std::vector<byte> epc){
  static const byte SELECT_TAG_CMD[]={0X0C,0X00,0X13,0X01,0X00,0X00,0X00,0X20,0X60,0X00};
  std::vector<byte> command;
  
  command.insert(command.end(), SELECT_TAG_CMD, SELECT_TAG_CMD + sizeof(SELECT_TAG_CMD));
  command.insert(command.end(),epc.begin(),epc.end());
  
  byte checksum = calculateChecksum(command.data(), command.size());
  command.push_back(checksum);
  command.insert(command.end(),END_BYTE,END_BYTE+sizeof(END_BYTE));
  command.insert(command.begin(),START_BYTE,START_BYTE+sizeof(START_BYTE));
  uhfSerial->write(command.data(), command.size());
  delay(100);
  std::vector<byte> response;
  response = readCommand();
  if (response[2]==0X0C){
    Serial.println("TAG select successfully");
    return true;
  }
  return false;
}

/**
  * @brief Reads memory data from a selected tag
  * @param memoryBank: The memory bank to read from (e.g., EPC, TID, User memory)
  * @return A vector of byte containing the memory data
  */
std::vector<byte> RAINY_UHF::readTagMemory(int memoryBank){
  static const byte READ_MEMORY_S[]={0X39,0X00,0X09,0X00,0X00,0X00,0X00};
  static const byte READ_MEMORY_E[]={0X00,0X00,0X00,0X08};
  
  std::vector<byte> command;
  command.insert(command.end(),READ_MEMORY_S,READ_MEMORY_S+sizeof(READ_MEMORY_S));
  byte a = byte(memoryBank);
  command.push_back(a);
  command.insert(command.end(),READ_MEMORY_E,READ_MEMORY_E+sizeof(READ_MEMORY_E));
  byte checksum = calculateChecksum(command.data(),command.size());
  command.push_back(checksum);
  command.insert(command.end(),END_BYTE,END_BYTE+sizeof(END_BYTE));
  command.insert(command.begin(),START_BYTE,START_BYTE+sizeof(START_BYTE));
  
  uhfSerial->write(command.data(), command.size());
  delay(100);
  std::vector<byte> response= readCommand();
  
  if (response[2]==0X39){
    response.erase(response.begin(), response.begin() + 20);
    response.erase(response.end() - 2, response.end());
    return response;
  }
  else {
    Serial.println("Tag reading failed");
    return response;
  }

}

/**
  * @brief Writes data to a selected tag's memory bank
  * @param memoryBankData: A vector containing the data to write
  * @param memoryBank: The memory bank to write to
  * @return True if the operation was successful, false otherwise
  */
bool RAINY_UHF::writeTagMemory(std::vector<byte> memoryBankData,int memoryBank){
  static const byte WRITE_MEMORY_S[]={0X49,0X00,0X19,0X00,0X00,0X00,0X00};
  static const byte WRITE_MEMORY_E[]={0X00,0X00,0X00,0X08};
  std::vector<byte> command;
   command.insert(command.end(),WRITE_MEMORY_S,WRITE_MEMORY_S+sizeof(WRITE_MEMORY_S));
   command.push_back(byte(memoryBank));
   command.insert(command.end(),WRITE_MEMORY_E,WRITE_MEMORY_E+sizeof(WRITE_MEMORY_E));
   command.insert(command.end(), memoryBankData.begin(), memoryBankData.end());
  byte checksum = calculateChecksum(command.data(),command.size());
  command.push_back(checksum);
  command.insert(command.end(),END_BYTE,END_BYTE+sizeof(END_BYTE));
  command.insert(command.begin(),START_BYTE,START_BYTE+sizeof(START_BYTE));
  for(byte&hex: command){
    Serial.print(hex,HEX);
    Serial.write(32);
  }
  Serial.println();
  uhfSerial->write(command.data(), command.size());
  delay(100);
  
  std::vector<byte> response ;
  response = readCommand();
  if (response.size()>0&&response[2]==0X49){
      Serial.println("Memory Data write successfully");
      return true;
  }
  Serial.println("Memory Write Failed!");
  return false;
}
