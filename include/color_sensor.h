#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <string.h>

/* Connect SCL    to analog PB6
   Connect SDA    to analog PB7

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

String detect_color()
{
  if (tcs.begin())
  {
    Serial.println("Found sensor");
    float r, g, b;
    String color;
    tcs.getRGB(&r, &g, &b);
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
    while (1)
      ;
  }
}