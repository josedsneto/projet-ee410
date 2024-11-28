#include <Wire.h>

double xyz359[3];
// Les valeurs de l'accélération suivant les différents axes
double ax359, ay359, az359; 
double module359;

// =========================Valeurs pour la librairie =========================

byte _buff359[9] ;    //6 bytes buffer for saving data read from the device
double gains359[3];        // counts to Gs
bool status359;           // set when error occurs
// see error code for details
byte error_code359;       // Initial state

#define ADXL359_DEVICE (0x1D)    // ADXL359 device address for I2C
#define ADXL359_TO_READ (9)      // num of bytes we are going to read each time (two bytes for each axis)

#define ADXL359_OK    1 // no error
#define ADXL359_ERROR 0 // indicates error is present

#define ADXL359_NO_ERROR   0 // initial state
#define ADXL359_READ_ERROR 1 // problem reading accel
#define ADXL359_BAD_ARG    2 // bad method argument


#define ADXL359_STATUS 0x04
#define ADXL359_THRESH_ACT_H 0x25 // Registre du theresold activation
#define ADXL359_THRESH_ACT_L 0x26 // Registre du theresold activation
#define ADXL359_ACT_COUNT 0x27 // Number of consecutive events above threshold (from ACT_THRESH) required to detect activity
#define ADXL359_DATAX3 0x08
#define ADXL359_POWER_CTL 0x2d
#define ADXL359_RANGE 0x2C // Setting the range
#define ADXL359_FILTER 0x28 // Filter settings
#define ADXL359_OFFSET_X_H 0x1E


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //for 20g
  gains359[0] = 0.000039; gains359[1] = 0.000039; gains359[2] = 0.000039;
  // for 10g
  // gains359[0] = 0.0000195;   gains359[1] = 0.0000195;   gains359[2] = 0.0000195;

  // Start communication
  Wire.begin();

  //Turning on the ADXL359
  setRegisterBit359(ADXL359_POWER_CTL, 0, 1); // Go to Standby Mode, must go to measurment mode after setup.
  delay(100);

  // Set range to +- 20g
  setRegisterBit359(ADXL359_RANGE, 0, 0);
  setRegisterBit359(ADXL359_RANGE, 1, 1);
  

  setRegisterBit359(ADXL359_POWER_CTL, 0, 0); // Go to measurment mode

}

void loop() {
  getAcceleration359(xyz359);
  getAccelerometerValues359();
  module359 = sqrt(ax359*ax359 + ay359*ay359 + az359*az359); // We calculate de magnitude
  printAccelerationAndModule359();
  delay(200); 

}

void writeTo359 (byte address, byte val) {
    Wire.beginTransmission(ADXL359_DEVICE); // start transmission to device
    Wire.write(address);             // send register address
    Wire.write(val);                 // send value to write
    Wire.endTransmission();         // end transmission
}

bool getRegisterBit359(byte regAdress, int bitPos) {
    byte _b;
    readFrom359(regAdress, 1, &_b);
    return ((_b >> bitPos) & 1);
}

void setRegisterBit359(byte regAdress, int bitPos, bool state) {
    byte _b;
    readFrom359(regAdress, 1, &_b);
    if (state) {
        _b |= (1 << bitPos);  // forces nth bit of _b to be 1.  all other bits left alone.
    } else {
        _b &= ~(1 << bitPos); // forces nth bit of _b to be 0.  all other bits left alone.
    }
    writeTo359(regAdress, _b);
}


void getAcceleration359(double* xyz) {
    int i;
    int xyz_int[3];
    readAccel359(xyz_int);
    for (i = 0; i < 3; i++) {
        xyz359[i] = xyz_int[i] * gains359[i];
    }
}

// Reads the acceleration into three variable x, y and z
void readAccel359(int* xyz) {
    readXYZ359(xyz, xyz + 1, xyz + 2);
}
void readXYZ359(int* x, int* y, int* z) {
    uint8_t buffer[9]; // Buffer pour lire 3 axes (3 octets par axe)
    readFrom359(ADXL359_DATAX3, 9, buffer); // Lire 9 octets à partir du registre DATAX0

    // Extraction des données brutes sur 20 bits pour chaque axe
    *x = (int)((((uint32_t)(buffer[0])) << 12) | 
               (((uint32_t)(buffer[1])) << 4) | 
               (((uint32_t)(buffer[2])) >> 4));

    *y = (int)((((uint32_t)(buffer[3])) << 12) | 
               (((uint32_t)(buffer[4])) << 4) | 
               (((uint32_t)(buffer[5])) >> 4));

    *z = (int)((((uint32_t)(buffer[6])) << 12) | 
               (((uint32_t)(buffer[7])) << 4) | 
               (((uint32_t)(buffer[8])) >> 4));

    // Sign extension pour convertir les valeurs 20 bits en entiers signés
    if (*x & 0x80000) *x |= 0xFFF00000; // Si bit 20 = 1, étendre le signe
    if (*y & 0x80000) *y |= 0xFFF00000; // Idem pour Y
    if (*z & 0x80000) *z |= 0xFFF00000; // Idem pour Z
}


// Reads num bytes starting from address register on device in to _buff array
void readFrom359(byte address, int num, byte _buff[]) {
    Wire.beginTransmission(ADXL359_DEVICE); // start transmission to device
    Wire.write(address);             // sends address to read from
    Wire.endTransmission(false);         // end transmission

    Wire.beginTransmission(ADXL359_DEVICE); // start transmission to device
    Wire.requestFrom(ADXL359_DEVICE, num);    // request 6 bytes from device

    int i = 0;
    while (Wire.available()) {      // device may send less than requested (abnormal)
        _buff[i] = Wire.read();    // receive a byte
        i++;
    }
    if (i != num) {
        status359 = ADXL359_ERROR;
        error_code359 = ADXL359_READ_ERROR;
    }
    Wire.endTransmission();         // end transmission
}


void getAccelerometerValues359() {
  getAcceleration359(xyz359);
  ax359 = xyz359[0]; ay359 = xyz359[1]; az359 = xyz359[2];
}

void printAccelerationAndModule359(){
  Serial.print(ax359);
  Serial.print(",");
  Serial.print(ay359);
  Serial.print(",");
  Serial.print(az359);
  Serial.print(",");
  Serial.println(module359);
}


