#define BUTTUN_PIN 9

void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTUN_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(BUTTUN_PIN));
  delay(100);
}
