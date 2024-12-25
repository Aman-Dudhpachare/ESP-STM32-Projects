// Blynk Credentials
#define BLYNK_TEMPLATE_ID "--"
#define BLYNK_TEMPLATE_NAME "WEEK 08"
#define BLYNK_AUTH_TOKEN "-----"


#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>


// WiFi credentials
char ssid[] = "S23";
char pass[] = "aman09877";

// Pin definitions
#define RAIN_SENSOR_ANALOG_PIN 34  // Analog rain sensor
#define RAIN_SENSOR_DIGITAL_PIN 32 // Digital rain sensor
#define BUZZER_PIN 26              // Buzzer
#define LED_PIN 25                 // LED for rain indication
#define DHT_PIN 4                  // DHT22 data pin

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT setup
#define DHTTYPE DHT22
DHT dht(DHT_PIN, DHTTYPE);

// Blynk virtual pins
#define TEMP_VPIN V1
#define HUMIDITY_VPIN V2
#define RAIN_STATUS_VPIN V3

// Rain threshold
#define RAIN_SENSOR_THRESHOLD 500  // Analog threshold for rain

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Pin modes
  pinMode(RAIN_SENSOR_DIGITAL_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  dht.begin();

  // OLED initialization
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("OLED allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  Blynk.run();

  // Read rain sensor data
  int rainAnalogValue = analogRead(RAIN_SENSOR_ANALOG_PIN);
  int rainDigitalValue = digitalRead(RAIN_SENSOR_DIGITAL_PIN);

  // Temperature and humidity readings
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Display data on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  // Show temperature
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperature);
  display.print(" C");

  // Show humidity
  display.setCursor(0, 10);
  display.print("Humidity: ");
  display.print(humidity);
  display.print(" %");

  // Rain detection logic
  display.setCursor(0, 20);
  if (rainDigitalValue == LOW || rainAnalogValue < RAIN_SENSOR_THRESHOLD) {
    display.print("Rain: Detected");
    digitalWrite(LED_PIN, HIGH);    // LED on
    digitalWrite(BUZZER_PIN, HIGH); // Buzzer on
    Blynk.virtualWrite(RAIN_STATUS_VPIN, 1); // Blynk update
    Blynk.logEvent("RAIN_DETECTED", "Rain has been detected!");
  } else {
    display.print("Rain: None");
    digitalWrite(LED_PIN, LOW);     // LED off
    digitalWrite(BUZZER_PIN, LOW);  // Buzzer off
    Blynk.virtualWrite(RAIN_STATUS_VPIN, 0); // Blynk update
  }

  display.display();

  // Update Blynk for temperature and humidity
  Blynk.virtualWrite(TEMP_VPIN, temperature);
  Blynk.virtualWrite(HUMIDITY_VPIN, humidity);

  delay(1000); // Update interval
}
