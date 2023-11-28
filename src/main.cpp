#include <Arduino.h>

void setup()
{
  // put your setup code here, to run once:
  // int result = myFunction(2, 3);
  // initialize digital pin PC13 as an output.
  pinMode(PC13, OUTPUT);
  
}

// the loop function runs over and over again forever
void loop()
{
  // put your main code here, to run repeatedly:
  digitalWrite(PC13, HIGH); // turn the LED on (HIGH is the voltage level)

  delay(100); // wait for a second

  digitalWrite(PC13, LOW); // turn the LED off by making the voltage LOW

  delay(100); // wait for a second
}

// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// }
