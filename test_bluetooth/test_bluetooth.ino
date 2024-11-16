#include <SoftwareSerial.h>

// Define the RX and TX pins for Bluetooth module
#define BT_RX 10  // Connect TX of BT module to pin 10
#define BT_TX 11  // Connect RX of BT module to pin 11

// Create a software serial object for Bluetooth
SoftwareSerial bluetooth(BT_RX, BT_TX);

void setup() {
  // Start the built-in serial monitor
  Serial.begin(9600); // For monitoring on Serial Monitor
  bluetooth.begin(9600); // Match the baud rate of your Bluetooth module
  
  Serial.println("Bluetooth Ready. Sending 'hello world'...");
}

void loop() {
  if (bluetooth.available() > 0) {
    // Read the incoming message
    Serial.print("Received: ");
    Serial.println(bluetooth.readStringUntil('\n'));
  }

  if (Serial.available() > 0) {
    // Read the incoming message
    bluetooth.print("Received: ");
    bluetooth.println(Serial.readStringUntil('\n'));
  }

  // Wait for 1 second
  delay(1000);
}
