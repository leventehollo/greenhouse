#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <LiquidCrystal.h>

#define ALTITUDE 100.0

Adafruit_BME280 bme; // I2C

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int LED_PIN = 13;
int ledState = LOW;

int period = 1000;
unsigned long time_now = 0;
long previousTime = 0;

float temperature;
float humidity;
float pressure;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(LED_PIN, OUTPUT);
  init_LCD();
  init_BME280();
}

void loop() {
  time_now = millis();

  while (time_now - previousTime > period) {
    previousTime = time_now;

    lcd.clear();

    temperature = bme.readTemperature();
    //serialTemperature();
    lcdTemperature();

    pressure = calculatePressure();
    //serialPressure();
    //lcdPressure();

    humidity = bme.readHumidity();
    //serialHumidity();
    lcdHumidity();

    if (ledState == LOW) {
      ledState = HIGH;
    }
    else {
      ledState = LOW;
    }

    digitalWrite(LED_PIN, ledState);

    Wire.requestFrom(8, 6);    // request 6 bytes from slave device #8

    while (Wire.available()) { // slave may send less than requested
      char c = Wire.read(); // receive a byte as character
      Serial.print(c);         // print the character
      lcd.setCursor(0, 1);
      lcd.print(c);
    }
  }

}

void init_LCD() {
  Serial.println(F("Init LCD..."));
  lcd.begin(16, 2);
}

void init_BME280() {
  Serial.println(F("Init BME280..."));
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void blinkLED() {
  digitalWrite(LED_PIN, HIGH);
}

float calculatePressure() {
  pressure = bme.readPressure();
  pressure = bme.seaLevelForAltitude(ALTITUDE, pressure);
  pressure = pressure / 100.0F;
  return pressure;
}

void serialTemperature() {
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");
}

void serialPressure() {
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");
}

void serialHumidity() {
  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");
}


void lcdTemperature() {
  lcd.print("T:");
  lcd.print(String(temperature, 1));
  lcd.print((char)223);
  lcd.print("C ");
}

void lcdPressure() {
  lcd.setCursor(0, 1);
  lcd.print("P: ");
  lcd.print(String(pressure, 2));
  lcd.print(" hPa");
}

void lcdHumidity() {
  lcd.print("H: ");
  lcd.print(String(humidity, 0));
  lcd.print("%");
}

void lcdAxes(float x, float y, float z) {
  lcd.setCursor(0, 1);
  lcd.print("X");
  lcd.print(x);
  lcd.print("Y");
  lcd.print(y);
  lcd.print("Z");
  lcd.print(z);
}

