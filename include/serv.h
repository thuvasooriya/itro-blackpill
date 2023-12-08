// #include <Arduino.h>
// #include <Servo.h>
// #include <pins.h>

// Servo rot_servo;
// Servo lin_servo;  // create servo object to control a servo
// // twelve servo objects can be created on most boards
// int init_rot_pos = 5;
// int init_lin_pos = 65;
// int arm_angle;
// int lower_pos = 150;
// int upper_pos = 10;

// void setup()
// {
//   rot_servo.attach(SER_R); // attaches the servo on GIO2 to the servo object
//   rot_servo.write(init_rot_pos);
//   lin_servo.attach(SER_L);
//   lin_servo.write(upper_pos);
// }

// void open_arm(int ang)
// {
//   rot_servo.write(ang);
// }

// void close_arm()
// {
//   rot_servo.write(init_rot_pos);
// }

// void move_arm(int angle)
// {
//   lin_servo.write(angle);
// }

// void lift_box()
// {
//   move_arm(lower_pos);
//   open_arm(40);
//   delay(2000);
//  // go_cms(5);
//   close_arm();
//   delay(200);
//  move_arm(upper_pos);

// }

// void drop_box()
// {
//   move_arm(lower_pos);
//   open_arm(45);
//   delay(1000);
//   //reverse_cms(5);
//   close_arm();
//   delay(200);
//   move_arm(upper_pos);
// }

// void pull_box()
// {
//   move_arm(lower_pos);
//   open_arm(30);
//  // go_cms(5);
//   close_arm();
// }

// void release_box()
// {
//   open_arm(45);
//   //reverse_cms(5);
//   close_arm();
//   move_arm(upper_pos);
// }
