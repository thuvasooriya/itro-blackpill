// int proximity;
// bool is_box_detected = false;

// bool detect_box()
// {
//      int scanTimes = 1;
//      while (not(is_box_detected))
//      {
//           read_tof_sensors();
//           while ((sensor2 > 400) and (sensor3 > 400))
//           {
//                scanLeft(1);
//                read_tof_sensors();
//                scanTimes++;
//                if (scanTimes > 4)
//                {
//                     break;
//                }
//           }
//           while ((sensor2 > 400) or (sensor3 > 400))
//           {
//                scanRight(1);
//                read_tof_sensors();
//                scanTimes++;
//                if (scanTimes > 8)
//                {
//                     break;
//                }
//           }
//           while ((sensor2 > 400) or (sensor3 > 400))
//           {
//                scanLeft(1);
//                read_tof_sensors();
//                scanTimes++;
//                if (scanTimes > 4)
//                {
//                     break;
//                }
//                read_tof_sensors();
//                int box_distance = min(sensor1, sensor2);
//                if (box_distance > 400)
//                {
//                     detect_box();
//                }
//                box_distance = box_distance / 10;
//                lift_box(box_distance + 5);
//           }
//      }
//      return is_box_detected;
// }