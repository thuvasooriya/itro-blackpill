// void pwm_test()

// {
//   int8_t x = 3;
//   digitalWrite(AIN1, HIGH);
//   digitalWrite(AIN2, LOW);
//   digitalWrite(BIN1, HIGH);
//   digitalWrite(BIN2, LOW);
//   for (int i = 0; i <= 255; i++)
//   {
//     analogWrite(PWMA, i);
//     analogWrite(PWMB, i);
//     delay(x * 10);
//   }
//   for (int i = 255; i >= 0; i--)
//   {
//     analogWrite(PWMA, i);
//     analogWrite(PWMB, i);
//     delay(x * 10);
//   }
// }