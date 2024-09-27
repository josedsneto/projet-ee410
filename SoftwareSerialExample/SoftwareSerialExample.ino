#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float acceleration = sqrt(a.acceleration.x * a.acceleration.x +
                            a.acceleration.y * a.acceleration.y +
                            a.acceleration.z * a.acceleration.z);
                            
  if (acceleration < 2) { // Fall detection threshold (adjust this value)
    Serial.println("Fall Detected!");
    digitalWrite(LED_BUILTIN, HIGH);  // Or trigger buzzer
    delay(2000);  // Simple debounce to prevent multiple triggers
  } else {
    digitalWrite(LED_BUILTIN, LOW);   // No fall detected
  }

  delay(100);
}
