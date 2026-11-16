# IoTシステムの構築とその活用

演習ドキュメント

ESP32でセンサの値を取得し，MQTT（Raspberry Pi + Node-RED）を経由してクラウド（Firebase）に保存・可視化し，Webアプリケーションから機器を操作するまでを，2日間で一通り構築します．

## 構成

```
[センサ BME280] → [ESP32] --MQTT--> [Raspberry Pi / Node-RED] --HTTPS--> [Firebase] → [Webアプリ]
                     ↑                                                                   |
                     └──────────────────── LEDの操作（逆方向の流れ） ────────────────────┘
```

## 目次

|No.|内容|
|:-:|:-|
|[s01](s01.md)|基本セットアップ（Arduino IDE，ESP32，ライブラリ）|
|[s02](s02.md)|ESP32-DevKitCについて，オンチップ温度・BME280センサの値の取得|
|[s03](s03.md)|Node-REDとMQTTブローカのセットアップ|
|[s04](s04.md)|ESP32をPublisherにしてセンサ情報を送信，ダッシュボード，OLED表示|
|[s05](s05.md)|MQTT Subscribe（LEDの制御），Retain・Will・QoSの実験，ブローカの認証|
|[s06](s06.md)|Firebaseのセットアップ，データの可視化Webアプリ|
|[s07](s07.md)|Firebaseとの連携，WebアプリケーションからLEDを操作|
|[s08](s08.md)|Webアプリケーションにログイン機能を追加|
|[s09](s09.md)|データベースのセキュリティルール（認証と認可）|
|[s10](s10.md)|応用設計ワーク：自分の分野でIoTシステムを考える|
|[つまずき対応集](troubleshooting.md)|よくあるトラブルと確認方法，講習の準備チェックリスト|

## サンプルコード

|ファイル|内容|対応する資料|
|:-|:-|:-:|
|`arduino_code/esp32_sample01`|オンチップ温度の取得|s02|
|`arduino_code/esp32_sample02`|BME280センサの値の取得|s02|
|`arduino_code/esp32_sample03`|MQTT Publish（センサ情報の送信）|s04|
|`arduino_code/esp32_sample04`|MQTT Subscribe（LEDの制御）|s05|
|`web/public/index_01.html`|データの可視化|s06|
|`web/public/index_02.html`|LED操作ボタンの追加|s07|
|`web/public/index_03.html`|ログイン機能の追加|s08|
|`web/database.rules.json`|Realtime Databaseのセキュリティルール|s09|

## 動作環境（2026年度）

- Arduino IDE 2.x，ESP32 Arduinoコア 3.x
- ライブラリ：SparkFun BME280，PubSubClient，ArduinoJson 7.x
- Raspberry Pi OS，Node-RED，node-red-contrib-aedes，@flowfuse/node-red-dashboard，node-red-contrib-firebase-data
- Firebase（Realtime Database，Hosting，Authentication）
