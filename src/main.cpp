#include <motor.h>
// #include <Arduino.h>
void line_following();

void setup()
{
  setup_pins();
  attachInterrupt(digitalPinToInterrupt(encoderInB), encIncrementB, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderInA), encIncrementA, RISING);
  Serial.begin(9600);
  // setup_display();
}

void loop()
{
  // play_game();
  // Serial.println("rekkit: ");
  // Serial.println(rekkit);
  // Serial.println(digitalRead(USER_BTN));
  rekkit = (btn_hold(20)) ? true : rekkit ;
  if (rekkit)
  {
    switch (level)
    {
    case 0:
      line_following();
      break;
    default:
      brake_free();
      rekkit = false;
      level = 0;
      good_night();
      break;
    }
  }
  else blink(3, 300, 100, 1000);
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
    Serial.println("not on line");
    prevError = 0;
    integral = 0;
    derivative = 0;
    handle_edge_cases();
  }
  delay(20);
}
