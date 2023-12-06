#include <SPI.h>
// # Servo Motors #
// #define PB0 PIN_A8
// #define PB1 PIN_A9
// motor driver pins
#define AIN1 PB4
#define AIN2 PB5
#define BIN1 PB9
#define BIN2 PB8
#define PWMA PB3
#define PWMB PA15
// #define STBY PB0 // hardwired to HIGH 3V3

// encoder pins
#define encoderInA PB0
#define encoderInB PB1

// IR sensor pins
#define IR1 PB12
#define IR2 PB13
#define IR3 PB14
#define IR4 PB15
#define IR5 PA8

// B9                   
// B8 B7                C13 C14
// B6 B5                C15 C0
// B4 B3                C1  C2
// D2 C12               C3  A0
// C11 C10              A1  A2
// A15 A12              A3  A4
// A11 A10              A5  A6
// A9 A8                A7  C4
// C9 C8                C5  B0
// C7 C6                B1  B2
// B15 B14              B10 B11
// B13 B12


// ### Pin Plan ###

// # FREE #

// #define PC3 PIN_A13 // SPI2_MOSI
// #define PC4 PIN_A14 // GPIO_Output
// #define PC5 PIN_A15

// #define PA15 15 // SYS_JTDI - SPI1_SS - SPI3_SS_ALT
// #define PA8 8   // SD_DETECT_PIN - I2C3_SCL
// #define PC12 44 // SDIO_CLK
// #define PB3 19  // SYS_JTDO - SWO - SPI1_SCK - SPI3_SCK_ALT
// #define PB4 20  // SYS_JTRST - SPI1_MISO - SPI3_MISO_ALT
// #define PB5 21  // SPI1_MOSI - SPI3_MOSI_ALT
// #define PB8 24  // I2C1_SCL
// #define PB9 25  // NO_CAP_FOR_F405 - I2C1_SDA - SPI2_SS

// # Doubtful Free
// #define PD2 48  // SDIO_CMD
// #define PC14 46 // OSC32_IN - SB
// #define PC15 47 // OSC32_OUT - SB
// #define PC11 43 // SDIO_D3-CD - 3V3


// # IR #
// IR1 #define PC6 38
// IR2 #define PC7 39
// IR3 #define PC8 40  // SDIO_D0
// IR4 #define PC9 41  // SDIO_D1-I2C3_SDA
// IR5 #define PC10 42 // SDIO_D2

// # I2C # 6 TOF 2 CS Acellerometer
// #define PB6 22  // I2C1_SCL
// #define PB7 23  // I2C1_SDA

// # Motor Pins #
// #define PB10 26 // I2C2_SCL - SPI2_SCK
// #define PB11 27 // NO_CAP_FOR_F405 - I2C2_SDA
// #define PB12 28 // SPI2_SS
// #define PB13 29 // SPI2_SCK
// #define PC1 PIN_A11
// #define PC2 PIN_A12 // SPI2_MISO

// # Encoder #
// #define PA4 PIN_A4 // SPI1_SS - DAC - SPI3_SS_ALT
// #define PA3 PIN_A3
// # Servo Motors #
// #define PB0 PIN_A8
// #define PB1 PIN_A9

// # Display #
// #define PA0 PIN_A0 // WAKEUP
// #define PA1 PIN_A1 //
// #define PA2 PIN_A2 //
// #define PA5 PIN_A5 // SPI1_SCK - DAC
// #define PA6 PIN_A6 // SPI1_MISO
// #define PA7 PIN_A7 // SPI1_MOSI

// # Ultrasound Sensor #
// #define PB14 30 // SPI2_MISO
// #define PB15 31 // SPI2_MOSI

// # Microphone #
// #define PC0 PIN_A10

// ### Reserved ###
// #define PA11 11    // USB_DN - USB_DM
// #define PA12 12    // USB_DP
// #define PA9 9      // USART1_TX
// #define PA10 10    // USART1_RX
// #define PC13 45 // USER_KEY
// #define PB2 18  // LED_BUILTIN
// #define PA13 13    // SYS_JTMS - SWDIO
// #define PA14 14    // SYS_JTCK - SWCLK

