#ifndef RAINY_UHF_H
#define RAINY_UHF_H
#include <vector>
#include <Arduino.h>
#include <HardwareSerial.h>

// CONSTANT RAINY UHF COMMANDS
static const byte SINGLE_READ_CMD[]={0XAA,0X00,0X22,0X00,0X00,0X22,0XDD};
static const byte HARDWARE_VERSION_CMD[]={0XAA,0X00, 0X03, 0X00, 0X01, 0X00, 0X04,0XDD};
static const byte MULTIPLE_READ_CMD[]={0XAA,0X00,0X27,0X00,0X03,0X22,0X27,0X10,0X83,0XDD};
static const byte STOP_READ_CMD[]={0XAA,0X00,0X28,0X00,0X00,0X28,0XDD};
static const byte START_BYTE[]={0XAA,0X00};
static const byte END_BYTE[]={0XDD};
static const byte GET_TRANSMIT_POWER_CMD[]={0xAA,0X00,0XB7,0X00,0X00,0XB7,0XDD};
static const byte REGION_US[]={0XAA,0X00,0X07,0X00,0X01,0X02,0X0A,0XDD};
static const byte REGION_EU[]={0XAA,0X00,0X07,0X00,0X01,0X03,0X0B,0XDD};


class RAINY_UHF {
public:              
    RAINY_UHF();
    void begin(long baudrate = 115200, int rxPin = 44, int txPin = 43);
    
    byte calculateChecksum(byte* dat,int len);
    std::vector<byte> readCommand();
    String hardwareVersion(); 
    
    bool stopRead();
    std::vector<byte> singleReadTag();
    std::vector<byte> multipleReadTags();

    bool setRegion(String region);
    bool setTransmitPower(int power);
    String getTransmitPower();

    bool selectTag(std::vector<byte> epc);
    std::vector<byte> readTagMemory(int memoryBank);
    bool writeTagMemory(std::vector<byte> memoryBankData,int memoryBank);

    
private:
    HardwareSerial* uhfSerial;
    bool multireadFlag=true;
    static byte serialBuffer[1024];
};

#endif
