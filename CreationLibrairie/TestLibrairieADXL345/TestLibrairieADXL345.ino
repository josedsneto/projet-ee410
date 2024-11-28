#include <Wire.h>


//ADXL345 adxl; //variable adxl is an instance of the ADXL345 library
double xyz[3];
// Les valeurs de l'accélération suivant les différents axes
double ax, ay, az; 
double module;

// =========================Valeurs pour la librairie =========================

byte _buff[6] ;    //6 bytes buffer for saving data read from the device
double gains[3];        // counts to Gs
bool status;           // set when error occurs
// see error code for details
byte error_code;       // Initial state

#define ADXL345_DEVICE (0x53)    // ADXL345 device address
#define ADXL345_TO_READ (6)      // num of bytes we are going to read each time (two bytes for each axis)

#define ADXL345_OK    1 // no error
#define ADXL345_ERROR 0 // indicates error is present

#define ADXL345_NO_ERROR   0 // initial state
#define ADXL345_READ_ERROR 1 // problem reading accel
#define ADXL345_BAD_ARG    2 // bad method argument

#define ADXL345_THRESH_ACT 0x24 // Registre du theresold activation
#define ADXL345_THRESH_INACT 0x25 // Registre du theresikd incativation
#define ADXL345_TIME_INACT 0x26
#define ADXL345_ACT_INACT_CTL 0x27 // Action inaction registre
#define ADXL345_DATAX0 0x32
#define ADXL345_POWER_CTL 0x2d
#define ADXL345_DATA_FORMAT 0x31

int count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // //for 2g
  // gains[0] = 0.00376390; // cf datasheet, ceci peut être calculé !!! TODO***
  // gains[1] = 0.00376009;
  // gains[2] = 0.00349265;
  // for 8g
  gains[0] = 0.016129; // cf datasheet, ceci peut être calculé !!! TODO***
  gains[1] = 0.016393;
  gains[2] = 0.0169491;

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



  // //look of activity movement on this axes - 1 == on; 0 == off
  // adxl.setActivityX(1);
  setRegisterBit(ADXL345_ACT_INACT_CTL, 6, 1);
  // adxl.setActivityY(1);
  setRegisterBit(ADXL345_ACT_INACT_CTL, 5, 1);
  // adxl.setActivityZ(1);
  setRegisterBit(ADXL345_ACT_INACT_CTL, 4, 1);



  // Try changing the range to +- 8
  setRegisterBit(ADXL345_DATA_FORMAT, 1, 1);





}

void loop() {
  // put your main code here, to run repeatedly:
  // adxl.getAcceleration(xyz);
  getAcceleration(xyz);
  getAccelerometerValues();
  module = sqrt(ax*ax + ay*ay + az*az); // We calculate de magnitude
  printAccelerationAndModule();
  delay(100); 

  count++;
  if (count > 10) {// every seconds
  count = 0;
  readFrom(ADXL345_DATA_FORMAT,  1, _buff);
  Serial.println(_buff[0]);
  }
}

void writeTo (byte address, byte val) {
    Wire.beginTransmission(ADXL345_DEVICE); // start transmission to device
    Wire.write(address);             // send register address
    Wire.write(val);                 // send value to write
    Wire.endTransmission();         // end transmission
}

bool getRegisterBit(byte regAdress, int bitPos) {
    byte _b;
    readFrom(regAdress, 1, &_b);
    return ((_b >> bitPos) & 1);
}

void setRegisterBit(byte regAdress, int bitPos, bool state) {
    byte _b;
    readFrom(regAdress, 1, &_b);
    if (state) {
        _b |= (1 << bitPos);  // forces nth bit of _b to be 1.  all other bits left alone.
    } else {
        _b &= ~(1 << bitPos); // forces nth bit of _b to be 0.  all other bits left alone.
    }
    writeTo(regAdress, _b);
}


void getAcceleration(double* xyz) {
    int i;
    int xyz_int[3];
    readAccel(xyz_int);
    for (i = 0; i < 3; i++) {
        xyz[i] = xyz_int[i] * gains[i];
    }
}

// Reads the acceleration into three variable x, y and z
void readAccel(int* xyz) {
    readXYZ(xyz, xyz + 1, xyz + 2);
}
void readXYZ(int* x, int* y, int* z) {
    readFrom(ADXL345_DATAX0, ADXL345_TO_READ, _buff); //read the acceleration data from the ADXL345
    *x = (short)((((unsigned short)_buff[1]) << 8) | _buff[0]);
    *y = (short)((((unsigned short)_buff[3]) << 8) | _buff[2]);
    *z = (short)((((unsigned short)_buff[5]) << 8) | _buff[4]);
}

// Reads num bytes starting from address register on device in to _buff array
void readFrom(byte address, int num, byte _buff[]) {
    Wire.beginTransmission(ADXL345_DEVICE); // start transmission to device
    Wire.write(address);             // sends address to read from
    Wire.endTransmission();         // end transmission

    Wire.beginTransmission(ADXL345_DEVICE); // start transmission to device
    Wire.requestFrom(ADXL345_DEVICE, num);    // request 6 bytes from device

    int i = 0;
    while (Wire.available()) {      // device may send less than requested (abnormal)
        _buff[i] = Wire.read();    // receive a byte
        i++;
    }
    if (i != num) {
        status = ADXL345_ERROR;
        error_code = ADXL345_READ_ERROR;
    }
    Wire.endTransmission();         // end transmission
}


void getAccelerometerValues() {
  getAcceleration(xyz);
  ax = xyz[0]; ay = xyz[1]; az = xyz[2];
}

void printAccelerationAndModule(){
  Serial.print(ax);
  Serial.print(",");
  Serial.print(ay);
  Serial.print(",");
  Serial.print(az);
  Serial.print(",");
  Serial.println(module);
}


