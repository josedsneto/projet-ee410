int sensorPin = A0;      // The analog pin where your sensor is connected
int sensorValue = 0;

void setup() {
  Serial.begin(9600); // Set the baud rate to match with the Bluetooth module
}

void loop() {
  if (Serial.available()) {
    char data = Serial.read();  // Read the incoming data
    Serial.print("Received: ");
    Serial.println(data);       // Echo the data back via Bluetooth
  }
}
