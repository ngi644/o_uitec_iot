#include <Ticker.h>

// Timer event
Ticker tickerInternal;

void displayInternalSensorData() {
  // シリアルプロッターは「ラベル:値」の形式で1行ずつ受け取ります
  Serial.print("Temp:");
  Serial.println(temperatureRead());
}


void setup() {

  Serial.begin(115200);

  tickerInternal.attach_ms(1000, displayInternalSensorData);

}


void loop() {

}

