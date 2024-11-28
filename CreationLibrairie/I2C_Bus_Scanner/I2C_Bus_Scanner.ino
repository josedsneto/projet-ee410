#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Scanning I2C...");
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    Serial.println(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found I2C device at address 0x");
      Serial.println(addr, HEX);
    }
  }
  Serial.println("End Check");
}

void loop() {
  Serial.println("Scanning I2C...");
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    Serial.println(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found I2C device at address 0x");
      Serial.println(addr, HEX);
    }
  }
  Serial.println("End Check");
  delay(10000);

}
