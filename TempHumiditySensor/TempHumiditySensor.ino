// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
#include <Arduino.h>
#include <TM1637Display.h>
#include <SoftwareSerial.h>

//Serial config for bluetooth module
SoftwareSerial mySerial(10, 11); // RX, TX

// Module connection pins (Digital Pins)
#define CLK 6
#define DIO 7

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   2000

#include "DHT.h"
#include <Adafruit_Sensor.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

#define BUTTONPIN 3

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

float maxTemperature = 0;
TM1637Display display(CLK, DIO);
uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  pinMode(BUTTONPIN, INPUT);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  mySerial.begin(9600);
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  if (t > maxTemperature) {
    maxTemperature = t;
  }

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

    

  int buttonValue = digitalRead(BUTTONPIN);

  Serial.print("Button Value : ");
  Serial.println(buttonValue);

  if (buttonValue == 1) {
    display.setBrightness(0x0f);
    mySerial.print("Max temperature = ");
    mySerial.println(maxTemperature);
    int temp = maxTemperature;
    display.showNumberDec(temp, false);
  }
  else {
    display.setSegments(blank);
    mySerial.print(F("Humidity: "));
    mySerial.print(h);
    mySerial.print(F("%  Temperature: "));
    mySerial.print(t);
    mySerial.print(F("°C "));
    mySerial.print(f);
    mySerial.print(F("°F  Heat index: "));
    mySerial.print(hic);
    mySerial.print(F("°C "));
    mySerial.print(hif);
    mySerial.println(F("°F"));
  }
}
