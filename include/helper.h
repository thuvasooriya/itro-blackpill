#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <pins.h>
#include <display.h>
#include <levels.h>

// logic when not using USB
#define HWUART // comment this if you want to connect to USBSerial
#if defined(HWUART)
#define Serial Serial1
#endif

bool dev_mode = true;

// unsigned long ex_millis = 0;
// int blink_state = 0;

// void blink(int t)
// {
//   unsigned long time = millis();
//   if (time - ex_millis >= t)
//   {
//     // save the last time you blinked the LED
//     ex_millis = time;
//     // if the LED is off turn it on and vice-versa:
//     blink_state = not blink_state;
//     // set the LED with the blink_state of the variable:
//     digitalWrite(LED_BUILTIN, blink_state);
//   }
// }

void blink(int x, int t1, int t2, int t3)
{
  for (int i = 1; i <= x; ++i)
  {
    digitalWrite(LED_BUILTIN, LOW);
    delay(t1);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(t2);
  }
  delay(t3);
}

void print_ir()
{
  Serial.print(digitalRead(IR1));
  Serial.print("::");
  Serial.print(digitalRead(IR2));
  Serial.print("::");
  Serial.print(digitalRead(IR3));
  Serial.print("::");
  Serial.print(digitalRead(IR4));
  Serial.print("::");
  Serial.println(digitalRead(IR5));
}

void reset_display()
{
  tft.setCursor(0, 0);
  tft.fillScreen(BLACK);
}

void dev_log(const char txt[], int x)
{
  Serial.print(txt);
  Serial.println(x);
  tft.startWrite();
  tft.print(txt);
  tft.println(x);
  tft.endWrite();
}
void logtxt(const char txt[])
{
  if (not dev_mode)
    return;
  Serial.println(txt);
  tft.startWrite();
  tft.println(txt);
  tft.endWrite();
}

void good_night()
{
  logtxt("byeee");
  blink(10, 100, 50, 3000);
}

bool btn_hold(int t)
{
  int btn_val = digitalRead(USER_BTN);
  if (btn_val == 1)
    return false;
  else
  {
    // black pill has a pull up switch
    logtxt("btn pressed");
    // dwrite("btn pressed");
    int i = 0;
    while (btn_val == 0)
    {
      i++;
      delay(20);
      // blink(20);
      if (i > t)
        break;
    }
    // blink(3, 100, 100, 1000);
    return true;
  }
}

int btn_hold_length()
{
  int btn_val = digitalRead(USER_BTN);
  // log("btn val: ",btn_val);
  // if (btn_val == 1)
  //   return false;
  // else
  if (digitalRead(USER_BTN) == 1)
    return 0;
  else
  {
    // black pill has a pull up switch
    int i = 0;
    while (digitalRead(USER_BTN) == 0)
    {
      i++;
      dev_log("btn_t: ", i);
      delay(100);
      // blink(20);
    }
    // blink(3, 100, 100, 1000);
    dev_log("held len: ",i );
    return i;
  }
}

void level_switcher()
{
  logtxt("short press to nxt lvl");
  logtxt("long press to start");

  delay(500);
  int val = btn_hold_length();
  // log("val: ",val);
  if (val < 5 and val > 0)
  {
    if (level < max_level)
    {
      level++;
    }
    else
    {
      level = 0;
    }
  }
  else if (val > 5)
  {
    logtxt("starting in ... 3");
    logtxt("reset to start over");
    delay(500);
    logtxt("starting in ... 2");
    delay(500);
    logtxt("starting in ... 1");
    delay(500);
    rekkit = true;
  }
  dev_log("lvl: ", level);
}

bool allblack()
{
  return ((digitalRead(IR1) == 0) and (digitalRead(IR2) == 0) and (digitalRead(IR3) == 0) and (digitalRead(IR4) == 0) and (digitalRead(IR5) == 0)) ? true : false;
}

bool allwhite()
{
  return ((digitalRead(IR1) == 1) and (digitalRead(IR2) == 1) and (digitalRead(IR3) == 1) and (digitalRead(IR4) == 1) and (digitalRead(IR5) == 1)) ? true : false;
}

bool right_branch()
{
  return (((digitalRead(IR1) == 0) and (digitalRead(IR2) == 0) and (digitalRead(IR3) == 1) and (digitalRead(IR4) == 1) and (digitalRead(IR5) == 1)) or ((digitalRead(IR1) == 0) and (digitalRead(IR2) == 1) and (digitalRead(IR3) == 1) and (digitalRead(IR4) == 1) and (digitalRead(IR5) == 1))) ? true : false;
}

bool left_branch()
{
  return (((digitalRead(IR1) == 1) and (digitalRead(IR2) == 1) and (digitalRead(IR3) == 1) and (digitalRead(IR4) == 0) and (digitalRead(IR5) == 0)) or ((digitalRead(IR1) == 1) and (digitalRead(IR2) == 1) and (digitalRead(IR3) == 1) and (digitalRead(IR4) == 1) and (digitalRead(IR5) == 0))) ? true : false;
}

int get_deviation()
{
  int r1 = digitalRead(IR1);
  int r2 = digitalRead(IR2);
  int r3 = digitalRead(IR3);
  int r4 = digitalRead(IR4);
  int r5 = digitalRead(IR5);
  int deviation = 999;

  if (r1 == 0 and r2 == 0 and r3 == 0 and r4 == 0 and r5 == 1)
  {
    deviation = -4;
  }
  else if (r1 == 0 and r2 == 0 and r3 == 0 and r4 == 1 and r5 == 1)
  {
    deviation = -3;
  }
  else if (r1 == 0 and r2 == 0 and r3 == 0 and r4 == 1 and r5 == 0)
  {
    deviation = -2;
  }
  else if (r1 == 0 and r2 == 0 and r3 == 1 and r4 == 1 and r5 == 0)
  {
    deviation = -1;
  }
  else if (r1 == 0 and r2 == 0 and r3 == 1 and r4 == 0 and r5 == 0)
  {
    deviation = 0;
  }
  else if (r1 == 0 and r2 == 1 and r3 == 1 and r4 == 0 and r5 == 0)
  {
    deviation = 1;
  }
  else if (r1 == 0 and r2 == 1 and r3 == 0 and r4 == 0 and r5 == 0)
  {
    deviation = 2;
  }
  else if (r1 == 1 and r2 == 1 and r3 == 0 and r4 == 0 and r5 == 0)
  {
    deviation = 3;
  }
  else if (r1 == 1 and r2 == 0 and r3 == 0 and r4 == 0 and r5 == 0)
  {
    deviation = 4;
  }
  return deviation;
}

int get_line_state()
{
  int r1 = digitalRead(IR1);
  int r2 = digitalRead(IR2);
  int r3 = digitalRead(IR3);
  int r4 = digitalRead(IR4);
  int r5 = digitalRead(IR5);
  int state = 999;
  if (r1 == 0 and r2 == 0 and r3 == 0 and r4 == 0 and r5 == 1)
  {
    state = -4;
  }
  else if (r1 == 0 and r2 == 0 and r3 == 0 and r4 == 1 and r5 == 1)
  {
    state = -3;
  }
  else if (r1 == 0 and r2 == 0 and r3 == 0 and r4 == 1 and r5 == 0)
  {
    state = -2;
  }
  else if (r1 == 0 and r2 == 0 and r3 == 1 and r4 == 1 and r5 == 0)
  {
    state = -1;
  }
  else if (r1 == 0 and r2 == 0 and r3 == 1 and r4 == 0 and r5 == 0)
  {
    state = 0;
  }
  else if (r1 == 0 and r2 == 1 and r3 == 1 and r4 == 0 and r5 == 0)
  {
    state = 1;
  }
  else if (r1 == 0 and r2 == 1 and r3 == 0 and r4 == 0 and r5 == 0)
  {
    state = 2;
  }
  else if (r1 == 1 and r2 == 1 and r3 == 0 and r4 == 0 and r5 == 0)
  {
    state = 3;
  }
  else if (r1 == 1 and r2 == 0 and r3 == 0 and r4 == 0 and r5 == 0)
  {
    state = 4;
  }
  else if (r1 == 0 and r2 == 0 and r3 == 0 and r4 == 0 and r5 == 0)
  {
    // black
    state = 5;
  }
  else if (r1 == 1 and r2 == 1 and r3 == 1 and r4 == 1 and r5 == 1)
  {
    // white
    state = 6;
  }
  else if ((r1 == 1 and r2 == 1 and r3 == 1 and r4 == 0 and r5 == 0) or (r1 == 1 and r2 == 1 and r3 == 1 and r4 == 1 and r5 == 0))
  {
    // left
    state = 7;
  }
  else if ((r1 == 0 and r2 == 0 and r3 == 1 and r4 == 1 and r5 == 1) or (r1 == 0 and r2 == 1 and r3 == 1 and r4 == 1 and r5 == 1))
  {
    // right
    state = 8;
  }
  return state;
}

void cleanup()
{
  if (tft.getCursorY() >= 120)
    reset_display();
}
