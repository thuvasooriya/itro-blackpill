#include <motor.h>
#include <color_sensor.h>
#include <sound.h>
#include <math.h>
#include <guard_robot.h>

void line_following();
void line_following_only();
void pick_box();
void wall_follow();
void go_5cms_line(int i);

String box_color = "";
String path_color = "";
unsigned long time;

// Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// // this holds the measurement
// VL53L0X_RangingMeasurementData_t measure;

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

rot_servo.attach(SER_R);  // attaches the servo on GIO2 to the servo object
rot_servo.write(init_rot_pos);
lin_servo.attach(SER_L);
lin_servo.write(upper_pos);

  Serial.begin(9600);
  brake_fast();
  Wire.begin();
  startToFs();
  //   if (!lox.begin())
  // {
  //     Serial.println(F("Failed to boot VL53L0X 1"));
  //     _blink(5, 300, 100, 1000);
  // }
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
      // detect_box();
      // level = 2;
      set_forward();
      set_speed(125,125);
      print_tof();
      delay(400);
      brake_fast();
      print_tof();
      delay(500);
      // line_following_only();
      // time = millis();
      // if (time - ex_millis >= 100)
      // {
      //   ex_millis = time;
      //   wall_follow();
      // }


    //  ex_millis = millis();
    //  while(true)
    //  {
    //     line_following();
    //     time = millis();
    //     if ((time - ex_millis) > 500){
    //       break;
    //     }

    //  }
    //   Serial.println("line following end1");
    //   brake_fast();
    //   delay(1000);
    //   wall_follow();

   //   pick_box();
  // pull_box(15);
  // level = 1;
      break;
    case 2:
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
  delay(20);
}

void line_following_only()
{
  offset = get_deviation();
  print_ir();
  on_line = (offset == 999) ? false : true;
  if (on_line)
    do_pid(calc_pid(offset));
  else
  {
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

void go_cms_line(int i)
{
  countA = 0;
  while (countA < (400 * i))
  {
    set_forward();
    set_speed(avg_speed, avg_speed);
    line_following();
  }
  brake_fast();
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
    scanLeft(1, 100);
    path_color = detect_color();
    ctt++;
  }
  align_center();
  ctt = 0;
  while ((path_color != "red") and (path_color != "blue") and (ctt < 5))
  {
    scanRight(1, 300);
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
    scanLeft(1, 100);
    if (cctt == 3)
    {
      break;
    }
    cctt++;
  }
  go_cms(10);
  // lift_bok();
  // detect_box_color();
  box_color = "red";
  reverse_cms(10);
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

void wall_follow()
{   String scanDir;
    Serial.println("wall check");
    read_tof_sensors();
    Serial.println("finished reading");
    tof1 = sensor1; tof2 = sensor2; tof3 = sensor3; tof4 = sensor4; tof5 = sensor5; //tof1 = sensor1; 
    // read_tof_sensors();
    Serial.print(tof1);
    Serial.print(" ");
    Serial.print(tof2);    
    Serial.print(" ");
    Serial.print(tof3);
    Serial.print(" ");
    Serial.print(tof4);
    Serial.print(" ");
    Serial.print(tof5);
    // Serial.print(" ");
    // Serial.print(sensor6);
    Serial.println(" ");
    if ((tof1 < 100) or (tof2 < 100) or (tof3 < 100) or (tof4 < 100))
    { 
        Serial.println("Inside first brake ");
        brake_fast();
        delay(1000);
        read_tof_sensors();
        tof1 = sensor1; tof2 = sensor2; tof3 = sensor3; tof4 = sensor4; tof5 = sensor5; //tof1 = sensor1; 
        Serial.println("Second Read Finished");
        //readTof
        if ((tof1 < tof2) or (tof2 < tof3) or (tof3 < tof4)){
        Serial.println("ifff 1");
           // scanLeft(3);
            scanDir = "left";
            Serial.println("ScaLe");

        }
        else if ((tof1 > tof2) or (tof2 > tof3) or (tof3 > tof4))
        {
        Serial.println("ifff 2");
          //  scanRight(3);
            scanDir = "Right";
                        Serial.println("ScaRight");
        }
        //
        Serial.println("Third");
        if (scanDir == "left") {
                                  Serial.println("sec left if");
            if (tof5 < 80){
               // scanLeft(1);
                delay(10);
            }
            else if (tof5 > 80){
              //  scanRight(1);
                delay(10);
            }
          //  go_cms(4);
        }
        while(digitalRead(IR3) == 0)
        {
            set_forward();
            set_speed(80,80);
        }
        brake_fast();
        delay(1000);
       // scanLeft(3);
        } else if (scanDir == "right"){
            if (tof5 < 80){
              //  scanRight(1);
                delay(10);
            }
            else if (tof5 > 80){
              //  scanLeft(1);
                delay(10);
            go_cms(4);
        }
        while(digitalRead(IR3) == 0)
        {
            set_forward();
            set_speed(80,80);
        }
        brake_fast();
        delay(1000);
            //scanRight(3);
        }
    }

void navigate_sound()
{
    while (soundLevel() < 200)
    {
        line_following();
    } 
}

int proximity;
bool is_box_detected = false;

bool detect_box()
{   int scanTimes = 0;
    while (not(is_box_detected))
    {   
      read_tof_sensors();
      while ((sensor2 > 300) or (sensor3 > 300))
      {
          scanLeft(1, 300);
          read_tof_sensors();
          scanTimes++;
          if (scanTimes > 1){
               break;
          }
      }
      scanTimes = 0;
          while ((sensor2 > 300) or (sensor3 > 300))
      {
          scanRight(1, 300);
          read_tof_sensors();
          scanTimes++;
          if (scanTimes > 3){
               break;
          }
      }
      scanTimes = 0;
          while ((sensor2 > 300) or (sensor3 >300))
      {
          scanLeft(1, 300);
          read_tof_sensors();
          scanTimes++;
          if (scanTimes > 1){
               break;
          }
    
      }
            read_tof_sensors();
          int box_distance = min(sensor1, sensor2);
          if (box_distance>300)
          {
               detect_box();
          }
          box_distance = floor(box_distance / 10);
          lift_box(box_distance + 5);
          is_box_detected = true;
          align_center();
      
    }
    return is_box_detected;
}