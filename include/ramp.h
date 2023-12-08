// void ramp()
// {
//     while (allwhite())
//     {
//         set_forward();
//         set_speed(avg_speed, avg_speed);
//     }
//     brake_fast();
//     delay(1000);
//     set_forward();
//     set_speed(85, 85);
//     while (not(verify_checkpoint()))
//     {
//         line_following();
//     }
//     brake_fast();
//     delay(1000);
//     while (allwhite())
//     {
//         set_forward();
//         set_speed(avg_speed, avg_speed);
//     }
//     brake_fast();
//     delay(1000);
//     set_forward();
//     set_speed(avg_speed, avg_speed);
//     while (not(allwhite()))
//     {

//         line_following();
//     }
//     brake_fast();
//     delay(1000);
//     sharpRight2();
//     while (not(allwhite()))
//         set_forward();
//     set_speed(avg_speed, avg_speed);
//     {
//         line_following();
//     }
//     brake_fast();
//     delay(5000);
//     pull_box();
//     reverse_cms(55);
//     release_box();
//     sharpRight3();
//     set_forward();
//     set_speed(avg_speed, avg_speed);
//     while (not(allwhite()))
//     {
//         line_following();
//     }
//     brake_fast();
//     go_cms(7);
//     sharpLeft3();
//     set_forward();
//     set_speed(avg_speed, avg_speed);
//     while (not(allwhite()))
//     {
//         line_following();
//     }
//     brake_fast();
//     go_cms(7);
//     sharpRight3();
//     set_forward();
//     set_speed(avg_speed, avg_speed);
//     while (not(verify_checkpoint()))
//     {
//         line_following();
//     }
//     brake_fast();
// }