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

int trn90 = 5900;

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
  // else if (pos == 0)
  // {
  //   // dead end from line, do a 180
  //   brake_fast();
  //   analogWrite(PWMA, avg_speed);
  //   analogWrite(PWMB, avg_speed);
  //   delay(cm_delay * 2);
  //   countA = 0;
  //   while (countA < 6500)
  //   {
  //     digitalWrite(AIN1, LOW);
  //     digitalWrite(AIN2, HIGH);
  //     digitalWrite(BIN1, HIGH);
  //     digitalWrite(BIN2, LOW);
  //     analogWrite(PWMA, cm_speed);
  //     analogWrite(PWMB, cm_speed);
  //   }
  //   delay(cm_delay);
  // }
  }
  cm_counter ++;
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
    // junction or right turn detected
    set_forward();
    analogWrite(PWMA, cm_speed);
    analogWrite(PWMB, cm_speed);
    // brake_fast();
    // delay(200);
    // delay(225);
    // brake_fast();
    delay(cm_delay);
    if (allblack())
    {
      countA = 0;
      while (countA < trn90)
      {
        sharp_right(125);
      }
      brake_free();
      // delay(200);
    }
    cm_counter = 0;
  }
  else if (left_branch())
  {
  Serial.println("left branch");
    // left turn detected
    // brake_fast();
    // delay(200);
    set_forward();
    analogWrite(PWMA, 150);
    analogWrite(PWMB, 150);
    delay(cm_delay);
    // brake_fast();
    if (allblack())
    {
      countB = 0;
      while (countB < trn90)
      {
        sharp_left(125);
      }
      brake_free();
      // delay(200);
    }
    cm_counter = 0;
  }
  else if (allwhite)
  {
    level = (verify_checkpoint()) ? 1 : 0;

  }
}