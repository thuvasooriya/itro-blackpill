#include <Arduino.h>
#include <pins.h>

long duration;
int distance;
const float sec_to_microsec = 1000000.0;
const float meter_to_cm = 100.0;
const float speed_of_sound = 340.0; // meters per second
int obstacle_proximity = 10;

int obstacle_distance()
{
    // Initiate triggerPin as LOW
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);

    // Set the triggerPin HIGH for atleast 10 microseconds
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    // Monitor the echoPin and
    duration = pulseIn(echoPin, HIGH); // Returns the length of the pulse in microseconds

    // Calculate distance and print on Serial Monitor
    distance = (duration / sec_to_microsec) * (speed_of_sound * meter_to_cm) / 2.0;
    Serial.print("Distance: ");
    Serial.println(distance);
    delay(500);
    return distance;
}

bool is_obstacle_ahead()
{
    if ((obstacle_distance() < obstacle_proximity))
    {
        return true;
    }
    return false;
}