#include <motor.h>
// #include <Arduino.h>
void line_following();

void setup()
{
  setup_pins();
  attachInterrupt(digitalPinToInterrupt(encoderInB), encIncrementB, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderInA), encIncrementA, RISING);
  Serial.begin(9600);
  setup_display();
  logtxt("hi it's itro");
  logtxt("let's begin...");
  logtxt("hold the button");
}

void loop()
{
  if (not rekkit) {
    level_switcher();
  }
  // rekkit = (btn_hold(10)) ? true : rekkit;
  else if (rekkit)
  {
    dev_log("level: ", level);
    switch (level)
    {
    case 0:
      // line_following(); // change
      line_following_revised();
      break;
    case 1:
      // wall_following();
      break;
    case 2:
      // cross_bridge();
      break;
    case 3:
      // box pulling
      break;
    case 4:
      // 
    default:
      brake_free();
      rekkit = false;
      level = 0;
      good_night();
      break;
    }
  }
  
  else
  { // fukin brackets
    // log("rekkit: ", rekkit);
    blink(level+1 , 200, 50, 500);
  }
  // cleanup();
  // delay(100);
}

void line_following()
{
  offset = get_deviation();
  print_ir();
  on_line = (offset == 999) ? false : true;
  if (on_line)
    do_pid(calc_pid(offset));
  else
  {
    logtxt("not on line");
    prevError = 0;
    integral = 0;
    derivative = 0;
    handle_edge_cases();
  }
  delay(20);
}

// void wall_following()
// {

// }

// void cross_bridge()
// {

// }

