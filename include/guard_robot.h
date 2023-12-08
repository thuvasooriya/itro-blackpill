#include <tof_2.h>
#include <Arduino.h>

bool can_go = false;
unsigned long ex_millis_1 = 0;
unsigned long time1;
void avoid_guard_robot()
{
    int c = 0;
    while (c < 3) {
        startToFs();
        delay(1000);
        c++;
    }
    while (not(can_go)){
    read_tof_sensors_guard();
    while ((sensor1 > 250) and (sensor4 > 250))
    {
         read_tof_sensors_guard();
    }
    if((sensor1 > 250) and (sensor4 < 250))
    {
        ex_millis_1 = millis();
        while(sensor1 > 250){
            read_tof_sensors_guard();
            time1 = millis();
            if ((time1 - ex_millis_1) > 1000)
            {
                break;
            }    
        }
        if (sensor1 < 250){
            can_go = false;
            continue;
        } else{
           can_go = true;
            break;
        }
    }
    else if((sensor1 <250) and (sensor4 > 250))
    {
        ex_millis_1 = millis();
        while(sensor4 > 250){
            read_tof_sensors_guard();
            time1 = millis();
            if ((time1 - ex_millis_1) > 1000)
            {
                break;
            }    
        }
        if (sensor4 < 250){
            can_go = true;
            break;
        } else{
           can_go = false;
        continue;
        }
    }
    else{
        continue;
    }

    }


}