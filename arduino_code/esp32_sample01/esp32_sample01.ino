#include <Ticker.h>

// Timer event
Ticker tickerInternal;

void displayInternalSensorData() {
  Serial.println("Temp, Hall");
  Serial.print(temperatureRead());
  Serial.print(",");
  Serial.println( hallRead());
  yield();
}


void setup() {

  Serial.begin(115200);

  tickerInternal.attach_ms(1000, displayInternalSensorData);

}


void loop() {

}