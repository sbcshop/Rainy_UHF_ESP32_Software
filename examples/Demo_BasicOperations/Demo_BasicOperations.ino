/*
 * Demo code to perform some Basic Rainy UHF operations
 */
#include "RAINY_UHF.h"  // include Rainy UHF library
#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789

#define UHF_ENABLE 40 // Rainy UHF Enable pin

RAINY_UHF uhf;   // create Rainy UHF module instance 

// define Display SPI pins 
#define TFT_CS        10  
#define TFT_RST       14 
#define TFT_DC        13
#define TFT_MOSI      11
#define TFT_CLK       12
#define BackLight     9
// configure SPI and create instance for display 
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);

void displayText(String text, int x, int y){
  tft.setTextSize(2); 
  tft.setTextWrap(false);
  tft.setCursor(x, y);
  tft.setTextColor(ST77XX_YELLOW);
  tft.println(text);
}

void setup() {
    pinMode(UHF_ENABLE, OUTPUT);     // Initialize pin as OUTPUT
    digitalWrite(UHF_ENABLE, HIGH);  // HIGH - Enable UHF module, LOW -Disable Module

    pinMode(BackLight,OUTPUT); // Set backLight LED pin OUTPUT
    digitalWrite(BackLight,HIGH); // turn display backlight ON
    
    Serial.begin(115200); // Initialize SerialTerimal-ESP32 communication 
    uhf.begin();          // Initialize UHF-ESP32 UART communication 

    tft.init(135, 240);           // Init ST7789 240x135
    // SPI speed defaults to SPI_DEFAULT_FREQ defined in the library, you can override it here
    // Note that speed allowable depends on chip and quality of wiring, if you go too fast, you
    // may end up with a black screen some times, or all the time.
    //tft.setSPISpeed(40000000);
    Serial.println(F("Initialized"));

    tft.setRotation(3); // Change display rotation as per requirement, value-> 0 (defualt), 1, 2, 3
    tft.setTextSize(1); 
    tft.setTextWrap(false);
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(0, 30);
    tft.setTextSize(3);
    tft.setTextColor(ST77XX_YELLOW);
    tft.println("Hello...");
    delay(500);
    Serial.println("Ready!");
    tft.fillScreen(ST77XX_BLACK);
}


void loop(){
  // Get Hardware version 
  String a = uhf.hardwareVersion();
  Serial.println(a);
  displayText(a, 20, 20);
  delay(100);
  
  // Set the transmit power level (acceptable range: 15-26 dBm)
  uhf.setTransmitPower(18);
  delay(100);
  // Get the transmit power level 
  String response = uhf.getTransmitPower();
  String msg = "Power: " + response;
  displayText(msg, 20, 50);
  Serial.println(response);
  delay(100);
  tft.fillScreen(ST77XX_BLACK);
}
