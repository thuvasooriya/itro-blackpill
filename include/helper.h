#include <Arduino.h>
#include <pins.h>
// #include <display.h>

// logic when not using USB
#define HWUART // comment this if you want to connect to USBSerial
#if defined(HWUART)
#define Serial Serial1
#endif

int level = 0;
bool rekkit = false;
unsigned long ex_millis = 0;
int blink_state = 0;

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
    digitalWrite(LED_BUILTIN, HIGH);
    delay(t1);
    digitalWrite(LED_BUILTIN, LOW);
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

void good_night()
{
  Serial.println("Bye Bye, Take Care");
  blink(10, 100, 50, 3000);
}

bool btn_hold(int t)
{
  int btn_val = digitalRead(USER_BTN);

  if (btn_val == 1)
    return false;
  else
  {
    Serial.println("btn_val: ");
    Serial.print(btn_val);
    int i = 0;
    while (btn_val == 0)
    {
      i++;
      delay(20);
      // blink(20);
      if (i > t)
        break;
    }
    return true;
  }
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

