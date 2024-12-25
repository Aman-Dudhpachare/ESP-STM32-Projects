#define BLYNK_TEMPLATE_ID "TMPL3rfdZNcyO"
#define BLYNK_TEMPLATE_NAME "ULTRASONIC"
#define BLYNK_AUTH_TOKEN "dBoRlw2lDdHl-HARklFF1MAwdyiBWQwZ"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int trigPin = 5;
const int echoPin = 18;

// Define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
int distanceCm;
float distanceInch;  // Changed to float for precision

// Wi-Fi credentials
char ssid[] = "S23"; // Your Wi-Fi SSID
char pass[] = "aman09877"; // Your Wi-Fi password

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input

  // Initialize display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(500);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  // Blynk run
  Blynk.run();

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in cm
  distanceCm = duration * SOUND_SPEED / 2;

  // Convert to inches (using float)
  distanceInch = distanceCm * CM_TO_INCH;

  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);

  // Display distance on OLED
  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.print("DISTANCE");
  
  display.setCursor(0, 25);
  display.setTextSize(2);
  display.print(distanceCm);
  display.print(" cm");

  display.setCursor(0, 50);
  display.setTextSize(2);
  display.print(distanceInch, 2);  // Printing float value with 2 decimal places
  display.print(" in");
  
  display.display();

  // Send distance to Blynk (Virtual Pin V3 for cm and V4 for inch)
  Blynk.virtualWrite(V3, distanceCm);  // Sending integer value for cm
  Blynk.virtualWrite(V4, distanceInch);  // Sending float value for inches

  delay(500);
}
