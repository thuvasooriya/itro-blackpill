#include <User_Setup.h>
#include <Arduino.h>
///// board pin layout /////

// B12      5V
// B13      G
// B14      3v3
// B15      B10
// A8       B2
// A9       B1
// A10      B0
// A11      A7
// A12      A6
// A15      A5
// B3       A4
// B4       A3
// B5       A2
// B6       A1
// B7       A0
// B8       R
// B9       C15
// 5V       C14
// G        C13
// 3V3      VB


// ### Pin Plan ###
// # IR #
// B12      
// B13      
// B14      
// B15      
// A8
#define IR1 PB12
#define IR2 PB13
#define IR3 PB14
#define IR4 PB15
#define IR5 PA8

// # I2C # : 6 TOF, 2 CS, Acellerometer
// B6
// B7

// # UART #
// A9
// A10

// # FLOATIN :) #
// A11 AN
// A12 DG
// B10 AN

// # MOTORS #
// A15 pwm
// B3  pwm
// B4  
// B5       
// B8       
// B9
#define AIN1 PB4
#define AIN2 PB5
#define BIN1 PB9
#define BIN2 PB8
#define PWMA PB3
#define PWMB PA15
// #define STBY PB0 // hardwired to HIGH 3V3
// B1 enc
// B0 enc
#define encoderInA PB0
#define encoderInB PB1

// # SERVO #
// A2
// A1

// # DISPLAY #
// A7
// A6
// A5
// A4
// R
// B2 CS ????

// # MIC #
// A3

// ULTRA SONIC
// C15
// C14

// A0  // USER_KEY
// C13 // LED

void setup_pins() {
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(IR5, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(USER_BTN, INPUT_PULLUP);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  
  pinMode(encoderInA, INPUT);
  pinMode(encoderInB, INPUT);

  pinMode(TFT_CS, OUTPUT);


  // pinMode(triggerPin, OUTPUT);
  // pinMode(echoPin, INPUT);

  // pinMode(encoderInA, INPUT);
  // attachInterrupt(digitalPinToInterrupt(encoderInA), encIncrementA, RISING);
  // pinMode(encoderInB, INPUT);
  // attachInterrupt(digitalPinToInterrupt(encoderInB), encIncrementB, RISING);

  // rot_servo.attach(SER_R); // attaches the servo on GIO2 to the servo object
  // rot_servo.write(init_rot_pos);
  // lin_servo.attach(SER_L);
  // lin_servo.write(upper_pos);

  // Serial.begin(9600);
  // brake_fast();
  Wire.begin();
}