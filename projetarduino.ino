#include <LiquidCrystal.h>

// Initialisation du LCD avec les broches spécifiées
LiquidCrystal lcd(32, 31, 22, 23, 24, 25, 26, 27, 28, 29);

// Définition des broches
const int lm35Pin = A1;           // Broche analogique pour le capteur LM35
const int currentSensorPin = A0;  // Broche pour le capteur de courant
const int motorRelayPin = 18;     // Broche numérique pour le relais moteur
const int buzzerRelayPin = 11;    // Broche numérique pour le relais buzzer
const int redLedPin = 9;          // LED rouge pour courant élevé
const int greenLedPin = 10;       // LED verte pour courant normal

// Température seuil et courant seuil
const float tempSeuil = 45.0;
const float currentSeuil = 5.00; // Seuil du courant en ampères

// Variables globales pour stocker la température et le courant
float temperature = 0.0;
float current = 0.0;

void setup() {
    lcd.begin(16, 2);          // Initialisation de l'écran LCD 16x2

    pinMode(motorRelayPin, OUTPUT); // Configurer le relais moteur en sortie
    digitalWrite(motorRelayPin, LOW); // Relais moteur au repos (ouvert en NO)

    pinMode(buzzerRelayPin, OUTPUT); // Configurer le relais buzzer en sortie
    digitalWrite(buzzerRelayPin, LOW); // Relais buzzer au repos (buzzer éteint)

    pinMode(redLedPin, OUTPUT); // Configurer la LED rouge en sortie
    pinMode(greenLedPin, OUTPUT); // Configurer la LED verte en sortie
}

void loop() {
    mesurerTemperature();
    mesurerCourant();
    afficherLCD();
    controlerTemperature();
    controlerCourant();

    delay(1000); // Rafraîchissement toutes les secondes
}

// Mesurer la température à partir du capteur LM35
void mesurerTemperature() {
    int sensorValueTemp = analogRead(lm35Pin);  // Lecture du capteur LM35
    temperature = sensorValueTemp * (5.0 / 1023.0) * 100.0; // Conversion en °C
}

// Mesurer le courant à partir du capteur de courant
void mesurerCourant() {
    int sensorValueCurrent = analogRead(currentSensorPin);  // Lecture du capteur de courant
    float voltage = sensorValueCurrent * (5.0 / 1023.0);    // Conversion en tension
    current = (voltage - 2.5) / 0.066; // Calcul du courant (A) pour un capteur ACS712
}

// Afficher les valeurs de température et de courant sur l'écran LCD
void afficherLCD() {
    lcd.setCursor(0, 0); 
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C   ");

    lcd.setCursor(0, 1);
    lcd.print("Courant: ");
    lcd.print(current, 3);
    lcd.print(" A   ");
}

// Contrôle de la température et activation du relais moteur si la température dépasse le seuil
void controlerTemperature() {
    if (temperature > tempSeuil) {
        digitalWrite(motorRelayPin, HIGH); // Activer le relais pour éteindre le moteur
    } else {
        digitalWrite(motorRelayPin, LOW); // Désactiver le relais pour allumer le moteur
    }
}

// Contrôle du courant et activation du buzzer en fonction du seuil de courant
void controlerCourant() {
    if (current >= currentSeuil) {
        digitalWrite(redLedPin, HIGH);  // Allumer la LED rouge pour courant élevé
        digitalWrite(greenLedPin, LOW); // Éteindre la LED verte
        digitalWrite(buzzerRelayPin, HIGH); // Activer le relais buzzer pour sonner
    } else {
        digitalWrite(redLedPin, LOW);   // Éteindre la LED rouge
        digitalWrite(greenLedPin, HIGH); // Allumer la LED verte pour courant normal
        digitalWrite(buzzerRelayPin, LOW); // Désactiver le relais buzzer
    }
}
