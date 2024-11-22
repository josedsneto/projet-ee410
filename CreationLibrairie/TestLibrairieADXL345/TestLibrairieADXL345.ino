#include <Wire.h>


ADXL345 adxl; //variable adxl is an instance of the ADXL345 library
double xyz[3];
// Les valeurs de l'accélération suivant les différents axes
double ax, ay, az; 
double module;

// =========================Valeurs pour la librairie =========================
#define ADXL345_DEVICE (0x53)    // ADXL345 device address
#define ADXL345_TO_READ (6)      // num of bytes we are going to read each time (two bytes for each axis)

#define ADXL345_THRESH_ACT 0x24 // Registre du theresold activation
#define ADXL345_THRESH_INACT 0x25 // Registre du theresikd incativation
#define ADXL345_TIME_INACT 0x26

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // ===== adxl.powerOn(); // cf datasheet page 14
  Wire.begin();        // join i2c bus (address optional for master)
  //Turning on the ADXL345
  writeTo(ADXL345_POWER_CTL, 0); // Désactive tout
  writeTo(ADXL345_POWER_CTL, 16); // Met auto sleep
  writeTo(ADXL345_POWER_CTL, 8); // Met mesure


  // =====  //set activity/ inactivity thresholds (0-255)
  // ===== adxl.setActivityThreshold(75); //62.5mg per increment
  int temp_int = constrain(75, 0, 255); // Pour le borner entre 0 et 255
  writeTo(ADXL345_THRESH_ACT, byte(temp_int)); // Set le threshold activation
  // ===== adxl.setInactivityThreshold(75); //62.5mg per increment
  temp_int = constrain(75, 0, 255); // Pour le borner entre 0 et 255
  writeTo(ADXL345_THRESH_INACT, byte(temp_int)); // Set le threshold inactivation
  // ===== adxl.setTimeInactivity(10); // how many seconds of no activity is inactive?
  temp_int = constrain(10, 0, 255); 
  writeTo(ADXL345_TIME_INACT, byte(temp_int));







}

void loop() {
  // put your main code here, to run repeatedly:

}

void writeTo (byte address, byte val) {
    Wire.beginTransmission(ADXL345_DEVICE); // start transmission to device
    Wire.write(address);             // send register address
    Wire.write(val);                 // send value to write
    Wire.endTransmission();         // end transmission
}


