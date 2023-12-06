int tof1, tof2, tof3, tof4;
int proximity;
bool is_box_detected = false;

bool detect_box()
{  
    while (not(is_box_detected))
    {   
        if (tof1 > proximity and  tof2 < proximity and tof3 < proximity and tof4 > proximity)
        {
                is_box_detected = true;
        }
        else if(tof1 < proximity and  tof2 > proximity and tof3 > proximity and tof4 > proximity)
        {
             //   scanLeft(2);
        }
        else if(tof1 < proximity and  tof2 < proximity and tof3 > proximity and tof4 > proximity)
        {
             //   scanLeft(1);
        }
        else if(tof1 > proximity and  tof2 > proximity and tof3 > proximity and tof4 < proximity)
        {
             //   scanRight(2);
        }
        else if(tof1 > proximity and  tof2 > proximity and tof3 < proximity and tof4 < proximity)
        {
             //   scanRight(1);
        }
        else{
            break;
        }
    }
    return is_box_detected;
}