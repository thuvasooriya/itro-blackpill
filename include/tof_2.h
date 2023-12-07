#include <i2c.h>
#include "Adafruit_VL53L0X.h"

int sensor1, sensor2, sensor3, sensor4, sensor5, sensor6;

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox5 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox6 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;
VL53L0X_RangingMeasurementData_t measure4;
VL53L0X_RangingMeasurementData_t measure5;
VL53L0X_RangingMeasurementData_t measure6;

void startToFs()
{
    TCA9548A(1);
    if (!lox1.begin())
    {
        Serial.println(F("Failed to boot VL53L0X 1"));
        _blink(5, 300, 100, 1000);
    }

    TCA9548A(2);
    if (!lox2.begin())
    {
        Serial.println(F("Failed to boot VL53L0X 2"));
        _blink(5, 300, 100, 1000);
    }

    TCA9548A(3);
    if (!lox3.begin())
    {
        Serial.println(F("Failed to boot VL53L0X 3"));
        _blink(5, 300, 100, 1000);
    }

    TCA9548A(4);
    if (!lox4.begin())
    {
        Serial.println(F("Failed to boot VL53L0X 4"));
        _blink(5, 300, 100, 1000);
    }

    TCA9548A(5);
    if (!lox5.begin())
    {
        Serial.println(F("Failed to boot VL53L0X 5"));
        _blink(5, 300, 100, 1000); 
    }

    TCA9548A(6);
    if (!lox6.begin())
    {
        Serial.println(F("Failed to boot VL53L0X 6"));
        _blink(5, 300, 100, 1000);
    }
}

void read_tof_sensors()
{
    TCA9548A(1);
    lox1.rangingTest(&measure1, false);

    if (measure1.RangeStatus != 4)
    { // phase failures have incorrect data
      //  Serial.print("Distance (mm): ");
        sensor1 =  measure1.RangeMilliMeter;
      //  Serial.println(measure1.RangeMilliMeter);
    }
    else
    {   
        sensor1 = 9999;
    }
    if (sensor1 > 9999){
        sensor1 = 9999;
    }

    TCA9548A(2);
    lox2.rangingTest(&measure2, false);

    if (measure2.RangeStatus != 4)
    { // phase failures have incorrect data
      //   Serial.print("Distance (mm): ");
        sensor2 =  measure2.RangeMilliMeter;
      //  Serial.println(measure2.RangeMilliMeter);
    }
    else
    {
        sensor2 = 9999;
    }

        if (sensor2 > 9999){
        sensor2 = 9999;
    }

    TCA9548A(3);
    lox3.rangingTest(&measure3, false);

    if (measure3.RangeStatus != 4)
    { // phase failures have incorrect data
     //   Serial.print("Distance (mm): ");
        sensor3 = measure3.RangeMilliMeter;
      //  Serial.println(measure3.RangeMilliMeter);
    }
    else
    {
        sensor3 = 9999;
    }

        if (sensor3 > 9999){
        sensor3 = 9999;
    }


    TCA9548A(4);
    lox4.rangingTest(&measure4, false);

    if (measure4.RangeStatus != 4)
    { // phase failures have incorrect data
      //  Serial.print("Distance (mm): ");
        sensor4 = measure4.RangeMilliMeter;
      //  Serial.println(measure4.RangeMilliMeter);
    }
    else
    {
        sensor4 = 9999;
    }

        if (sensor4 > 9999){
        sensor4 = 9999;
    }


    TCA9548A(5);
    lox5.rangingTest(&measure5, false);

    if (measure5.RangeStatus != 4)
    { // phase failures have incorrect data
     //   Serial.print("Distance (mm): ");
        sensor5 = measure5.RangeMilliMeter;
    //  Serial.println(measure5.RangeMilliMeter);
    }
    else
    {
        sensor5 = 9999;
    }

        if (sensor5 > 9999){
        sensor5 = 9999;
    }


    TCA9548A(6);
    lox6.rangingTest(&measure6, false);

    if (measure6.RangeStatus != 4)
    { // phase failures have incorrect data
        //Serial.print("Distance (mm): ");
        sensor6 = measure6.RangeMilliMeter;
        //Serial.println(measure6.RangeMilliMeter);
    }
    else
    {
        sensor6 = 9999;
    }

        if (sensor6 > 9999){
        sensor6 = 9999;
    }
}

    // read_tof_sensors();
    // Serial.print(sensor1);
    // Serial.print(" ");
    // Serial.print(sensor2);    
    // Serial.print(" ");
    // Serial.print(sensor3);
    // Serial.print(" ");
    // Serial.print(sensor4);
    // Serial.print(" ");
    // Serial.print(sensor5);
    // Serial.print(" ");
    // Serial.print(sensor6);
    // Serial.println(" ");
    // delay(200);


