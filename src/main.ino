#include <motor.h>
#include <color_sensor.h>
#include <tof_2.h>

void line_following();
void line_following_only();
void pick_box();
void wall_follow();
void go_5cms_line(int i);

String box_color = "";
String path_color = "";
int tof1, tof2, tof3, tof4, tof5;

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
  Wire.begin();
  startToFs();
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
    wall_follow();
      // pick_box();
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
    read_tof_sensors();
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
delay(200);
    if ((tof1 < 100) or (tof2 < 100) or (tof3 < 100) or (tof4 < 100))
    { 
        Serial.println("Inside first brake ");
        brake_fast();
        delay(1000);
         read_tof_sensors();
         tof1 = sensor1; tof2 = sensor2; tof3 = sensor3; tof4 = sensor4; tof5 = sensor5; //tof1 = sensor1; 
        //readTof
                Serial.println("Second Read");
        if ((tof1 < tof2) or (tof2 < tof3) or (tof3 < tof4)){
            scanLeft(3);
            scanDir = "left";
            Serial.println("ScaLe");

        }
        else if ((tof1 > tof2) or (tof2 > tof3) or (tof3 > tof4))
        {
            scanRight(3);
            scanDir = "Right";
                        Serial.println("ScaRight");
        }
        //
        if (scanDir == "left") {
                                  Serial.println("sec left if");
        while (tof5 < 200){
            if (tof5 < 80){
                scanLeft(1);
                delay(10);
            }
            else if (tof5 > 80){
                scanRight(1);
                delay(10);
            }
            go_cms(4);
        }
        while(digitalRead(IR3) == 0)
        {
            set_forward();
            set_speed(80,80);
        }
        brake_fast();
        delay(1000);
        scanLeft(3);
        } else if (scanDir == "right"){
                  while (tof5 < 200){
            if (tof5 < 80){
                scanRight(1);
                delay(10);
            }
            else if (tof5 > 80){
                scanLeft(1);
                delay(10);
            }
            go_cms(4);
        }
        while(digitalRead(IR3) == 0)
        {
            set_forward();
            set_speed(80,80);
        }
        brake_fast();
        delay(1000);
            scanRight(3);
        }
    }
    else{
        line_following_only();
        delay(200);
        brake_free();
        delay(200);
}
}