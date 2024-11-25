#include <Wire.h>
#include <ADXL345.h>
#include <SoftwareSerial.h>

#define BUTTON_PIN 9

#define FALL_LOW_LEVEL 0.6
#define FALL_HIGH_LEVEL 1.5
// Time during which we look to see if there has been a peak in acceleration (in milliseconds)
#define TIME_FALL_MAX_DETECTION 500
#define CHUTE_ERROR 0.4  //Margin d'erreur sur laquel une personne peut buger sans que le système comprendre comme
                         //false chute

ADXL345 adxl; //variable adxl is an instance of the ADXL345 library
double xyz[3];
// Les valeurs de l'accélération suivant les différents axes
double ax, ay, az; 
double module;
double maxModuleValue = 1;
int buttonValue;
bool chuteLabel = false;  //when in waiting state, this label is resposible for passing to the FallDetected.

SoftwareSerial mySerial(10, 11); // RX, TX

// Enumeration of the differents states
enum State {
  Idle,
  StartFall,
  WaitFall,
  FallDetected
};

// The current State
State CS = Idle;

long startTime;
long startWaitTime;
long startFallTime;
long startOkTime;


void setup() {
    Serial.begin(9600);
    adxl.powerOn();

    //set activity/ inactivity thresholds (0-255)
    adxl.setActivityThreshold(75); //62.5mg per increment
    adxl.setInactivityThreshold(75); //62.5mg per increment
    adxl.setTimeInactivity(10); // how many seconds of no activity is inactive?

    //look of activity movement on this axes - 1 == on; 0 == off
    adxl.setActivityX(1);
    adxl.setActivityY(1);
    adxl.setActivityZ(1);

    //look of inactivity movement on this axes - 1 == on; 0 == off
    adxl.setInactivityX(1);
    adxl.setInactivityY(1);
    adxl.setInactivityZ(1);

    //look of tap movement on this axes - 1 == on; 0 == off
    adxl.setTapDetectionOnX(0);
    adxl.setTapDetectionOnY(0);
    adxl.setTapDetectionOnZ(1);

    //set values for what is a tap, and what is a double tap (0-255)
    adxl.setTapThreshold(50); //62.5mg per increment
    adxl.setTapDuration(15); //625us per increment
    adxl.setDoubleTapLatency(80); //1.25ms per increment
    adxl.setDoubleTapWindow(200); //1.25ms per increment

    //set values for what is considered freefall (0-255)
    adxl.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
    adxl.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment

    //setting all interrupts to take place on int pin 1
    //I had issues with int pin 2, was unable to reset it
    adxl.setInterruptMapping(ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN);
    adxl.setInterruptMapping(ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN);

    //register interrupt actions - 1 == on; 0 == off
    adxl.setInterrupt(ADXL345_INT_SINGLE_TAP_BIT, 1);
    adxl.setInterrupt(ADXL345_INT_DOUBLE_TAP_BIT, 1);
    adxl.setInterrupt(ADXL345_INT_FREE_FALL_BIT,  1);
    adxl.setInterrupt(ADXL345_INT_ACTIVITY_BIT,   1);
    adxl.setInterrupt(ADXL345_INT_INACTIVITY_BIT, 1);


    mySerial.begin(9600);
    mySerial.println("Starting Bluetooth Connection");
}

void loop() {
  // int x, y, z;
  // adxl.readXYZ(&x, &y, &z); //read the accelerometer values and store them in variables  x,y,z
  // // Output x,y,z values
  // Serial.print("values of X , Y , Z: ");
  // Serial.print(x);
  // Serial.print(" , ");
  // Serial.print(y);
  // Serial.print(" , ");
  // Serial.println(z);

  // ----- getValues and calculation
  getAccelerometerValues();
  module = sqrt(ax*ax + ay*ay + az*az); // We calculate de magnitude
  // printAccelerationAndModule();
  //printFormatJoseValues();
  buttonValue = digitalRead(BUTTON_PIN);
  // The running of the fallDetector => Following an ASM
  runASM();
  
  BluetoothAccelerationAndModule();


  delay(100);

}


void getAccelerometerValues() {
  adxl.getAcceleration(xyz);
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
void BluetoothAccelerationAndModule(){
  mySerial.print(ax);
  mySerial.print(",");
  mySerial.print(ay);
  mySerial.print(",");
  mySerial.print(az);
  mySerial.print(",");
  mySerial.print(module);
  mySerial.print(",");
  mySerial.println(buttonValue);

}

void printFormatJoseValues(){
  Serial.println("****************************");
  Serial.println("values of X , Y , Z: -1 , 2 , 23");
  Serial.print("X="); Serial.print(ax); Serial.println(" g");
  Serial.print("Y="); Serial.print(ay); Serial.println(" g");
  Serial.print("Z="); Serial.print(az); Serial.println(" g");

}

void runASM() {
  // Notre diagramme d'Etat
  switch (CS) {
    case Idle:
      // Current working
      //Serial.print("En attente. module: ");
      //Serial.print(module);
      Serial.println("idle"); 

      if(module<FALL_LOW_LEVEL) { // Maybe there is a fall
        CS = StartFall;
        //maxModuleValue = 1; // Valeur neutre
        //startTime = millis(); // Start Measuring time
      }

      break;

    case StartFall:
      startTime = millis();
      
      while ((millis() - startTime) < TIME_FALL_MAX_DETECTION) { // Si on a dépassé le temps d'enregistrement des valeurs
        getAccelerometerValues();
        module = sqrt(ax*ax + ay*ay + az*az);
        maxModuleValue = max(maxModuleValue, module);
      }

      if (maxModuleValue > FALL_HIGH_LEVEL) { // Il y a bien eu une chute
          CS = WaitFall;
      }
      else {
          CS = Idle;
          //Serial.print("Pas de chute détectée finalent, max Module : ");
          //Serial.println(maxModuleValue);
          //Serial.print(module);
          Serial.println("ok");
      }

      break;

    case WaitFall:
      startWaitTime = millis();
      
      while ((millis() - startWaitTime) < 5000) { // On attend 5 seconds pour voir si la personne va bien
        getAccelerometerValues();
        module = sqrt(ax*ax + ay*ay + az*az);
        //Serial.println(module);
        
        if ((0.9 - CHUTE_ERROR < module) and (module < 0.9 + CHUTE_ERROR)) {
          CS = WaitFall;
          //Serial.println("Anomalie détectée, est-ce qu'il y un chute");
          //Serial.print(module);
          Serial.println("waiting");
          delay(100);
          chuteLabel = true;

        } else {
          startOkTime = millis();
          CS = Idle;
          chuteLabel = false;
          //Serial.println("Alarme false");
          //Serial.println("Retour etat initial");
          //Serial.print(module);
          
          while((millis() - startOkTime) < 5000) {
            Serial.println("ok");
            delay(100);
          }
          
          break;
        }
      }
      
      if (chuteLabel == true) {
        CS = FallDetected;
      }else {
        CS = Idle;
      }
      break;

    case FallDetected:
      startFallTime = millis();

      while((millis() - startFallTime) < 5000) {
        Serial.println("danger");
        delay(100);
      }
    
      //Serial.print("Chute détectée !!!!!!!!!!!!!!!!!!!!!! max :");
      //Serial.println(maxModuleValue);
      
      //delay(5000);
      CS = Idle;
      //Serial.println("Retour etat initial");

      break;
    
    default :
      //Serial.println("Not a State, Error");
      delay(1000);
      break;
  }
}
