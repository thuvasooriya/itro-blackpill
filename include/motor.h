#include <helper.h>

int offset = 0;
int line_state = 0;
int prev_line_state = 0;
// int line_state_2 = 0;
// 5 - all_black
// 6 - all_white
// 7 - left
// 8 - right
// -1,0,1 - center
// ### speed and delays ###
int max_speed = 255;
int min_speed = 75;
int avg_speed = 125;
int lsp, rsp;
// counter measures
int cm_speed = 100;
int cm_delay = 20;
int cm_counter = 0;
int cm_max = 200;
int chk_point_t = 4;

int line_tracker = 0;

int trn90 = 5800;

int fa_10cm = 0;
int fb_10cm = 0;
int ra_10cm = 0;
int rb_10cm = 0;

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
bool lf_override = false;

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

void u_turn()
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

// void go_5cms(int i)
// {
//   countA = 0;
//   while (countA < (2000 * i))
//   {
//     set_forward();
//     set_speed(125, 120);
//   }
//   brake_fast();
// }

void go_cms(int i)
{
  countA = 0;
  while (countA < (400 * i))
  {
    set_forward();
    set_speed(125, 120);
  }
  brake_fast();
}

// void reverse_5cms(int i)
// {
//   countA = 0;
//   while (countA < (2000 * i))
//   {
//     set_back();
//     set_speed(125, 120);
//   }
//   brake_fast();
// }

void reverse_cms(int i)
{
  countA = 0;
  while (countA < (400 * i))
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

void calibrate_encoder()
{
  bool cali_status = false;
  set_forward();
  set_speed(100, 100);
  while (not cali_status)
  {

    countA = 0;
    countB = 0;
    if (allwhite())
    {
      set_speed(40, 40);
      while (allwhite())
      {
        fa_10cm = countA;
        fb_10cm = countB;
      }
    }
    if (fa_10cm != 0 and fb_10cm != 0)
    {
      cali_status = true;
    }
  }
  dev_log("fa_10cm", fa_10cm);
  dev_log("fb_10cm", fb_10cm);
}

// void checkpoint_crossing()
// {
//   while (allwhite())
//   {
//     set_forward();
//     set_speed(avg_speed, avg_speed);
//   }
//   int cttt = 0;
//   while (((get_deviation() < -4) or (get_deviation() > 4)) and (cttt < 5))
//   {
//     if (allwhite())
//     {
//       checkpoint_crossing();
//     }
//     scanLeft(1);
//     cttt++;
//   }
//   cttt = 0;
//   while (((get_deviation() < -4) or (get_deviation() > 4)) and (cttt < 10))
//   {
//     if (allwhite())
//     {
//       checkpoint_crossing();
//     }
//     scanLeft(1);
//     cttt++;
//   }
// }

float calc_pid(int e)
{
  logtxt("calculating pid...");
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
  logtxt("pid workin...");
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
      logtxt("ccw");
      set_ccw();
      analogWrite(PWMA, cm_speed);
      analogWrite(PWMB, cm_speed);
      delay(cm_delay);
    }
    else if (pos < 0)
    { // turnback when leaving line
      logtxt("cw");
      set_cw();
      analogWrite(PWMA, cm_speed);
      analogWrite(PWMB, cm_speed);
      delay(cm_delay);
    }
    else if (pos == 0)
    {
      logtxt("U");
      // dead end from line, do a 180
      u_turn();
    }
  }
  cm_counter++;
  // log("cm_counter: ", cm_counter);
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

void handle_checkpoint()
{
  if (level == 0)
  {
  }
  else
  {
  }
}

void detect_junction()
{
}

void handle_junction()
{
  if (level == 0)
  {
  }
  else
  {
  }
}

void handle_edge_cases()
{
  logtxt("handling edge");
  if (allblack())
  {
    counter_measures(offset);
  }
  else if (right_branch())
  {
    logtxt("right turn");
    // junction or right turn detected
    // countA = 0;
    // while (countA < 5400)
    // {
    //   set_forward();
    //   set_speed(125, 120);
    // }
    // brake_fast();

    // if (allblack())
    // {
    //   sharpRight3(avg_speed);
    // }

    cm_counter = 0;
  }
  else if (left_branch())
  {
    logtxt("left turn");
    // junction or left turn detected
    countA = 0;
    while (countA < 5400)
    {
      set_forward();
      set_speed(125, 120);
    }
    // brake_fast();

    if (allblack())
    {
      sharpLeft3(avg_speed);
    }
    cm_counter = 0;
  }
  // {
  //   logtxt("left turn");
  //   // left turn detected
  //   sharpLeft2(avg_speed);
  //   cm_counter = 0;
  // }
  else if (allwhite)
  {
    logtxt("tricky stuff");
    bool checkpoint = (verify_checkpoint()) ? true : false;
    // level = (checkpoint) ? level : level++;
    if (checkpoint)
    {
      checkpoint_count++;
      handle_checkpoint();
      // sharpLeft2(avg_speed);
    }
    else
    {
      junction_count++;
      handle_junction();
    }
  }
  handle_levels();
}
//////////////////////////////////////////////////////////////////
//////////////// LINE FOLLOWING MAIN FUNCTION ////////////////////
//////////////////////////////////////////////////////////////////
void line_following_revised()
{
  if (lf_override)
  {
    logtxt("override called");
  }
  else
  {
    bool skip_state = false;
    line_state = get_line_state();
    dev_log("line_state: ", line_state);

    // ##### black #####
    // #################
    if (line_state == 5)
    {
      // if (prev_line_state == 5) // still black
      // {
      //   logtxt("still black");
      //   // count to stop
      // }
      if (prev_line_state == 6) // white line ended
      {
        // junction logic
        line_tracker = countA;
        if (line_tracker > 2000)
        {
          // found a checkpoint
          logtxt("checkpoint");
          brake_fast();
          line_tracker = 0;
        }
        else if (line_tracker > 300)
        {
          // found a line
          // turn left
          logtxt("line");
          sharpLeft2(100);
          line_tracker = 0;
        }
      }
      else if (prev_line_state == 7) // left turn detection complete
      {
        // 90 degree left turn
        logtxt("left");
        sharpLeft2(100);
      }
      else if (prev_line_state == 8) // right turn detection complete
      {
        // 90 degree right turn
        logtxt("right");
        sharpRight2(100);
      }
      else if ((prev_line_state == -1) or (prev_line_state == 0) or (prev_line_state == 1))
      {
        // center line ended take uturn
        logtxt("u turn");
        u_turn();
      }
      else
      { // usual pwm missing line
        if (prev_line_state != 999)
        {
          if (prev_line_state > 0)
          { // turnback when leaving line
            logtxt("missed right");
            set_ccw();
            analogWrite(PWMA, cm_speed);
            analogWrite(PWMB, cm_speed);
            delay(cm_delay);
          }
          else if (prev_line_state < 0)
          { // turnback when leaving line
            logtxt("missed left");
            set_cw();
            analogWrite(PWMA, cm_speed);
            analogWrite(PWMB, cm_speed);
            delay(cm_delay);
          }
        }
      }
      cm_counter++;
      // log("cm_counter: ", cm_counter);
      level = (cm_counter > cm_max) ? 1 : 0;
    }

    // ##### white #####
    // #################
    else if (line_state == 6)
    {
      
      if (prev_line_state == 5) // white thing detected
      {
        logtxt("start white");
        set_forward();
        set_speed(avg_speed, avg_speed);
        if (line_tracker == 0)
        {
          line_tracker = 1;
          countA = 0;
          countB = 0;
        }
        else
        {
          line_tracker = countA;
        }
      }
      else if (prev_line_state == 6) // still white
      {
        logtxt("cont white");

        set_forward();
        set_speed(avg_speed, avg_speed);
        if (line_tracker == 0)
        {
          line_tracker = 1;
          countA = 0;
          countB = 0;
        }
        else
        {
          line_tracker = countA;
        }
      }
      // else if (prev_line_state == 7) // error ?
      // {
      //   // 90 degree left turn
      // }
      // else if (prev_line_state == 8) // error ?
      // {
      //   // 90 degree right turn
      // }
      else if ((prev_line_state == -1) or (prev_line_state == 0) or (prev_line_state == 1)) // center line ended
      {
        logtxt("t junction");
        set_forward();
        set_speed(avg_speed, avg_speed);
        if (line_tracker == 0)
        {
          line_tracker = 1;
          countA = 0;
          countB = 0;
        }
        else
        {
          line_tracker = countA;
        }
      }
      // else
      // {
      //   if (prev_line_state != 999)
      //   {
      //     if (prev_line_state > 0)
      //     {
      //     }
      //     else if (prev_line_state < 0)
      //     {
      //     }
      //   }
      //   // cm_counter++;
      //   // log("cm_counter: ", cm_counter);
      //   // level = (cm_counter > cm_max) ? 1 : 0;
      // }
      delay(cm_delay);
    }

    // ##### left #####
    // ################
    else if (line_state == 7)
    {
      
      if (prev_line_state == 5) // found a left line
      {
        set_forward();
        set_speed(avg_speed, avg_speed);
        if (line_tracker == 0)
        {
          line_tracker = 1;
          countA = 0;
          countB = 0;
        }
        else
        {
          line_tracker = countA;
        }
        // sharpLeft2(100);
      }
      else if (prev_line_state == 6) // error ?
      {
        // just go left :)
        sharpLeft2(100);
      }
      else if (prev_line_state == 7)
      {
        set_forward();
        set_speed(avg_speed, avg_speed);
        if (line_tracker == 0)
        {
          line_tracker = 1;
          countA = 0;
          countB = 0;
        }
        else
        {
          line_tracker = countA;
        }
      }
      else if (prev_line_state == 8) // error?
      {
      }
      else if ((prev_line_state == -1) or (prev_line_state == 0) or (prev_line_state == 1)) // left turn detected
      {
        set_forward();
        set_speed(avg_speed, avg_speed);
        if (line_tracker == 0)
        {
          line_tracker = 1;
          countA = 0;
          countB = 0;
        }
        else
        {
          line_tracker = countA;
        }
      }
      else
      {
        if (prev_line_state != 999)
        {
          if (prev_line_state > 0)
          {
          }
          else if (prev_line_state < 0)
          {
          }
        }
        // cm_counter++;
        // log("cm_counter: ", cm_counter);
        // level = (cm_counter > cm_max) ? 1 : 0;
      }
      delay(cm_delay);
    }

    // ##### right #####
    // #################
    else if (line_state == 8)
    {
      
      if (prev_line_state == 5) // found a right line
      {
      
        set_forward();
        set_speed(avg_speed, avg_speed);
        if (line_tracker == 0)
        {
          line_tracker = 1;
          countA = 0;
          countB = 0;
        }
        else
        {
          line_tracker = countA;
        }
        // sharpRight2(100);
      }
      else if (prev_line_state == 6) // error
      {
      }
      else if (prev_line_state == 7) // error ?
      {
      }
      else if (prev_line_state == 8)
      {
        set_forward();
        set_speed(avg_speed, avg_speed);
        if (line_tracker == 0)
        {
          line_tracker = 1;
          countA = 0;
          countB = 0;
        }
        else
        {
          line_tracker = countA;
        }
      }
      else if ((prev_line_state == -1) or (prev_line_state == 0) or (prev_line_state == 1)) // right turn detected
      {
        set_forward();
        set_speed(avg_speed, avg_speed);
        if (line_tracker == 0)
        {
          line_tracker = 1;
          countA = 0;
          countB = 0;
        }
        else
        {
          line_tracker = countA;
        }
      }
      else
      {
        if (prev_line_state != 999)
        {
          if (prev_line_state > 0)
          {
          }
          else if (prev_line_state < 0)
          {
          }
        }
        // cm_counter++;
        // log("cm_counter: ", cm_counter);
        // level = (cm_counter > cm_max) ? 1 : 0;
      }
      delay(cm_delay);
    }
    // ##### pwm #####
    // ###############
    else
    {
      if (not on_line)
      {
        prevError = 0;
        integral = 0;
        derivative = 0;
      }
      on_line = (line_state == (999 or 5 or 6 or 7 or 8)) ? false : true;
      if (on_line)
      {
        do_pid(calc_pid(line_state));
      }
    }
    prev_line_state = (skip_state) ? prev_line_state : line_state;
  }
}