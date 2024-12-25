// Blynk credentials
#define BLYNK_TEMPLATE_ID "TMPL3kB_AseTD"
#define BLYNK_TEMPLATE_NAME "Rain Notification"
#define BLYNK_AUTH_TOKEN "KjCTm7KXpXaUDt8fNX-dLQ-W-gaoy5Dr"

#include <WiFi.h>  // WiFi library for ESP32
#include <BlynkSimpleEsp32.h>  // Blynk library for ESP32

// Pin definitions
#define RAIN_SENSOR_ANALOG_PIN 34  // GPIO 34 for the rain sensor (analog pin)
#define RAIN_SENSOR_DIGITAL_PIN 4  // GPIO 4 for the rain sensor (digital pin)
#define BUZZER_PIN 5               // GPIO 5 for the buzzer

#define RAIN_SENSOR_THRESHOLD 500  // Threshold for rain detection (analog value)

char ssid[] = "POCO X2";  // Your Wi-Fi SSID
char pass[] = "aman09877";  // Your Wi-Fi password

void setup() {
  Serial.begin(115200);  // Start serial communication at 115200 baud rate
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);  // Connect to Blynk using Wi-Fi credentials
  
  pinMode(BUZZER_PIN, OUTPUT);  // Initialize the buzzer pin as an output
  digitalWrite(BUZZER_PIN, LOW); // Ensure buzzer is off at the start
  
  pinMode(RAIN_SENSOR_DIGITAL_PIN, INPUT);  // Initialize the rain sensor digital pin as input
}

void loop() {
  int sensorAnalogValue = analogRead(RAIN_SENSOR_ANALOG_PIN);  // Read the rain sensor analog value
  int sensorDigitalValue = digitalRead(RAIN_SENSOR_DIGITAL_PIN);  // Read the rain sensor digital value

  // Calibrate the sensor reading - if the analog value is too high, consider it as no rain.
  if (sensorAnalogValue > 4000) {
    sensorAnalogValue = 0;
    sensorDigitalValue = HIGH;  // No rain detected
  }

  // Send data to Serial Plotter
  Serial.print(sensorAnalogValue);  // Print the rain sensor analog value for the Serial Plotter
  Serial.print(" ");
  Serial.println(sensorDigitalValue);  // Print 1 if rain is detected (digital), otherwise 0

  // Control buzzer and LED based on rain detection
  if ((sensorAnalogValue > 0 && sensorAnalogValue < RAIN_SENSOR_THRESHOLD) || sensorDigitalValue == LOW) {
    Blynk.logEvent("rain_notification");  // Trigger Blynk notification event
    digitalWrite(BUZZER_PIN, HIGH);  // Turn on the buzzer if rain is detected
    Blynk.virtualWrite(V2, 1);  // Turn on LED in Blynk app
    Serial.println("Buzzer ON");  // Debug: Print when buzzer is turned on
  } else {
    digitalWrite(BUZZER_PIN, LOW);  // Turn off the buzzer if no rain is detected
    Blynk.virtualWrite(V2, 0);  // Turn off LED in Blynk app
    Serial.println("Buzzer OFF");  // Debug: Print when buzzer is turned off
  }

  // Send the sensor value to the Blynk app on virtual pin V1
  Blynk.virtualWrite(V1, sensorAnalogValue);  // Send the analog value

  Blynk.run();  // Run Blynk
  delay(1000);  // Delay for 1 second to prevent spamming
}
