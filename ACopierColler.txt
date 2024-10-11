#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// Créer un objet pour le capteur ADXL345
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// Seuils pour détecter une chute
#define FREEFALL_THRESHOLD 0.5  // Accélération proche de la chute libre (en g)
#define IMPACT_THRESHOLD 1.5    // Accélération à l'impact (en g)
#define FREEFALL_TIME 100       // Temps minimum en ms pour considérer une chute libre
#define IMPACT_TIME 500         // Temps maximum après chute libre pour détecter l'impact

// Variables pour stocker le temps
unsigned long freefallStart = 0;
bool isFreefalling = false;

void setup() {
  // Initialiser la communication série
  Serial.begin(9600);
  
  // Initialiser le capteur ADXL345
  if (!accel.begin()) {
    Serial.println("Erreur : impossible de trouver l'ADXL345");
    while (1);
  }
  
  // Configurer la plage à ±2g pour une meilleure précision
  accel.setRange(ADXL345_RANGE_2_G);
}

void loop() {
  // Créer un événement pour stocker les données d'accélération
  sensors_event_t event;
  accel.getEvent(&event);

  // Calculer l'accélération totale (la norme du vecteur)
  float accelTotal = sqrt(pow(event.acceleration.x, 2) + 
                          pow(event.acceleration.y, 2) + 
                          pow(event.acceleration.z, 2)) / 9.81;  // Convertir en "g"
  
  Serial.print("Accélération totale: ");
  Serial.print(accelTotal);
  Serial.println(" g");

  // Détection de la phase de chute libre (accélération faible)
  if (accelTotal < FREEFALL_THRESHOLD) {
    if (!isFreefalling) {
      isFreefalling = true;
      freefallStart = millis();
      Serial.println("Début de chute libre détecté");
    }
  }

  // Si nous sommes en chute libre et que le temps est suffisant
  if (isFreefalling) {
    // Vérifier si la phase de chute libre dure depuis suffisamment longtemps
    if (millis() - freefallStart > FREEFALL_TIME) {
      // Détection de l'impact
      if (accelTotal > IMPACT_THRESHOLD) {
        Serial.println("Impact détecté. Chute confirmée !");
        isFreefalling = false;  // Réinitialiser la détection de chute
      }
    }

    // Si le temps écoulé dépasse le temps maximum sans impact, annuler la détection de chute
    if (millis() - freefallStart > IMPACT_TIME) {
      Serial.println("Pas d'impact détecté après la chute libre, annulation.");
      isFreefalling = false;  // Réinitialiser
    }
  }

  delay(100);  // Délai pour lisser les mesures
}
