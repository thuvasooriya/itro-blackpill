#include <sound.h>
#include <math.h>
#include <guard_robot.h>


void line_following();
void line_following_only();
void pick_box();
void wall_follow();
void go_5cms_line(int i);
void avoid_guard_robot();
void detect_box();
void navigate_sound();
void reverse_line_following();
void ramp();


String path_color = "";
unsigned long time;

bool can_go = false;
unsigned long ex_millis_1 = 0;
unsigned long time1;
int box_detect_count = 0;
bool is_box_detected = false;

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

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(encoderInA, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderInA), encIncrementA, RISING);
  pinMode(encoderInB, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoderInB), encIncrementB, RISING);

  rot_servo.attach(SER_R); // attaches the servo on GIO2 to the servo object
  rot_servo.write(init_rot_pos);
  lin_servo.attach(SER_L);
  lin_servo.write(upper_pos);

  Serial.begin(9600);
  brake_fast();
  Wire.begin();
 // startToFs();
  //   if (!lox.begin())
  // {
  //     logtxt(F("Failed to boot VL53L0X 1"));
  //     _blink(5, 300, 100, 1000);
  // }
}

void loop()
{
  // logtxt("rekkit: ");
  // logtxt(rekkit);
  // logtxt(digitalRead(USER_BTN));
  rekkit = (btn_hold(20)) ? true : rekkit;
  if (rekkit)
  {
    switch (level)
    {
    case 0:
      ramp();
        // logtxt(detect_path_color());
      // detect_box();
      // level = 2;
      // set_forward();
      // set_speed(125,125);
      // print_tof();
      // delay(400);
      // brake_fast();
      // print_tof();
      // delay(500);
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
      //   logtxt("line following end1");
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
  // logtxt(offset);
  on_line = (offset == 999) ? false : true;
  if (on_line)
    do_pid(calc_pid(offset));
  else
  {
    // logtxt("not on line");
    prevError = 0;
    integral = 0;
    derivative = 0;
    handle_edge_cases();
  }
  delay(20);
}

void reverse_line_following()
{
  offset = get_deviation();
  print_ir();
  on_line = (offset == 999) ? false : true;
  if (on_line)
    do_pid_reverse(calc_pid(offset));
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
  // countA = 0;
  // while (countA < 4000)
  // {
  //   line_following_only();
  // }
  go_cms(20);
  brake_fast();
  delay(1000);
  align_center();
  path_color = detect_path_color();
  int ctt = 0;
  while ((path_color != "red") and (path_color != "blue") and (ctt < 5))
  {
    scanLeft(1, 100);
    path_color = detect_path_color();
    ctt++;
  }
  align_center();
  ctt = 0;
  while ((path_color != "red") and (path_color != "blue") and (ctt < 5))
  {
    scanRight(1, 300);
    path_color = detect_path_color();
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
  // int cctt = 0;
  // while (cctt < 5)
  // {
  //   scanLeft(1, 100);
  //   if (cctt == 3)
  //   {
  //     break;
  //   }
  //   cctt++;
  // }
  // go_cms(10);
  // lift_bok();
  // detect_box_color();
  detect_box();
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
  while (not(verify_checkpoint))
  {
    line_following();
  }
  drop_box(8);
  delay(1000);
  sharpLeft2(avg_speed);
  while(allwhite())
  {
    set_forward();
    set_speed(125,125);
  }
  brake_fast;
  delay(1000);
}

void wall_follow()
{
  String scanDir;
  logtxt("wall check");
  read_tof_sensors();
  logtxt("finished reading");
  tof1 = sensor1;
  tof2 = sensor2;
  tof3 = sensor3;
  tof4 = sensor4;
  tof5 = sensor5; // tof1 = sensor1;
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
  logtxt(" ");
  if ((tof1 < 100) or (tof2 < 100) or (tof3 < 100) or (tof4 < 100))
  {
    logtxt("Inside first brake ");
    brake_fast();
    delay(1000);
    read_tof_sensors();
    tof1 = sensor1;
    tof2 = sensor2;
    tof3 = sensor3;
    tof4 = sensor4;
    tof5 = sensor5; // tof1 = sensor1;
    logtxt("Second Read Finished");
    // readTof
    if ((tof1 < tof2) or (tof2 < tof3) or (tof3 < tof4))
    {
      logtxt("ifff 1");
      // scanLeft(3);
      scanDir = "left";
      logtxt("ScaLe");
    }
    else if ((tof1 > tof2) or (tof2 > tof3) or (tof3 > tof4))
    {
      logtxt("ifff 2");
      //  scanRight(3);
      scanDir = "Right";
      logtxt("ScaRight");
    }
    //
    logtxt("Third");
    if (scanDir == "left")
    {
      logtxt("sec left if");
      if (tof5 < 80)
      {
        // scanLeft(1);
        delay(10);
      }
      else if (tof5 > 80)
      {
        //  scanRight(1);
        delay(10);
      }
      //  go_cms(4);
    }
    while (digitalRead(IR3) == 0)
    {
      set_forward();
      set_speed(80, 80);
    }
    brake_fast();
    delay(1000);
    // scanLeft(3);
  }
  else if (scanDir == "right")
  {
    if (tof5 < 80)
    {
      //  scanRight(1);
      delay(10);
    }
    else if (tof5 > 80)
    {
      //  scanLeft(1);
      delay(10);
      go_cms(4);
    }
    while (digitalRead(IR3) == 0)
    {
      set_forward();
      set_speed(80, 80);
    }
    brake_fast();
    delay(1000);
    // scanRight(3);
  }
}

void navigate_sound()
{
  ex_millis = millis();
  while(not(right_branch()))
  {
    time = millis();
    if ((time - ex_millis) > 100){
      if (soundLevel() > 200){
        brake_fast();
        delay(100);
      }
    }
    line_following_only();
    ex_millis = time;
  }
  sharpRight2(125);
  ex_millis = millis();
  while(not(verify_checkpoint()))
  {
    time = millis();
    if ((time - ex_millis) > 100){
      if (soundLevel() > 200){
        brake_fast();
        delay(100);
      }
    }
    line_following_only();
    ex_millis = time;
  }
  brake_fast();
  delay(1000);
  while(allwhite())
  {
    set_forward();
    set_speed(125,125);
  }
  brake_fast();
  delay(200);
}



void detect_box()
{
     int scanTimes = 1;
     while (not(is_box_detected))
     {
          read_tof_sensors();
          while ((sensor2 > 400) and (sensor3 > 400))
          {
               scanLeft(1, 100);
               read_tof_sensors();
               scanTimes++;
               if (scanTimes > 4)
               {
                    break;
               }
          }
          while ((sensor2 > 400) or (sensor3 > 400))
          {
               scanRight(1, 100);
               read_tof_sensors();
               scanTimes++;
               if (scanTimes > 8)
               {
                    break;
               }
          }
          while ((sensor2 > 400) or (sensor3 > 400))
          {
               scanLeft(1, 100);
               read_tof_sensors();
               scanTimes++;
               if (scanTimes > 4)
               {
                    break;
               }
               read_tof_sensors();
               int box_distance = min(sensor1, sensor2);
               if ((box_distance > 400) and (box_detect_count <3))
               {
                    detect_box();
                    box_detect_count++;
               }
               box_distance = box_distance / 10;
               lift_box(box_distance + 5);
               is_box_detected = true;
          }
     }
}

void avoid_guard_robot()
{
    int c = 0;
    while (c < 3)
    {
        startToFs();
        delay(1000);
        c++;
    }
    while (not(can_go))
    {
        read_tof_sensors_guard();
        while ((sensor1 > 250) and (sensor4 > 250))
        {
            read_tof_sensors_guard();
        }
        if ((sensor1 > 250) and (sensor4 < 250))
        {
            ex_millis_1 = millis();
            while (sensor1 > 250)
            {
                read_tof_sensors_guard();
                time1 = millis();
                if ((time1 - ex_millis_1) > 1000)
                {
                    break;
                }
            }
            if (sensor1 < 250)
            {
                can_go = false;
                continue;
            }
            else
            {
                can_go = true;
                break;
            }
        }
        else if ((sensor1 < 250) and (sensor4 > 250))
        {
            ex_millis_1 = millis();
            while (sensor4 > 250)
            {
                read_tof_sensors_guard();
                time1 = millis();
                if ((time1 - ex_millis_1) > 1000)
                {
                    break;
                }
            }
            if (sensor4 < 250)
            {
                can_go = true;
                break;
            }
            else
            {
                can_go = false;
                continue;
            }
        }
        else
        {
            continue;
        }
    }
    if (can_go){
        go_cms(5);
        set_forward();
        set_speed(175, 175);
        avg_speed = 175;
        while(not(allwhite())){
            line_following_only();
        }
        brake_fast();
        sharpRight3(125);
        set_forward();
        set_speed(175, 175);
        avg_speed = 175;
        while(not(verify_checkpoint())){
            line_following_only();
        }
        brake_fast();
        //The End.....
    }
}

void ramp()
{
    // sharpRight3(125);
    // go_cms(5);
    // brake_fast();
    // delay(1000);
    set_forward();
    set_speed(85,85);

    while (not(allwhite()))
    {
        line_following_only();
    }
    brake_fast();
    delay(1000);
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
    delay(1000);
    level =1;
    sharpRight2(125);
    set_forward();
    set_speed(avg_speed, avg_speed);
    while (not(allwhite()))
    {
        line_following_only();
    }
    brake_fast();
    delay(500);
    pull_box(13);
    while (allwhite())
    {
        set_back();
        set_speed(125,125);
    }
    brake_fast();
    delay(500);
    reverse_cms(60);
    brake_fast;
    delay(500);
    go_cms(10);
    release_box();
    reverse_cms(5);
    move_arm(upper_pos);
    delay(500);
    open_arm(init_rot_pos);
    delay(500);
    sharpRight3(125);
    set_forward();
    set_speed(avg_speed, avg_speed);
    while (not(allwhite()))
    {
        line_following();
    }
    brake_fast();
    while(allwhite())
    {
    set_forward();
    set_speed(avg_speed, avg_speed);
    }
    brake_fast();
    delay(500);
    sharpLeft3(125);
    while (allwhite())
    {
        set_forward();
        set_speed(avg_speed, avg_speed);
    }
    brake_fast();
    delay(500);
    align_center();
    delay(500);
    set_forward();
    set_speed(avg_speed, avg_speed);
    while (not(allwhite()))
    {
        line_following();
    }
    brake_fast();
    delay(500);
    while(allwhite())
    {
    set_forward();
    set_speed(avg_speed, avg_speed);
    }
    brake_fast();
    delay(500);
    sharpRight3(125);
    while (allwhite())
    {
        set_forward();
        set_speed(avg_speed, avg_speed);
    }
    brake_fast();
    delay(500);
    align_center();
    delay(500);
    // set_forward();
    // set_speed(avg_speed, avg_speed);
    // while (not(verify_checkpoint()))
    // {
    //     line_following_only();
    // }
    // brake_fast();
    // delay(1000);
}
