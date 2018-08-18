#include <Arduino.h>

#include "I2Cdev.h"
#include "MPU6050.h"

#define LED_PIN 13

MPU6050 imu;

int16_t forward, right, up;
int16_t rollright, pitchup, yawleft;

void setup() {
    Serial.begin(9600);

    Wire.begin();
    imu.initialize();
    if (imu.testConnection())
        Serial.println("Failed to connect to IMU!");

    // Set the digital low-pass filter mode to 5 (meaning a filter bandwidth of 10 Hz).
    imu.setDLPFMode(5);

    pinMode(LED_PIN, OUTPUT);

    Serial.println("fwd\tright\tup\tR-roll\tU-pitch\tR-yaw");
}

void loop() {
    imu.getMotion6(&right, &forward, &up, &pitchup, &rollright, &yawleft);

    digitalWrite(LED_PIN, yawleft > 0);

    Serial.print(forward);   Serial.print("\t");
    Serial.print(right);     Serial.print("\t");
    Serial.print(up);        Serial.print("\t");
    Serial.print(rollright); Serial.print("\t");
    Serial.print(pitchup);   Serial.print("\t");
    Serial.print(-yawleft);  Serial.print("\n");

    delay(100);
}