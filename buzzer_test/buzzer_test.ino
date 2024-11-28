#define BUZZER 2
#define BUTTON 13

void setup() {
  // put your setup code here, to run once:
  pinMode(BUTTON, INPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(BUZZER, LOW);
  

}

void loop() {
  // put your main code here, to run repeatedly:

  if (digitalRead(BUTTON) == HIGH) {
    digitalWrite(BUZZER, HIGH); 
    delay(1000);
    digitalWrite(BUZZER, LOW); 
    delay(1000);
    digitalWrite(BUZZER, HIGH); 
    delay(1000);
    digitalWrite(BUZZER, LOW); 
    delay(1000); digitalWrite(BUZZER, HIGH); 
    delay(1000);
    digitalWrite(BUZZER, LOW); 
    delay(1000);
    digitalWrite(BUZZER, HIGH); 
    delay(1000);
    digitalWrite(BUZZER, LOW); 
    delay(1000); 
  } else {
    digitalWrite(BUZZER, LOW);  
  }

}
