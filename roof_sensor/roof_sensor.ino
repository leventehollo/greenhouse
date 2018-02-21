#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

#define addr 0x1E //I2C Address for The HMC5883

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

int period = 300;
unsigned long time_now = 0;

void setup() {
  Wire.begin(8);
  Wire.onRequest(requestEvent); // register event

  Serial.begin(9600);

  Wire.beginTransmission(addr); // Start I2C Transmission
  Wire.write(0x00);             // Select configure register A
  Wire.write(0x60);             // Set normal measurement configuration, data output rate = 0.75Hz
  Wire.endTransmission();       // Stop I2C Transmission

  Wire.beginTransmission(addr); // Start I2C Transmission
  Wire.write(0x02);             // Select Mode register
  Wire.write(0x00);             // Set continuous measurement
  Wire.endTransmission();       // Stop I2C Transmission

  delay(300);
}

void requestEvent() {
  Wire.write("HB"); // respond with message of 6 bytes

}

float x, y, z;
long previousTime = 0;
unsigned int data[6];

void loop() {
  time_now = millis();

  while (time_now - previousTime > period) {
    previousTime = time_now;

    Wire.beginTransmission(addr);
    Wire.write(0x03);               // Select data register
    Wire.endTransmission();

    Wire.requestFrom(addr, 6);      // Request 6 bytes of data

    if (Wire.available() == 6)      // Read 6 bytes of data
    {
      data[0] = Wire.read();  // x msb
      data[1] = Wire.read();  // x lsb
      data[2] = Wire.read();  // z msb
      data[3] = Wire.read();  // z lsb
      data[4] = Wire.read();  // y msb
      data[5] = Wire.read();  // y lsb
    }


    // Convert the data
    x = ((data[0] * 256) + data[1]);
    y = ((data[4] * 256) + data[5]);
    z = ((data[2] * 256) + data[3]);

    Serial.print("X: "); Serial.print(x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(y); Serial.print("  ");
    Serial.print("Z: "); Serial.print(z); Serial.print("  "); Serial.println("uT");
  }

}


