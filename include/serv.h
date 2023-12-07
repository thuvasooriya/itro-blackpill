#include <Arduino.h>
#include <color_sensor.h>
#include <pins.h>
#include <Servo.h>
#define SER_L PA2
#define SER_R PA1
// logic when not using USB
#define HWUART // comment this if you want to connect to USBSerial
#if defined(HWUART)
#define Serial Serial1
#endif

Servo rot_servo;
Servo lin_servo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int init_rot_pos;
int init_lin_pos;
int arm_angle;
int lower_pos;
int upper_pos;

void setup() {
  rot_servo.attach(SER_R);  // attaches the servo on GIO2 to the servo object
  lin_servo.attach(SER_L);
  Serial.begin(9600);
}

void loop() {
  // int pos;

  // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
  // }

  Serial.println(rot_servo.read());
  delay(200);

  
}

void open_arm(int ang)
{
  rot_servo.write(ang);
}

void close_arm()
{
  rot_servo.write(init_rot_pos);
}

void move_arm(int angle)
{
  lin_servo.write(angle);
}

void lift_box()
{
  move_arm(lower_pos);
  open_arm(30);
 // go_cms(5);
  close_arm();
 move_arm(upper_pos);

}

void drop_box()
{
  move_arm(lower_pos);
  open_arm(45);
  //reverse_cms(5);
  close_arm();
  move_arm(upper_pos);
}

void pull_box()
{
  move_arm(lower_pos);
  open_arm(30);
 // go_cms(5);
  close_arm();
}

void release_box()
{
  open_arm(45);
  //reverse_cms(5);
  close_arm();
  move_arm(upper_pos);
}