#define BLYNK_TEMPLATE_ID "TMPL3YWgU_0Oa"
#define BLYNK_TEMPLATE_NAME "IR Sensor"
#define BLYNK_AUTH_TOKEN "kY5q7mfFoM3fKEVqWIlyNFZ87J5ShkIm"

// Include necessary libraries
#include <BlynkSimpleEsp32.h> // For ESP32
#include <WiFi.h>             // For ESP32 WiFi

// Blynk credentials
char auth[] = BLYNK_AUTH_TOKEN;   
char ssid[] = "S23";              
char pass[] = "aman09877";        

// Pin definitions
const int IR_SENSOR_PIN = 4; // IR Sensor output pin connected to D4
const int BUZZER_PIN = 5;     // Buzzer pin connected to D5

// Define Blynk virtual pin for LED widget
#define BLYNK_LED_VPIN V0 // LED widget is on virtual pin V0

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  
  // Connect to WiFi and Blynk
  Blynk.begin(auth, ssid, pass);

  // Set the sensor pin as input and the buzzer pin as output
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Initialize buzzer as OFF
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  // Run Blynk
  Blynk.run();

  // Read the IR sensor's output
  int sensorValue = digitalRead(IR_SENSOR_PIN);

  // If the sensor detects an object, turn OFF the buzzer
  if (sensorValue == HIGH) {
    digitalWrite(BUZZER_PIN, LOW); // Turn buzzer OFF
    Serial.println("Object detected! Buzzer OFF");
    
    // Turn OFF LED widget in Blynk
    Blynk.virtualWrite(BLYNK_LED_VPIN, 0);  // LED OFF
  }
  // If no object is detected, turn ON the buzzer
  else {
    digitalWrite(BUZZER_PIN, HIGH); // Turn buzzer ON
    Serial.println("No object detected. Buzzer ON");
    
    // Turn ON LED widget in Blynk
    Blynk.virtualWrite(BLYNK_LED_VPIN, 255);  // LED ON
  }

  // Small delay to avoid rapid switching
  delay(100);
}
