#include <Wire.h>

// Adresse I2C de l'ADXL359 (par défaut 0x1D, ou 0x53 si CS est relié à GND)
#define ADXL359_I2C_ADDRESS  0x1D

// Registres
#define ADXL359_POWER_CTL    0x2D // Contrôle de l'alimentation
#define ADXL359_DATAX0       0x08 // Adresse de départ pour les données X, Y, Z

// Facteur d'échelle pour convertir les valeurs brutes en g
#define SCALE_FACTOR         0.0000039 // 3.9 µg/LSB pour ±10g

void setup() {
  Wire.begin();                // Initialisation du bus I2C
  Serial.begin(9600);        // Initialisation de la communication série
  initializeADXL359();         // Initialisation du capteur
}

void loop() {
  int x, y, z;                 // Variables pour stocker les données brutes
  readXYZ(&x, &y, &z);         // Lire les données des 3 axes
  
  // Conversion des données brutes en g
  float accel_x = x * SCALE_FACTOR;
  float accel_y = y * SCALE_FACTOR;
  float accel_z = z * SCALE_FACTOR;

  // Afficher les résultats sur le moniteur série
  Serial.print("X: "); Serial.print(accel_x, 6);
  Serial.print(" g, Y: "); Serial.print(accel_y, 6);
  Serial.print(" g, Z: "); Serial.println(accel_z, 6);
  Serial.println("");

  delay(500); // Pause de 500 ms

  uint8_t readPowerCtl;
  readRegisters(ADXL359_POWER_CTL, 1, &readPowerCtl);
  Serial.print("POWER_CTL: 0x");
  Serial.println(readPowerCtl, HEX);
}

// Fonction pour initialiser le capteur ADXL359
void initializeADXL359() {
  // Configurer le capteur en mode Measurement
  writeRegister(ADXL359_POWER_CTL, 0x01); // Activer le mode Measurement
  delay(100);
  writeRegister(ADXL359_POWER_CTL, 0x00); // Activer le mode Measurement
}

// Fonction pour lire les données des 3 axes
void readXYZ(int* x, int* y, int* z) {
  uint8_t buffer[9]; // Buffer pour stocker les données brutes

  // Lire les 9 octets à partir du registre DATAX0
  readRegisters(ADXL359_DATAX0, 9, buffer);

  // Convertir les données brutes en valeurs 20 bits signées
  *x = (int)((((uint32_t)(buffer[0])) << 12) | 
             (((uint32_t)(buffer[1])) << 4) | 
             (((uint32_t)(buffer[2])) >> 4));
  if (*x & 0x80000) *x |= 0xFFF00000; // Extension du signe pour X

  *y = (int)((((uint32_t)(buffer[3])) << 12) | 
             (((uint32_t)(buffer[4])) << 4) | 
             (((uint32_t)(buffer[5])) >> 4));
  if (*y & 0x80000) *y |= 0xFFF00000; // Extension du signe pour Y

  *z = (int)((((uint32_t)(buffer[6])) << 12) | 
             (((uint32_t)(buffer[7])) << 4) | 
             (((uint32_t)(buffer[8])) >> 4));
  if (*z & 0x80000) *z |= 0xFFF00000; // Extension du signe pour Z
}

// Fonction pour écrire dans un registre
void writeRegister(uint8_t reg, uint8_t value) {
  Wire.beginTransmission(ADXL359_I2C_ADDRESS); // Début de la transmission
  Wire.write(reg);                             // Envoyer l'adresse du registre
  Wire.write(value);                           // Envoyer la donnée
  Wire.endTransmission();                      // Terminer la transmission
}

// Fonction pour lire plusieurs registres
void readRegisters(uint8_t reg, uint8_t count, uint8_t* dest) {
  Wire.beginTransmission(ADXL359_I2C_ADDRESS); // Début de la transmission
  Wire.write(reg);                             // Envoyer l'adresse du registre
  Wire.endTransmission(false);                 // Redémarrage pour lecture
  Wire.requestFrom(int(ADXL359_I2C_ADDRESS), int(count));// Demande de données
  for (int i = 0; i < count; i++) {
    if (Wire.available()) {
      dest[i] = Wire.read();                   // Lire les octets dans le buffer
    }
  }
}
