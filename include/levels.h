#include <Arduino.h>

int level = 0;
int checkpoint_count = 0;
int junction_count = 0;
int turn_count = 0;
bool rekkit = false;
bool checkpoint = false;
int intersection_case = 0;
int max_level = 7;

void handle_levels()
{
    if (level == 0) // line following
    {
        level = (checkpoint) ? level++ : level;
    }
    else if (level == 1) // wall
    {
        level = (checkpoint) ? level++ : level;
    }
    else if (level == 2) // bridge
    {

    }
    else if (level == 3) // excavate
    {

    }
    else if (level == 4) // color box
    {

    }
    else if (level == 5) // far cry
    {

    }
    else if (level == 6) // bob is here
    {

    }
    else if (level == 7)
    {
        // success
        rekkit = 0;
    }
    else {
        // failed somewhere
        rekkit = 0;
    }
}


// c1 // lvl0
// line following
// c2 // lvl1
// wall
// c3 // lvl2
// bridge
// c4 // lvl3
// detect 2 paths
// go straight // turn right // find the box
// c5 // box in checkpoint
// pull it till right turn // go forward a bit // release the box
// turn right and go to check point 
// c4
// turn left // go to other check point 
// c5 // detect big box
// turn right 
// pass white line (optional)
// c6 // lvl4
// color box task
// c7 // find box // detect the color // pick the box
// go back to junction and find color line
// solve path
// c8 // drop the box in color box
// lvl5
// initiate cry mode
// c9 // lvl6
// guard robot detect going left
// c10

