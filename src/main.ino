#include <motor.h>
#include <color_sensor.h>
#include <tof_3.h>

void line_following();
void line_following_only();
void pick_box();
void wall_following();

String box_color = "";
String path_color = "";

void setup()
{
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
  attachInterrupt(digitalPinToInterrupt(encoderInA), encIncrementA, RISING);
  pinMode(encoderInB, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderInB), encIncrementB, RISING);

  Serial.begin(9600);
  brake_fast();

//   Serial.println("Adafruit VL53L0X test");
//   if (!lox.begin())
//   {
//     Serial.println(F("Failed to boot VL53L0X"));
//     while (1)
//       ;
//   }
}

void go_5cms_line(int i)
{
  countA = 0;
  while (countA < (2000 * i))
  {
    set_forward();
    set_speed(avg_speed, avg_speed);
    line_following();
  }
  brake_fast();
}

void loop()
{
  // Serial.println("rekkit: ");
  // Serial.println(rekkit);
  // Serial.println(digitalRead(USER_BTN));
  rekkit = (btn_hold(20)) ? true : rekkit;
  if (rekkit)
  {
    switch (level)
    {
    case 0:
    line_following();
      // pick_box();
    //   if (measure_tof() < 80)
    //   {
    //     wall_following();
    //   }
    //   else
    //   {
    //     line_following();
    //   }
    // countB = 0;
    // delay(500);
    // set_forward();
    // set_speed(80,80);
    // delay(1000);
    // Serial.println(countB);
    // level = 1;
    break;
    case 2:
      // pick_box();
      break;
    default:
      brake_free();
      rekkit = false;
      level = 0;
      good_night();
      break;
    }
  }
  else
    _blink(3, 300, 100, 1000);
}

void line_following()
{
  offset = get_deviation();
  print_ir();
  // Serial.print("offset: ");
  // Serial.println(offset);
  on_line = (offset == 999) ? false : true;
  if (on_line)
    do_pid(calc_pid(offset));
  else
  {
    // Serial.println("not on line");
    prevError = 0;
    integral = 0;
    derivative = 0;
    handle_edge_cases();
  }
  delay(100);  //20
}

void line_following_only()
{
  offset = get_deviation();
  print_ir();
  // Serial.print("offset: ");
  // Serial.println(offset);
  on_line = (offset == 999) ? false : true;
  if (on_line)
    do_pid(calc_pid(offset));
  else
  {
    // Serial.println("not on line");
    prevError = 0;
    integral = 0;
    derivative = 0;
    if (allblack())
    {
      counter_measures(offset);
    }
  }
  delay(20);
}

void pick_box()
{
  while (allwhite())
  {
    set_forward();
    set_speed(avg_speed, avg_speed);
  }
  brake_fast();
  delay(1000);
  set_forward();
  set_speed(avg_speed, avg_speed);
  while (not(allwhite()))
  {
    line_following_only();
  }
  brake_fast();
  delay(100);
  sharpLeft2(avg_speed);
  set_forward();
  set_speed(avg_speed, avg_speed);
  countA = 0;
  while (countA < 4000)
  {
    line_following_only();
  }
  brake_fast();
  delay(1000);
  align_center();
  path_color = detect_color();
  int ctt = 0;
  while ((path_color != "red") and (path_color != "blue") and (ctt < 5))
  {
    scanLeft(1);
    path_color = detect_color();
    ctt++;
  }
  align_center();
  ctt = 0;
  while ((path_color != "red") and (path_color != "blue") and (ctt < 5))
  {
    scanRight(1);
    path_color = detect_color();
    ctt++;
  }
  align_center();
  delay(1000);
  U_turn();
  while (not(allwhite()))
  {
    line_following_only();
  }
  brake_fast();
  delay(100);
  sharpLeft2(avg_speed);
  //

  delay(2000);
  align_center();
  delay(1000);
  int cctt = 0;
  while (cctt < 5)
  {
    scanLeft(1);
    if (cctt == 3)
    {
      break;
    }
    cctt++;
  }
  go_5cms(2);
  // lift_bok();
  // detect_box_color();
  box_color = "red";
  reverse_5cms(2);
  delay(2000);
  align_center();
  delay(1000);
  if (path_color == box_color)
  {
    sharpLeft3(avg_speed);
  }
  else
  {
    sharpRight3(avg_speed);
  }
  brake_free();
  delay(2000);
  bool check = false;
  while (not(check))
  {
    line_following();
  }
}

void wall_following()
{
  brake_fast();
  delay(2000);
  scanLeft(5);
  delay(3000);
  set_forward();
  set_speed(125, 120);
  delay(800);
  while ((get_deviation() > 4) and (get_deviation() < -4))
  {
    set_forward();
    set_speed(125, 120);
  }
  brake_fast();
  delay(1000);
}

// #include <color_sensor.h>
// #include <Arduino.h>
// #include <i2c.h>
// #include "Adafruit_VL53L0X.h"

// // logic when not using USB
// #define HWUART // comment this if you want to connect to USBSerial
// #if defined(HWUART)
// #define Serial Serial1
// #endif


// int sensor1, sensor2, sensor3, sensor4, sensor5, sensor6;

// // objects for the vl53l0x
// Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
// Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
// Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
// Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();
// Adafruit_VL53L0X lox5 = Adafruit_VL53L0X();
// Adafruit_VL53L0X lox6 = Adafruit_VL53L0X();

// // this holds the measurement
// VL53L0X_RangingMeasurementData_t measure1;
// VL53L0X_RangingMeasurementData_t measure2;
// VL53L0X_RangingMeasurementData_t measure3;
// VL53L0X_RangingMeasurementData_t measure4;
// VL53L0X_RangingMeasurementData_t measure5;
// VL53L0X_RangingMeasurementData_t measure6;

// void startToFs()
// {   
    // TCA9548A(0);
    // if (!lox1.begin())
    // {
    //     Serial.println(F("Failed to boot VL53L0X 1"));
    //         ;
    // }

    // TCA9548A(1);
    // if (!lox2.begin())
    // {
    //     Serial.println(F("Failed to boot VL53L0X 2"));
    //         ;
    // }

    // TCA9548A(3);
    // if (!lox3.begin())
    // {
    //     Serial.println(F("Failed to boot VL53L0X 3"));
    //     while (1)
    //         ;
    // }

    // TCA9548A(4);
    // if (!lox4.begin())
    // {
    //     Serial.println(F("Failed to boot VL53L0X 4"));
    //     while (1)
    //         ;
    // }

    // TCA9548A(5);
    // if (!lox5.begin())
    // {
    //     Serial.println(F("Failed to boot VL53L0X 5"));
    //     while (1)
    //         ;
    // }

    // TCA9548A(6);
    // if (!lox6.begin())
    // {
    //     Serial.println(F("Failed to boot VL53L0X 6"));
    //     while (1)
    //         ;
    // }
// }

// void read_tof_sensors()
// {
    //int tof_readings[6];
    // TCA9548A(0);
    // lox1.rangingTest(&measure1, true);

    // if (measure1.RangeStatus != 4)
    // { // phase failures have incorrect data
    //     Serial.print("Distance (mm): ");
    //    // tof_readings[0] = measure1.RangeMilliMeter;
    //     Serial.println(measure1.RangeMilliMeter);
    // }
    // else
    // {
    //     Serial.println(" out of range ");
    // }

//     TCA9548A(1);
//     lox2.rangingTest(&measure2, false);

//     if (measure2.RangeStatus != 4)
//     { // phase failures have incorrect data
//         Serial.print("Distance (mm): ");
//        // tof_readings[1] = measure2.RangeMilliMeter;
//         Serial.println(measure2.RangeMilliMeter);
//     }
//     else
//     {
//         Serial.println(" out of range ");
//     }

// }
//     TCA9548A(3);
//     lox3.rangingTest(&measure3, false);

//     if (measure3.RangeStatus != 4)
//     { // phase failures have incorrect data
//         Serial.print("Distance (mm): ");
//         tof_readings[2] = measure3.RangeMilliMeter;
//         Serial.println(measure3.RangeMilliMeter);
//     }
//     else
//     {
//         Serial.println(" out of range ");
//     }
//     TCA9548A(4);
//     lox4.rangingTest(&measure4, false);

//     if (measure4.RangeStatus != 4)
//     { // phase failures have incorrect data
//         Serial.print("Distance (mm): ");
//         tof_readings[3] = measure4.RangeMilliMeter;
//         Serial.println(measure4.RangeMilliMeter);
//     }
//     else
//     {
//         Serial.println(" out of range ");
//     }
//     TCA9548A(5);
//     lox5.rangingTest(&measure1, false);

//     if (measure5.RangeStatus != 4)
//     { // phase failures have incorrect data
//         Serial.print("Distance (mm): ");
//         tof_readings[4] = measure5.RangeMilliMeter;
//         Serial.println(measure5.RangeMilliMeter);
//     }
//     else
//     {
//         Serial.println(" out of range ");
//     }
//     TCA9548A(6);
//     lox6.rangingTest(&measure1, false);

//     if (measure1.RangeStatus != 4)
//     { // phase failures have incorrect data
//         Serial.print("Distance (mm): ");
//         tof_readings[5] = measure6.RangeMilliMeter;
//         Serial.println(measure6.RangeMilliMeter);
//     }
//     else
//     {
//         Serial.println(" out of range ");
//     }
    
//     return tof_readings[6];
// }


// void setup()
// {
//   Serial.begin(9600);
//   Wire.begin();
//   delay(100);
//   startToFs();
// }

// void loop()
// {
//   read_tof_sensors();
//   delay(100);
// }

// #include <Arduino.h>
// #include <Servo.h>
// #include <color_sensor.h>
// #include <Wire.h>

// // logic when not using USB
// #define HWUART // comment this if you want to connect to USBSerial
// #if defined(HWUART)
// #define Serial Serial1
// #endif

// #define SER PB0
// // Servo myservo;  // create servo object to control a servo
// // // twelve servo objects can be created on most boards


// // void setup() {
// //   Serial.begin(9600);
// //   myservo.attach(PB0);  // attaches the servo on GIO2 to the servo object
// //   Serial.println("Hello");
// // }

// // void loop() {
// //     myservo.write(30);
// //     delay(1200);
// //   myservo.write(0);
// //   Serial.println("0");
// //   delay(1200);
// //   // myservo.write(30);
// //   // delay(1000);
// //   // myservo.write(60);
// //   // Serial.println("60");
// //   // delay(1000);
// //   // myservo.write(0);
// //   // delay(1000);
// //   // myservo.write(-30);
// //   // delay(1000);
// // }


// Servo myservo;  // create servo object to control a servo
// // twelve servo objects can be created on most boards


// void setup() {
//   pinMode(SER, OUTPUT);
//   myservo.attach(SER);  // attaches the servo on GIO2 to the servo object
// }

// void loop() {
//   int pos;
//   myservo.write(60);

//   // for (pos = 0; pos <= 20; pos += 1) { // goes from 0 degrees to 180 degrees
//   //   // in steps of 1 degree
//   //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
//   //   delay(15);                       // waits 15ms for the servo to reach the position
//   // }
//   // for (pos = 20; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//   //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
//   //   delay(15);                       // waits 15ms for the servo to reach the position
//   // }
// }