String box_color = "";
String path_color = "";
// bool is_box_detected = false;
// bool is_box_lifted = false;

void pick_box()
{
    while (allwhite())
    {
        set_forward();
        set_speed(avg_speed, avg_speed);
    }
    brake_fast();
    delay(1000);
    set_forward();
    set_speed(avg_speed, avg_speed);
    while (not(allwhite))
    {
        line_following_only();
    }
    break_fast();
    delay(1000);
    sharpLeft();
    countA = 0;
    set_forward();
    set_speed(avg_speed, avg_speed);
    while (countA < 3000)
    {
        line_following_only();
    }
    brake_fast();
    delay(1000);
    path_color = detect_color();
    if ((path_color == "red") or (path_color == "blue"))
    {
        U_turn();
    }
    else
    {
        break_fast();
        delay(5000);
        // while ((path_color != "red") and (path_color != "blue"))
        // scan();
        // path_color = detect_color();
    }
    countA = 0;
    set_forward();
    set_speed(avg_speed, avg_speed);
    while (countA < 3000)
    {
        line_following_only();
    }
    brake_fast();
    delay(100);
    sharpLeft(avg_speed);
    // scan();
    // lift_box();
    // box_color = detect_box_color();
    // reverse_motion();
    box_color = "red";
    if (path_color == box_color)
    {
        sharpLeft(avg_speed);
    }
    else
    {
        sharpRight(avg_speed);
    }
    brake_free();
    delay(2000);
}
