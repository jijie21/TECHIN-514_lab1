#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_VEML7700.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define BUZZER_PIN    A1  // Change as per your connection

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_BME280 bme;
Adafruit_VEML7700 veml7700 = Adafruit_VEML7700();

void setup() {
  Serial.begin(9600);

  // Initialize I2C
  Wire.begin();

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();

  // Initialize BME280 sensor
  if (!bme.begin(0x76)) {
    Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
    while (1);
  }

  // Initialize VEML7700 sensor
  if (!veml7700.begin()) {
    Serial.println(F("Could not find a valid VEML7700 sensor, check wiring!"));
    while (1);
  }

  // Initialize Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  // Read data from BME280
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F; // Convert to hPa

  // Read data from VEML7700
  float lux = veml7700.readLux();

  // Display data on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);

  display.print(F("Temp: "));
  display.print(temperature);
  display.println(F(" C"));
  Serial.print(temperature);
  Serial.print(" C");


  display.print(F("Pressure: "));
  display.print(pressure);
  display.println(F(" hPa"));

  display.print(F("Humidity: "));
  display.print(humidity);
  display.println(F(" %"));

  display.print(F("Light: "));
  display.print(lux);
  display.println(F(" Lux"));
  Serial.print(lux);
  Serial.print(F(" Lux"));

  display.display();

  // Buzzer beeps when light level exceeds a threshold
  if (lux > 50.0) { // Adjust this threshold as needed
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200); // Buzzer on for 100 milliseconds
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(1000); // Update every 2 seconds
}