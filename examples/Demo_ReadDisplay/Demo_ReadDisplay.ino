/*
 * Demo code to test onboard TFT Display with UHF EPC Extraction
 * 
 * Interfacing Details:
 * IO44/U0RXD - UHF_TXD 
 * IO43/U0TXD - UHF_RXD 
 * IO40       - UHF_EN 
 * 
 * IO48  - Buzzer
 * IO3 - User LED
 * 
 * ESP32 <-> Display
 * IO14 - TFT_RST 
 * IO13 - TFT_DC  
 * IO10 - TFT_CS  
 * IO11 - TFT_MOSI  
 * IO12 - TFT_CLK 
 * IO9  - TFT_BL  
 */
#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include "RAINY_UHF.h"  // Include Rainy UHF library

// Define Display SPI pins 
#define TFT_CS        10  
#define TFT_RST       14 
#define TFT_DC        13
#define TFT_MOSI      11
#define TFT_CLK       12
#define BackLight     9
#define UHF_ENABLE    40 // Rainy UHF Enable pin

RAINY_UHF uhf;   // Create Rainy UHF module instance 

// Configure SPI and create instance for display 
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);

const int buzzerPin = 48 ;    // Buzzer GPIO pin
const int userLED = 3; // programmable led GPIO pin

void beep(int note, int duration){  // need note frequency and time duration (in millis)
  digitalWrite(userLED, HIGH);
  tone(buzzerPin, note, duration);
  delay(duration);
  digitalWrite(userLED, LOW);
}

void displayText(String text, int x, int y, uint16_t color, int size) {
  tft.setTextSize(size); 
  tft.setTextWrap(false);
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.println(text);
}

void setup() {
    pinMode(UHF_ENABLE, OUTPUT);     // Initialize pin as OUTPUT
    pinMode(BackLight, OUTPUT); // Set backLight LED pin OUTPUT
    pinMode(userLED, OUTPUT); // Set LED pin OUTPUT
    pinMode(buzzerPin, OUTPUT); // Set Buzzer LED pin OUTPUT
    
    digitalWrite(UHF_ENABLE, HIGH);  // Enable UHF module
    digitalWrite(BackLight, HIGH); // Turn display backlight ON

    Serial.begin(115200); // Initialize Serial Terminal communication 
    uhf.begin();          // Initialize UHF-ESP32 UART communication 

    tft.init(135, 240);   // Init ST7789 240x135
    Serial.println(F("Initialized"));

    tft.setRotation(3); // Change display rotation as per requirement
    tft.fillScreen(ST77XX_BLACK);
    tft.drawRect(5, 5, 230, 125, ST77XX_WHITE); // Surrounding border
    displayText("Rainy UHF", 20, 30, ST77XX_YELLOW, 3); // Display Text
    displayText("for ESP32", 20, 80, ST77XX_YELLOW, 3);
    delay(2000);
    
    tft.fillScreen(ST77XX_BLACK);
    tft.drawRect(5, 5, 230, 125, ST77XX_WHITE); // Redraw border
    displayText("Read Demo!", 30, 30, ST77XX_GREEN, 3);
    beep(1245, 500);
    delay(3000);
    tft.fillScreen(ST77XX_BLACK);
}


void loop() {
    std::vector<byte> single;  // Variable to hold single polling response from Rainy UHF
    single = uhf.singleReadTag();   // Perform single polling operation
      
    for (byte& hexData : single){
      Serial.print(hexData,HEX);       // print reponse data
      Serial.write(32);            // add space between each hex byte value
    }

    Serial.println();
    
    String label = "EPC: ", epcText = "";
    if (single.size() > 10) { // Ensure we have a valid response
        beep(1245, 500);
        for (int i = 8; i < single.size() - 4; i++) { // Extract EPC bytes
            String hexValue = String(single[i], HEX); // Convert byte to HEX string
            if (hexValue.length() == 1) {
                hexValue = "0" + hexValue;  // Ensure two-digit formatting
            }
            epcText += hexValue + " ";
            
        }
        Serial.print(label);
        Serial.println(epcText);  // Print extracted EPC
        // Display EPC on TFT
        tft.fillScreen(ST77XX_BLACK);
        displayText(label, 5, 30, ST77XX_YELLOW, 1);
        displayText(epcText, 5, 50, ST77XX_YELLOW, 1);
        delay(2000);
    } 
    else {
        label = "No EPC Found";
        Serial.println(label);
        tft.fillScreen(ST77XX_BLACK);
        displayText(label, 5, 30, ST77XX_YELLOW, 2);
        delay(500);
    }
    
    delay(10);
}
