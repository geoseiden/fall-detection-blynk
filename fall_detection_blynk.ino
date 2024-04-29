#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>

#define BLYNK_TEMPLATE_ID "{Your Blynk Template ID}"
#define BLYNK_TEMPLATE_NAME "{Your Blynk Template Name}"
#define BLYNK_AUTH_TOKEN "{Your Blynk Token}"

#define BLYNK_PRINT Serial

char auth[] = BLYNK_AUTH_TOKEN; // Your Blynk authentication token
char ssid[] = ""; // Your Wi-Fi SSID
char pass[] = ""; // Your Wi-Fi password

Adafruit_MPU6050 mpu;

#include <BlynkSimpleEsp32.h>

const float fallThreshold = 2.0; // Threshold for detecting a fall using accelerometer data
const float gyroThreshold = 5.0; // Threshold for detecting a fall using gyroscope data

bool fallDetected = false; // Flag to track if a fall has been detected

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(20);
  if (!mpu.begin()) {
    while (1) {
      delay(20);
    }
  }
}

void loop() {
  Blynk.begin(auth, ssid, pass);
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  float totalAcceleration = sqrt(pow(accel.acceleration.x, 2) + pow(accel.acceleration.y, 2) + pow(accel.acceleration.z, 2));
  float totalGyro = sqrt(pow(gyro.gyro.x, 2) + pow(gyro.gyro.y, 2) + pow(gyro.gyro.z, 2));

  if (totalAcceleration > fallThreshold && totalGyro > gyroThreshold && !fallDetected) {
    Serial.println("Fall detected!");
    Blynk.logEvent("fall");
    fallDetected = true;
  }
  else if (!(totalAcceleration > fallThreshold && totalGyro > gyroThreshold)) {
    fallDetected = false;
  }

  if (!fallDetected) {
    Serial.println("All good");
  }

  delay(1000);
  Blynk.run();
}
