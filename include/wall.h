// int tof1, tof2, tof3, tof4, tof5;
// int detected_distance;

// void wall_follow()
// {   String scanDir;
//     read_tof_sensors();
//     tof1 = sensor1; tof2 = sensor2; tof3 = sensor3; tof4 = sensor4; tof5 = sensor5; //tof1 = sensor1; 
//     if ((tof1 < 100) or (tof2 < 100) or (tof3 < 100) or (tof4 < 100))
//     {
//         brake_fast();
//         delay(1000);
//          read_tof_sensors();
//          tof1 = sensor1; tof2 = sensor2; tof3 = sensor3; tof4 = sensor4; tof5 = sensor5; //tof1 = sensor1; 
//         //readTof
//         if ((tof1 < tof2) or (tof2 < tof3) or (tof3 < tof4)){
//             scanLeft(3);
//             scanDir = "left"

//         }
//         else if ((tof1 > tof2) or (tof2 > tof3) or (tof3 > tof4))
//         {
//             scanRight(3);
//             scanDir = "Right"
//         }
//         //
//         while (tof5 < 200){
//             if (tof5 < 80){
//                 scanLeft(1);
//                 delay(10);
//             }
//             else if (tof5 > 80){
//                 scanRight(1);
//                 delay(10);
//             }
//             go_cms(4);
//         }
//         while(digitalRead(IR3) == 0)
//         {
//             set_forward();
//             set_speed(80,80);
//         }
//         brake_fast();
//         delay(1000);
//         if (scanDir == "left") {
//             scanLeft(3);
//         } else if (scanDir == "right"){
//             scanRight(3);
//         }
//     }
//     else{
//         line_following_only();
//         delay(80);
//         brake_fast();
//         delay(200);
// }
// }

