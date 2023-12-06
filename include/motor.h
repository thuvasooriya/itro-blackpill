#include <helper.h>

int offset = 0;
// ### speed and delays ###
int max_speed = 255;
int min_speed = 75;
int avg_speed = 125;
int lsp, rsp;
// counter measures
int cm_speed = 100;
int cm_delay = 200;
int cm_counter = 0;
int cm_max = 200;
int chk_point_t = 4;

int leftCount, rightCount, prevLeftCount, prevRightCount;

int trn90 = 5800;

// ### pid variables ###
float kp = 8; // 1.2;
float ki = 0; // 0.7;
float kd = 1; // 0.5;

float pVal, iVal, dVal, error;
float prevError = 0;
float integral = 0;
float derivative = 0;

// ### encoder ###
volatile int countA = 0;
volatile int countB = 0;

// check
float pid_val;
bool on_line;

int junction_detection_count = 0;

void encIncrementA()
{
  countA++;
}

void encIncrementB()
{
  countB++;
}

void limit_pwm()
{
  lsp = (lsp > max_speed) ? max_speed : lsp;
  lsp = (lsp < min_speed) ? min_speed : lsp;
  rsp = (rsp > max_speed) ? max_speed : rsp;
  rsp = (rsp < min_speed) ? min_speed : rsp;
}

void set_forward()
{
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void set_back()
{
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void set_cw()
{
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void set_ccw()
{
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void set_speed(int spdA, int spdB)
{
  analogWrite(PWMA, spdA);
  analogWrite(PWMB, spdB);
}

void brake_fast()
{
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, HIGH);
  delay(20);
}

void brake_free()
{
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  delay(20);
}

void sharpRight(int spd)
{
  countA = 0;
  while (countA < 5000)
  {
    set_forward();
    set_speed(spd, 0);
  }
  brake_free();
}

void sharpLeft(int spd)
{
  countB = 0;
  while (countB < 2500)
  {
    set_forward();
    set_speed(0, spd);
  }
  brake_free();
}

void sharp_right(int spd)
{
  set_cw();
  set_speed(spd, 0);
}

void sharp_left(int spd)
{
  set_ccw();
  set_speed(0, spd);
}

void U_turn()
{
  countA = 0;
  while (countA < 2000)
  {
    set_forward();
    set_speed(avg_speed, avg_speed);
  }
  countA = 0;
  while (countA < 6700)
  {
    set_ccw();
    set_speed(avg_speed, avg_speed);
  }
  delay(cm_delay);
}

void align_center()
{
  while (digitalRead(IR3) != 1)
  {
    if (get_deviation() > 0)
    {
      countA = 0;
      while (countA < 100)
      {
        set_ccw();
        set_speed(100, 100);
      }
    }
    else if (get_deviation() < 0)
    {
      countA = 0;
      while (countA < 100)
      {
        set_cw();
        set_speed(100, 100);
      }
    }
  }
  brake_fast();
}

void scanLeft(int times)
{
  int ct = 0;
  brake_fast();
  set_forward();
  while (ct < times)
  {
    countA = 0;
    while (countA < 100)
    {
      set_ccw();
      set_speed(75, 75);
    }
    ct++;
  }
  brake_fast();
  delay(1000);
}

void scanRight(int times)
{
  int ct = 0;
  brake_fast();
  set_forward();
  while (ct < times)
  {
    countA = 0;
    while (countA < 100)
    {
      set_cw();
      set_speed(75, 75);
    }
    ct++;
  }
  brake_fast();
  delay(1000);
}

void scan(int scanTimes)
{
  scanLeft(scanTimes);
  delay(1000);
  scanRight(scanTimes);
  delay(1000);
  align_center();
  scanRight(scanTimes);
  delay(1000);
  scanLeft(scanTimes);
  delay(1000);
  align_center();
}

void go_5cms(int i)
{
  countA = 0;
  while (countA < (2000 * i))
  {
    set_forward();
    set_speed(125, 120);
  }
  brake_fast();
}

void reverse_5cms(int i)
{
  countA = 0;
  while (countA < (2000 * i))
  {
    set_back();
    set_speed(125, 120);
  }
  brake_fast();
}

void sharpLeft2(int spd)
{
  countA = 0;
  while (countA < 5400)
  {
    set_forward();
    set_speed(125, 120);
  }
  brake_fast();
  countA = 0;
  while (countA < 3900)
  {
    set_ccw();
    set_speed(spd, spd);
  }
  brake_free();
  delay(100);
}

void sharpRight2(int spd)
{
  countA = 0;
  while (countA < 5400)
  {
    set_forward();
    set_speed(125, 120);
  }
  brake_fast();
  countA = 0;
  while (countA < 4000)
  {
    set_cw();
    set_speed(spd, spd);
  }
  brake_free();
  delay(100);
}

void sharpLeft3(int spd)
{
  countA = 0;
  while (countA < 3900)
  {
    set_ccw();
    set_speed(spd, spd);
  }
  brake_fast();
  delay(100);
}

void sharpRight3(int spd)
{
  countA = 0;
  while (countA < 4000)
  {
    set_cw();
    set_speed(spd, spd);
  }
  brake_fast();
  delay(100);
}

void checkpoint_crossing()
{
  while (allwhite())
  {
    set_forward();
    set_speed(avg_speed, avg_speed);
  }
  int cttt = 0;
  while (((get_deviation() < -4) or (get_deviation() > 4)) and (cttt < 5))
  {
    if (allwhite())
    {
      checkpoint_crossing();
    }
    scanLeft(1);
    cttt++;
  }
  cttt = 0;
  while (((get_deviation() < -4) or (get_deviation() > 4)) and (cttt < 10))
  {
    if (allwhite())
    {
      checkpoint_crossing();
    }
    scanLeft(1);
    cttt++;
  }
}

float calc_pid(int e)
{
  Serial.println("calculating pid...");
  error = e * 4;
  pVal = kp * error;
  integral += error;
  // integral += error * dt;
  iVal = ki * integral;
  derivative = (error - prevError);
  // derivative = (error - prevError)*dt;
  dVal = kd * derivative;
  return pVal + iVal + dVal;
}

void do_pid(int pid)
{
  Serial.println("pid workin...");
  lsp = avg_speed - pid;
  rsp = avg_speed + pid;
  limit_pwm();
  set_forward();
  analogWrite(PWMA, lsp);
  analogWrite(PWMB, rsp);
}

void counter_measures(int pos)
{
  if (pos != 999)
  {
    if (pos > 0)
    { // turnback when leaving line
      Serial.print("cm: pos > 0");
      set_ccw();
      analogWrite(PWMA, cm_speed);
      analogWrite(PWMB, cm_speed);
      delay(cm_delay);
    }
    else if (pos < 0)
    { // turnback when leaving line
      Serial.print("cm: pos < 0");
      set_cw();
      analogWrite(PWMA, cm_speed);
      analogWrite(PWMB, cm_speed);
      delay(cm_delay);
    }
    else if (pos == 0)
    {
      // dead end from line, do a 180
      U_turn();
    }
  }
  cm_counter++;
  level = (cm_counter > cm_max) ? 1 : 0;
}

bool verify_checkpoint()
{
  int t = 0;
  while (allwhite())
  {
    set_forward();
    set_speed(100, 100);
    delay(200);
    t++;
  }
  return (t > chk_point_t) ? true : false;
}

void handle_edge_cases()
{
  // Serial.println("handling edge");
  if (allblack())
  {
    counter_measures(offset);
  }
  else if (right_branch())
  {
    Serial.println("right branch");
    // // junction or right turn detected
    // set_forward();
    // analogWrite(PWMA, cm_speed);
    // analogWrite(PWMB, cm_speed);
    // // brake_fast();
    // // delay(200);
    // // delay(225);
    // // brake_fast();
    // delay(cm_delay);
    // if (allblack())
    // {
    //   countA = 0;
    //   while (countA < trn90)
    //   {
    //     sharp_right(100);
    //   }
    //   brake_free();
    //   // delay(200);
    // }
    // else if(digitalRead(IR3==1)){
    //   set_forward();
    //   set_speed(avg_speed,avg_speed);
    //   delay(200);
    // }
    countA = 0;
    while (countA < 5400)
    {
      set_forward();
      set_speed(125, 120);
    }
    brake_fast();

    if (allblack())
    {
      sharpRight3(avg_speed);
    }
    cm_counter = 0;
  }
  else if (left_branch())
  {
    Serial.println("left branch");
    // left turn detected
    sharpLeft2(avg_speed);
    // brake_fast();
    // delay(200);
    // set_forward();
    // analogWrite(PWMA, 150);
    // analogWrite(PWMB, 150);
    // delay(cm_delay);
    // brake_fast();
    // if (allblack())
    // {
    //   countB = 0;
    //   while (countB < trn90)
    //   {
    //     sharp_left(125);
    //   }
    //   brake_free();
    //   // delay(200);
    // }
    // countB = 0;
    // while (countB < trn90)
    // {
    //   sharp_left(125);
    // }
    // brake_free();
    // delay(200);
    cm_counter = 0;
  }
  else if (allwhite)
  {
    bool junction = (verify_checkpoint()) ? false : true;
    level = (junction) ? level : level++;
    if (junction)
    {
      junction_detection_count++;
      sharpLeft2(avg_speed);
    }
  }
}


void driveStraight(int speed) {

  // use wheel encoders to drive straight continuously

  // amount to offset motor powers to drive straight
  int leftspeed, rightspeed = speed;
  int offset = 1;
  set_forward();
  set_speed(lsp,rsp);

  leftCount = countA;
  rightCount = countB;
  
  int leftDiff = leftCount - prevLeftCount;
  int rightDiff = rightCount - prevRightCount;

  prevLeftCount = leftCount;
  prevRightCount = rightCount;

  if (leftDiff > rightDiff) {
    leftspeed = leftspeed - offset;
    rightspeed = rightspeed + offset;
  }

  else if (leftDiff < rightDiff) {
    leftspeed = leftspeed +  offset;
    rightspeed = rightspeed- offset;
  }

  set_speed(leftspeed,rightspeed);
  delay(10);
}

void driveDistance(float distance) {


    int leftSpeed = 125;
    int rightSpeed = 125;
    set_forward();
    
    // amount to offset motor powers to drive straight
    int offset = 1;

    // if negative distance, make motor powers & offset also negative
    if (distance < 0) {
        set_back();
    }

    // variables for tracking wheel encoder counts
    leftCount = 0;
    rightCount = 0;
    prevLeftCount = 0;
    prevRightCount = 0;
    int leftDiff, rightDiff;

    int gear_ratio = 50;
    float countsPerRev = gear_ratio * 7;
    float wheelDiam = 6.4; //cm
    float wheelCirc = PI * wheelDiam; // wheel circumference = 3.14 x 2.56 in = 8.04 in


    // based on distance, calculate number of wheel revolutions
    float numRev = distance / wheelCirc;

    // calculate target encoder count
    float targetCount = numRev * countsPerRev;

    // reset encoder counters and start driving
    countA = 0;
    countB = 0;
    delay(100);
    set_speed(leftSpeed, rightSpeed);

    // keeps looping while right encoder count less than target count
    while (abs(rightCount) < abs(targetCount)) {

        // get current wheel encoder counts
        leftCount = countA;
        rightCount = countB;

        // calculate increase in count from previous reading
        leftDiff = abs(leftCount - prevLeftCount);
        rightDiff = abs(rightCount - prevRightCount);

        // store current counts as "previous" counts for next reading
        prevLeftCount = leftCount;
        prevRightCount = rightCount;

        // adjust left & right motor powers to keep counts similar (drive straight)

        // if left rotated more than right, slow down left & speed up right
        if (leftDiff > rightDiff) {
            leftSpeed = leftSpeed - offset;
            rightSpeed = rightSpeed + offset;
        }
        // else if right rotated more than left, speed up left & slow down right
        else if (leftDiff < rightDiff) {
            leftSpeed = leftSpeed + offset;
            rightSpeed = rightSpeed - offset;
        }
        set_speed(leftSpeed, rightSpeed);
        delay(10); 
    }

    brake_fast();
    delay(500);
}