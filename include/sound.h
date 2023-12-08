#include <Arduino.h>
#include <pins.h>

int sound_proximity = 300;
const int sampleWindow = 50; // 50mS Sample window width (20Hz)
unsigned int sample;

int soundLevel()
{
    unsigned long startMillis = millis(); // Sample Window Start
    unsigned int peakToPeak = 0;          // Pk to Pk Level
    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;

    // collect data for 50 mS
    while (millis() - startMillis < sampleWindow)
    {
        sample = analogRead(SND);
        if (sample < 1024) // Cast out spurious readings
        {
            if (sample > signalMax)
            {
                signalMax = sample; // Save just the MAX levels
            }
            else if (sample < signalMin)
            {
                signalMin = sample; // Save just the MIN levels
            }
        }
    }
    peakToPeak = signalMax - signalMin;
    // MAX – MIN = P-P Amplitude
    Serial.println(peakToPeak);
    return peakToPeak;
}
