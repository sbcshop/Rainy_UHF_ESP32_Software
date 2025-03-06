# Rainy_UHF_ESP32_Software

<img src="https://github.com/sbcshop/Rainy_UHF_ESP32_Software/blob/main/images/feature_banner.jpg" width="" height=""> 

Unlock the power of IoT and RFID with this ESP32-S3-based dev board, perfect for smart tracking, automation, and wireless control. With long-range UHF RFID (1-20m), a vibrant 1.14” TFT display, dual USB-C, and WiFi/BLE connectivity, it’s built for seamless real-time data processing. Whether for inventory management, access control, or interactive projects, this board delivers versatility and performance in one compact solution.

This GitHub repository provides Rainy UHF for ESP32 getting started instructions.

### Features:
- Powered by **ESP32 S3 Wroom-1** microcontroller module
- Its WiFi/BLE capabilities make it suited for a variety of IoT applications
- **1.14"** TFT Display for visual interaction
- **SMA Antenna port** to connect antenna of choice for increased range 1-20m
- Onboard **Buzzer** and **User** LED to show or add alert for any activity
- **Programmable Buttons** for adding additional control features while Boot and Reset Buttons for programming purposes
- Onboard **microSD card** compatibility is useful for data logging projects
- **Two USB Type-C** interface one for Native ESP32 USB for programming and other for Rainy UHF module access
- **GPIOs Breakout** for peripheral sensors or actuator interfacing
- **Jumper option** to choose between ESP32 (UART TTL) and USB Mode, so you can use onboard UHF either with ESP32 or directly with software
 
### Specifications:
- **Microcontroller**  : ESP32-S3 series of SoCs having Xtensa® dual-core 32-bit LX7 microprocessor
- **Connectivity** : 2.4 GHz Wi-Fi (802.11 b/g/n) and Bluetooth® 5 (LE)
- **Memory** : Flash up to 16 MB, PSRAM up to 8 MB
- **Working mode** : Support STA/AP/STA+AP 
- **Supply Voltage:** 5V
- **Operating Pin:** 3.3V ~ 5V
- **Display Size**: 1.14"
- **Display Resolution**: 135x240 pixel
- **Communication Interface:** TTL (UART), Type C
- **Frequency:** EU 865-868MHz or US 902-928MHz
- **Protocol:** ISO 18000-6C (EPC Gen 2)
- **Read speed:** Multi-tag,1-60tags/second (depend on antenna/tag and application)
- **Read range:**- 1-20m (depend on tag, antenna and application)
- **Read/Write Capability:** Yes
- **RF Power Output:** 15-26 dBm(adjustable)
- **Cooling Method:** Air cooling (no external heat sink required)
- **Antenna port:** 1 port ,SMA
- **Estimated Antenna Range:** 
     - **3dBi Antenna:** 0-2m Range
     - **5.5dBi Antenna:** 0-5m Range
       
## Getting Started with Rainy UHF for ESP32
### Pinout 
<img src="https://github.com/sbcshop/Rainy_UHF_ESP32_Software/blob/main/images/Pinouts.jpg" width="" height=""> 

### Interfacing Details

 - _**Rainy UHF Module with ESP32**_
   
   | ESP32 | Rainy UHF Module | Description | 
   |---|---|---|
   | TXD0/GPIO43 | UHF RXD | Serial UART communication pin |
   | RXD0/GPIO44 | UHF TXD | Serial UART communication pin |
   | IO40        | UHF EN  | Enable pin, HIGH - activate, LOW - deactivate  |
   
- _**Display Interfacing with ESP32**_

   | ESP32 | Display | Function |
   |---|---|---|
   | IO12 | LCD_CLK | Clock pin of SPI interface for Display|
   | IO11 | LCD_DIN | MOSI (Master OUT Slave IN) pin of SPI interface|
   | IO10 | LCD_CS | Chip Select pin of SPI interface|
   | IO13 | LCD_DC| Data/Command (MISO) pin of SPI interface|
   | IO14 | LCD_RST | Display Reset pin |
   | IO9  | BL | Backlight of display|
  
 - **_SDcard Interface_**

   | ESP32 | SDCard | Function |
   |---|---|---|
   | IO42 | CARD_CLK | Clock pin of SPI interface for Display|
   | IO2  | CARD_MOSI | MOSI (Master OUT Slave IN) pin of SPI interface|
   | IO41 | CARD_MISO  | MISO (Master IN Slave OUT) pin of SPI interface|
   | IO1  | CARD_CS  | Chip Select pin of SPI interface|

- **_Buzzer, LED and Buttons Interfacing_**

   | ESP32 | Hardware | Function |
   |---|---|---| 
   |IO0 | BOOT |Boot button |
   |IO15 | BT1 | Programmable Button |
   |IO16 | BT2 | Programmable Button |
   |IO48 | Buzzer | Buzzer Positive Pin |
   |IO3  | LED | User LED |

- _**Breakout Pins**_
  
   | ESP32 | Type* | Multi-Function (_**Bold-Italic**_ default Function) |
   |---|---|---|
   |GND  | P     | Supply Ground |
   |GP38 | I/O/T | _**GPIO38**_ , FSPIWP, SUBSPIWP  |
   |GP39 | I/O/T | _**MTCK**_ , GPIO39, CLK_OUT3, SUBSPICS1 |
   |GP17  | I/O/T | RTC_GPIO17, _**GPIO17**_, U1TXD, ADC2_CH6 |  
   |GP18  | I/O/T | RTC_GPIO18, _**GPIO18**_, U1RXD, ADC2_CH7, CLK_OUT3 |
   |GP07  | I/O/T | RTC_GPIO7, _**GPIO7**_, TOUCH7, ADC1_CH6  |
   |GP06 | I/O/T | RTC_GPIO6, _**GPIO6**_, TOUCH6, ADC1_CH5  |
   |GP05 | I/O/T | RTC_GPIO5, _**GPIO5**_, TOUCH5, ADC1_CH4 |
   |GP04 | I/O/T | RTC_GPIO4, _**GPIO4**_, TOUCH4, ADC1_CH3 |
   |5V   | P     | Positive Supply, 5V |

  *I-INPUT, O-OUTPUT, P-POWER & T-HIGH IMPEDENCE


### 1. Configure and Setup Development Environment
   - Download Arduino IDE from [official site](https://www.arduino.cc/en/software) and install into your system. We have use Arduino IDE 1.8.19
   - Once installation done will add ESP32 S3 board support into IDE, for this first you need to add below link into preference:
     
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
     Select File > Preference, and add link as show in below image,

     <img src= "https://github.com/sbcshop/3.2_Touchsy_ESP-32_Resistive_Software/blob/main/images/preference_board.gif" />
      
   - Now will install ESP32 based boards as shown in below image,

     <img src= "https://github.com/sbcshop/3.2_Touchsy_ESP-32_Resistive_Software/blob/main/images/install_ESP32boards.gif" />
   
   - You have two Type-C options, use ESP32_USB side to program onboard ESP32 controller and set jumper setting on UHF-ESP. Here Native USB of ESP32 is used, so you will have to press hold BOOT button once and then connect Type C. 

     <img src="https://github.com/sbcshop/Rainy_UHF_ESP32_Software/blob/main/images/uhf_withESP32.png" width="381" height="286">   
   
   - When using USB native you will get COM PORT as shown in below image, and while uploading you can enable CDC Mode to visualize data on serial com port.
     
     <img src="https://github.com/sbcshop/2x2_Display_ESP32_Software/blob/main/images/Native_USB_device_com_port.jpg" width="410" height="93">
     
     <img src="https://github.com/sbcshop/2x2_Display_ESP32_Software/blob/main/images/Native_USB_Arduino_com.jpg" width="" height="">
     
        
### 2. Installing Libraries
   - When compiling sample codes there are some dependency on external libraries sometime which you can add as shown here.
   - For example installing library, for display select Sketch > Include Library > Manage Libraries. We need ST7789 (1.10.3 version) and GFX library (1.11.7 version) for 1.14" TFT Display,

     <img src= "https://github.com/sbcshop/EnkFi_7.5_Software/blob/main/images/Lib_install.png" />

     <img src= "https://github.com/sbcshop/Rainy_UHF_ESP32_Software/blob/main/images/st7789_lib.png" width="589" height="228" />
     <img src= "https://github.com/sbcshop/Rainy_UHF_ESP32_Software/blob/main/images/GFX_lib.png" width="588" height="217" />

   - Similarly you can add more libraries if needed, make sure to install correct version. 


### 3. Testing First Code
   - At this step you are all set to test codes, for easy getting started we have provided various demo [example codes]() in github which you can download and try. 
   - Open one example code in Arduino and make sure you have selected correct board with suitable com port, click on upload button to transfer code on board.
     <img src="https://github.com/sbcshop/StackyFi_Software/blob/main/images/upload_code.gif">
   - Checkout other more examples [here]() and build your own custom program codes using those references.

## Rainy UHF Module with Application
* You can use Rainy UHF module directly with application. For this change jumper setting USB-UHF mode and connect hardware to laptop/PC using Type C UHF_USB as shown below.
  
  <img src="https://github.com/sbcshop/Rainy_UHF_ESP32_Software/blob/main/images/UHF_withapp.png" width="506" height="353">

* Now you can follow steps mentioned [here](https://github.com/sbcshop/Rainy_UHF_Breakout_Software#rainy-uhf-breakout-standalone) to use Rainy UHF module with standalone windows application.
  
## Resources
  * [Schematic](https://github.com/sbcshop/Rainy_UHF_ESP32_Hardware/blob/main/Design%20Data/Rainy%20UHF%20FOR%20ESP32%20Sch.PDF)
  * [Hardware Files](https://github.com/sbcshop/Rainy_UHF_ESP32_Hardware)
  * [Rainy UHF Module Command Manual](https://github.com/sbcshop/Rainy_UHF_Breakout_Software/blob/main/Document/Rainy%20UHF%20Module%20Command%20Manual.pdf)
  * [CH340 Driver Installation Guide](https://github.com/sbcshop/NFC_Module/blob/main/documents/CH340%20Driver%20installation%20steps.pdf)
  * [Getting Started with ESP32 in Arduino](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
  * [ESP32 S3 Hardware Reference](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/hw-reference/index.html)
  * [ESP32 S3 Datasheet](https://github.com/sbcshop/3.2_Touchsy_ESP-32_Capacitive_Software/blob/main/documents/esp32-s3-wroom-1_wroom-1u_datasheet_en.pdf)
  * [Arduino IDE 1 overview](https://docs.arduino.cc/software/ide-v1/tutorials/Environment)
       

## Related Products
   * [Rainy UHF Pico Expansion](https://shop.sb-components.co.uk/products/rainypi-uhf-based-on-pico-complete-kit) -  UHF Expansion board easily incorporate Pico/Pico W/Pico 2
   * [Rainy UHF HAT](https://shop.sb-components.co.uk/products/rainy-uhf-pi-hat-complete-kit) - UHF module HAT with Standard 40pin to support Raspberry Pi
   * [Rainy UHF SHIELD](https://shop.sb-components.co.uk/products/rainy-shield-for-arduino-board-complete-kit) - UHF Shield form factor for use with Arduino, Ardi-32, Ardi-Pi, and other compatible boards.
   * [Rainy UHF Breakout](https://shop.sb-components.co.uk/products/rainy-uhf-breakout-complete-ki) - Compact UHF module breakout with Type C for standalone use and TTL for interfacing with various MCU.

## Product License

This is ***open source*** product. Kindly check LICENSE.md file for more information.

Please contact support@sb-components.co.uk for technical support.
<p align="center">
  <img width="360" height="100" src="https://cdn.shopify.com/s/files/1/1217/2104/files/Logo_sb_component_3.png?v=1666086771&width=300">
</p>
