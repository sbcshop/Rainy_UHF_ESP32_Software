/*
 * Demo code to test onboard TFT Display
 */
#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789

// Define Display SPI pins 
#define TFT_CS        10  
#define TFT_RST       14 
#define TFT_DC        13
#define TFT_MOSI      11
#define TFT_CLK       12
#define BackLight     9

// Configure SPI and create instance for display 
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST);

const int buzzerPin = 48;    // Buzzer GPIO pin
const int bootButton = 0;   // Boot button GPIO pin 
const int userButton1 = 15;  // Programmable Button 1 
const int userButton2 = 16;  // Programmable Button 2 
const int userLED = 3; // programmable led for status

void displayText(String text, int x, int y, uint16_t color, int size) {
  tft.setTextSize(size); 
  tft.setTextWrap(false);
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.println(text);
}

void beep(int note, int duration){  // need note frequency and time duration (in millis)
  digitalWrite(userLED, HIGH);
  tone(buzzerPin, note, duration);
  delay(duration);
  digitalWrite(userLED, LOW);
}

void setup() {
    pinMode(bootButton, INPUT_PULLUP);  // set boot Button pin to INPUT PULLUP
    pinMode(userButton1, INPUT_PULLUP); // set user button1 as INPUT_PULLUP 
    pinMode(userButton2, INPUT_PULLUP); // set user button2 as INPUT_PULLUP 
    pinMode(buzzerPin, OUTPUT); // set Buzzer pin as OUTPUT
    pinMode(userLED, OUTPUT);  // set User led pin as OUTPUT

    pinMode(BackLight, OUTPUT); // Set backLight LED pin OUTPUT
    digitalWrite(BackLight, HIGH); // Turn display backlight ON
    
    Serial.begin(115200); // Initialize SerialTerminal-ESP32 communication 
    
    tft.init(135, 240);           // Init ST7789 240x135
    Serial.println(F("Initialized"));

    tft.setRotation(3); // Change display rotation as per requirement
    tft.fillScreen(ST77XX_BLACK);
    
    // Draw outer box
    tft.drawRect(5, 5, 230, 125, ST77XX_WHITE); // Surrounding border
    
    displayText("Press Buttons..!", 20, 20, ST77XX_YELLOW, 2);
    
    delay(2000);
}

void loop() {
  
  if (digitalRead(bootButton)==0){      // Check if bootButton Pressed or NOT, LOW when Pressed else HIGH
    tft.fillScreen(ST77XX_BLACK);
    Serial.println("Boot Button Pressed!");
    tft.drawRect(5, 5, 230, 125, ST77XX_WHITE); // Surrounding border
    displayText("BootBTN", 40, 30, ST77XX_RED, 3);
    displayText("Pressed!", 40, 70, ST77XX_RED, 3);   
    beep(1245,500);
    delay(50);
  }

  if (digitalRead(userButton1)==0){     // Check if userButton1 Pressed or NOT, LOW when Pressed else HIGH
    tft.fillScreen(ST77XX_BLACK);
    Serial.println("BTN 1 Pressed!");
    tft.drawRect(5, 5, 230, 125, ST77XX_WHITE); // Surrounding border
    displayText("BUTTON 1", 40, 30, ST77XX_GREEN, 3);
    displayText("Pressed!", 40, 70, ST77XX_GREEN, 3);   
    beep(1245,500);
    delay(50);
  }

  if (digitalRead(userButton2)==0){     // Check if userButton2 Pressed or NOT, LOW when Pressed else HIGH
    tft.fillScreen(ST77XX_BLACK);
    Serial.println("BTN 2 Pressed!");
    tft.drawRect(5, 5, 230, 125, ST77XX_WHITE); // Surrounding border
    displayText("BUTTON 2", 40, 30, ST77XX_YELLOW, 3);
    displayText("Pressed!", 40, 70, ST77XX_YELLOW, 3);
    beep(1245,500);
    delay(50);
  }
  
  delay(50);
}
