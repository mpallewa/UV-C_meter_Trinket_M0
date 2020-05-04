/* 
  This code is released under the MIT License.
  
  Copyright 2020 Methlal Pallewatta

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
  and associated documentation files (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial 
  portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
  LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <Wire.h>
#include <Adafruit_DotStar.h>
#include <SPI.h>
#define NUMPIXELS 1 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN   7
#define CLOCKPIN   8

Adafruit_DotStar strip = Adafruit_DotStar(
  NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

#define INTERVAL 1000

int ledPin = 13;

int sensorPinUV0 = A3;
int sensorPinUV1 = A4;

unsigned int sensorValueUV0, sensorValueUV1;
unsigned long lastRun = 0L, runCount = 0L, t;
double intensity0, intensity1, intensityAvg;

void setup() {
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn off Dot Star LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Turn off red LED
 
  analogReadResolution(12);
 
  Serial.begin(9600);
  while (!Serial && millis() < 5000) {
    delay(100);
  }

}

void loop() {
  /* The sensors are read approx. once every second. (INTERVAL = 1000) */
  if (((t = millis()) - lastRun) >= INTERVAL) {
    lastRun = t;
    Serial.print("Run = "); Serial.println(runCount);
    sensorValueUV0 = analogRead(sensorPinUV0);
    /*
     * With 12 bit ADC resolution and a ref, voltage of 3.3v.
     * 
     * intensity = (sensorValue*3.3/4096) / (4.3*10^7*0.04*0.00076) W/cm^2 = sensorValue * 0.6163 uW/cm^2
     * 
     * See Akiba's article https://hackerfarm.jp/2020/03/nukemeter/ for details.
     */
    intensity0 = sensorValueUV0 * 0.6163;
    /* Serial.print("UV0 RAW = "); Serial.println(sensorValueUV0); */
    Serial.print("UV0 intensity (uW/cm^2) = "); Serial.println(intensity0, 3);
    sensorValueUV1 = analogRead(sensorPinUV1);
    intensity1 = sensorValueUV1 * 0.6163;
    /* Serial.print("UV1 RAW = "); Serial.println(sensorValueUV1); */
    Serial.print("UV1 intensity (uW/cm^2) = "); Serial.println(intensity1, 3);
    intensityAvg = (intensity0 + intensity1)/2;
    Serial.print("UV avg. intensity (uW/cm^2) = "); Serial.println(intensityAvg, 3);
    Serial.println("********\n");
    
    runCount++;
  } else {
    delay(10);
  }
}
