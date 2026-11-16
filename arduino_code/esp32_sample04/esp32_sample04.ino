#include <WiFi.h>
#include <Wire.h>
#include <time.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <SparkFunBME280.h>

// wifi config
#define WIFI_SSID "{WiFiSSID}"
#define WIFI_PASSWORD "{WiFiパスワード}"

// MQTT config
#define MQTT_SERVER "{Raspi IPアドレス}"
#define MQTT_PORT 1883
#define MQTT_BUFFER_SIZE 128
#define TOPIC "device01/bme"
#define TOPIC_STATUS "device01/status"

// デバイスID　デバイスIDは機器ごとにユニークにします
#define DEVICE_ID "esp001"

// センサデータの送信間隔 (ミリ秒)
#define SEND_INTERVAL_MS 3000

// LED
#define LED_PIN 4
#define BLINK_INTERVAL_MS 100

// BME280
BME280 bme;
BME280_SensorMeasurements measurements;

// MQTT Subscribe用のJSON
JsonDocument json_request;

// MQTT Publish用のJSONとバッファ
JsonDocument json_message;
char message_buffer[MQTT_BUFFER_SIZE];

// MQTT用インスタンス作成
WiFiClient espClient;
PubSubClient client(espClient);

// 最後にデータを送信した時刻 (millis()の値)
unsigned long lastSendTime = 0;

// LEDステータス (0:変更なし 1:点灯 2:消灯 3:点滅)
int led_status = 0;
bool blink_on = false;
unsigned long lastBlinkTime = 0;


// MQTT Subscribeのコールバック（メッセージを受信したときに呼ばれる）
void mqttCallback(char* topic, byte* payload, unsigned int length) {

  DeserializationError err = deserializeJson(json_request, payload, length);
  if (err) {
    Serial.println("Deserialize error");
    Serial.println(err.c_str());
    return;
  }

  serializeJson(json_request, Serial);
  Serial.println("");

  // "status" が無いときは 0 (変更なし) とする
  led_status = json_request["status"] | 0;
}


// WiFiへの接続
void setupWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected : ");
  Serial.println(WiFi.localIP());
  // sync Time
  configTime(3600L * 9, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");
}


// MQTTブローカへの接続（切断されていたら再接続）
void connectMqtt() {
  while (!client.connected()) {
    Serial.println("Mqtt Reconnecting");
    if (client.connect(DEVICE_ID)) {
      // 接続のたびに購読し直す
      client.subscribe(TOPIC_STATUS);
      Serial.println("Mqtt Connected");
    } else {
      Serial.print("failed, state=");
      Serial.println(client.state());
      delay(1000);
    }
  }
}


void sendSensorData() {
  //センサからデータの取得
  bme.readAllMeasurements(&measurements);
  Serial.println("Humidity,Pressure,BME-Temp");
  Serial.print(measurements.humidity, 0);
  Serial.print(",");
  Serial.print(measurements.pressure / 100, 2);
  Serial.print(",");
  Serial.println(measurements.temperature, 2);

  // ペイロードを作成して送信を行う．
  json_message.clear();
  json_message["humid"] = measurements.humidity;
  json_message["press"] = measurements.pressure / 100;
  json_message["temp"] = measurements.temperature;
  serializeJson(json_message, message_buffer, sizeof(message_buffer));
  client.publish(TOPIC, message_buffer);
}


// led_status に応じてLEDを制御する（delayを使わない）
void updateLed() {
  switch (led_status) {
    case 1:
      // LED ON
      digitalWrite(LED_PIN, HIGH);
      break;
    case 2:
      // LED OFF
      digitalWrite(LED_PIN, LOW);
      break;
    case 3:
      // LED Blink: BLINK_INTERVAL_MS ごとに点灯/消灯を切り替える
      if (millis() - lastBlinkTime >= BLINK_INTERVAL_MS) {
        lastBlinkTime = millis();
        blink_on = !blink_on;
        digitalWrite(LED_PIN, blink_on ? HIGH : LOW);
      }
      break;
    default:
      // 0 またはそれ以外: 何もしない
      break;
  }
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

  pinMode(LED_PIN, OUTPUT);

  // WiFi接続
  setupWiFi();

  // MQTTブローカの設定と，受信時のコールバックの登録
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(mqttCallback);
}


void loop() {
  // MQTT未接続の場合は，再接続
  if (!client.connected()) {
    connectMqtt();
  }
  // MQTTの送受信処理（受信するとここで mqttCallback が呼ばれる）
  client.loop();

  // 前回の送信から SEND_INTERVAL_MS 経過していたら送信する
  unsigned long now = millis();
  if (now - lastSendTime >= SEND_INTERVAL_MS) {
    lastSendTime = now;
    sendSensorData();
  }

  updateLed();
}
