#include <Ticker.h>
#include <Wire.h>
#include <SparkFunBME280.h>

// Timer event
Ticker tickerBME280;

// BME280

BME280 bme;
BME280_SensorMeasurements measurements;


void displayBME280SensorData() {
  bme.readAllMeasurements(&measurements);
  // シリアルプロッター用に「ラベル:値,ラベル:値,...」の形式で出力
  Serial.print("Humidity:");
  Serial.print(measurements.humidity, 0);
  Serial.print(",Pressure:");
  Serial.print(measurements.pressure / 100, 2);
  Serial.print(",BME-Temp:");
  Serial.println(measurements.temperature, 2);
}


void setup() {

  Serial.begin(115200);

  Wire.begin();
  bme.setI2CAddress(0x77);  // I2C address {0x76:GND or 0x77:VDD}
  if (bme.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while (1); //Freeze
  }

  tickerBME280.attach_ms(5000, displayBME280SensorData);

}


void loop() {

}

