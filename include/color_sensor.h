#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <string.h>
#include <tof_2.h>

/* Connect SCL    to analog PB6
   Connect SDA    to analog PB7

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
Adafruit_TCS34725 tcs_path = Adafruit_TCS34725();
Adafruit_TCS34725 tcs_color = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
// Adafruit_TCS34725 tcs_path = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

String detect_path_color()
{
  TCA9548A(6);
  String color;
  if (tcs_path.begin())
  {
    Serial.println("Found sensor");
    float r, g, b;
    tcs_path.getRGB(&r, &g, &b);
    if ((r > 120) and (r > b))
    {
      color = "red";
    }
    else if ((b > 1) or (b > r))
    {
      color = "blue";
    }
    return color;
  }
  else
  {
    Serial.println("No TCS34725 found ... check your connections");
    color = "not_detected";
    return color; // while (1) ;
  }
}

String detect_box_color()
{
  String color;
  TCA9548A(7);
  if (tcs_path.begin())
  {
    Serial.println("Found sensor");
    float r, g, b;
    tcs_path.getRGB(&r, &g, &b);
    if ((r > 120) and (r > b))
    {
      color = "red";
    }
    else if ((b > 1) or (b > r))
    {
      color = "blue";
    }
    return color;
  }
  else
  {
    Serial.println("No TCS34725 found ... check your connections");
    color = "not_detected";
    return color; // while (1) ;
  }
}