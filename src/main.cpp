#include <Arduino.h>
#include <EEPROM.h>

#include "I2Cdev.h"
#include "MPU6050.h"

#define LED_PIN 13
#define SAMPLES_TO_STORE 170

struct imuRecord {
    uint8_t forward;
    uint8_t right;
    uint8_t up;
    uint8_t rollRight;
    uint8_t pitchUp;
    uint8_t yawRight;
};

MPU6050 imu;
unsigned long lastTime;

void setup() {
    Serial.begin(9600);

    Wire.begin();
    imu.initialize();
    if (!imu.testConnection())
        Serial.println("Failed to connect to IMU!");

    // Set the digital low-pass filter mode to 5 (meaning a filter bandwidth of 10 Hz).
    imu.setDLPFMode(5);

    pinMode(LED_PIN, OUTPUT);

    Serial.println("fwd\tright\tup\tR-roll\tU-pitch\tR-yaw");

    for (int readSampleIndex = 0; readSampleIndex < SAMPLES_TO_STORE; readSampleIndex++) {
        Serial.print(EEPROM.read(6 * readSampleIndex) - 128);     Serial.print("\t");
        Serial.print(EEPROM.read(6 * readSampleIndex + 1) - 128); Serial.print("\t");
        Serial.print(EEPROM.read(6 * readSampleIndex + 2) - 128); Serial.print("\t");
        Serial.print(EEPROM.read(6 * readSampleIndex + 3) - 128); Serial.print("\t");
        Serial.print(EEPROM.read(6 * readSampleIndex + 4) - 128); Serial.print("\t");
        Serial.print(EEPROM.read(6 * readSampleIndex + 5) - 128); Serial.print("\n");
    }

    lastTime = millis();
}

int16_t forward, right, up;
int16_t rollRight, pitchUp, yawLeft;

uint16_t sampleIndex = 0;

void loop() {
    imu.getMotion6(&right, &forward, &up, &pitchUp, &rollRight, &yawLeft);

    // digitalWrite(LED_PIN, yawLeft > 0);

    if (sampleIndex >= SAMPLES_TO_STORE) {
        digitalWrite(LED_PIN, 0);
    }
    else {
        EEPROM.write(6 * sampleIndex,     forward   / 256  + 128);
        EEPROM.write(6 * sampleIndex + 1, right     / 256  + 128);
        EEPROM.write(6 * sampleIndex + 2, up        / 256  + 128);
        EEPROM.write(6 * sampleIndex + 3, rollRight / 256  + 128);
        EEPROM.write(6 * sampleIndex + 4, pitchUp   / 256  + 128);
        EEPROM.write(6 * sampleIndex + 5, -(yawLeft / 256) + 128);

        digitalWrite(LED_PIN, sampleIndex % 2);

        sampleIndex++;
    }

    unsigned long currentTime = millis();
    unsigned long nextTime = lastTime + 100;

    if (currentTime < nextTime)
        delay (nextTime - currentTime);
    
    lastTime = nextTime;
}