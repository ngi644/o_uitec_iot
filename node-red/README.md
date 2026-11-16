# Node-RED 完成版フロー

各演習が終わった時点の完成版フローです．演習で遅れたときや，フローがうまく動かないときに読み込んで，次の演習に進むために使います．

|ファイル|内容|対応する資料|
|:-|:-|:-:|
|`s03_mqtt_broker.json`|MQTTブローカ（Aedes）と，センサデータの受信（`device01/bme`）|[s03](../s03.md)|
|`s04_dashboard.json`|s03 ＋ ダッシュボード（温度・湿度・気圧のゲージ）|[s04](../s04.md)|
|`s05_led_control.json`|s04 ＋ LEDの操作（`device01/status`），接続状態の表示（`device01/connection`）|[s05](../s05.md)|
|`s06_firebase.json`|s05 ＋ Realtime Databaseへの保存（`stations/st001/data`）|[s06](../s06.md)|
|`s07_firebase_led.json`|s06 ＋ Realtime Database経由のLEDの操作（`stations/st001/status`）|[s07](../s07.md)|

後のファイルほど，前の演習の内容をすべて含んでいます．途中から始める場合は，その演習の**1つ前**のファイルを読み込みます（例：s05から始めるなら `s04_dashboard.json`）．

## 必要なノード

フローを読み込む前に，使うノードをインストールしておきます．

|ノード|使うファイル|インストール方法|
|:-|:-:|:-|
|node-red-contrib-aedes|すべて|[s03](../s03.md) の手順（ターミナルで `npm install`）|
|@flowfuse/node-red-dashboard|s04以降|`パレットの管理` → `ノードを追加`|
|node-red-contrib-firebase-data|s06以降|`パレットの管理` → `ノードを追加`|

ノードが足りないまま読み込むと，そのノードが破線で表示され，デプロイできません．

## 読み込み方

1. このファイルをPC（またはRaspberry Pi）にダウンロードします．GitHubでファイルを開き，`Raw` ボタンの右にあるダウンロードボタンを押します．
2. Node-REDの右上のメニュー（≡）→ `読み込み` を開きます．
3. `読み込むファイルを選択` からダウンロードしたファイルを選び，`読み込み` を押します．
4. **すでに同じノードがある**と表示された場合は，`置換` を選びます．完成版フローは，どの段階のファイルも同じノードIDを使っているので，前の段階のフローがそのまま置き換わります．
5. `デプロイ` を押します．

> **自分で作ったフローがある場合の注意**
> 自分で作った `Aedes MQTT broker` ノードが残っていると，ブローカが2つになり，ポート1883が重なって動きません．読み込む前に，自分で作ったフローのタブを削除するか，タブを無効にしてください．

## 読み込んだ後に確認・変更すること

- **トピック名**：`device01/bme`，`device01/status`，`device01/connection` を使っています．ESP32のコードのトピックと一致しているか確認します．
- **Firebaseの設定（s06以降）**：`Add Firebase` ノードを開き，Firebaseの設定（鉛筆アイコン）で `YOUR-PROJECT-ID-default-rtdb` を，自分のRealtime DatabaseのURLの `https://` と `.firebaseio.com/` の間の部分に書き換えます．
- **Firebaseの認証（s09以降）**：セキュリティルールを設定した後は，`Auth Type` を `JSON Web Token` にし，サービスアカウントの `Client Email` と `Private Key` を入力します．パスワードや秘密鍵は，フローのファイルには保存されません（読み込むたびに入力が必要です）．
- **MQTTブローカの認証（s05の最後）**：ブローカにユーザー名とパスワードを設定した場合は，`Aedes MQTT broker` ノードの `Security` タブと，サーバ設定 `localhost` の `セキュリティ` タブに入力し直します．

## 動作確認について

これらのフローは，Node-RED 4.1，node-red-contrib-aedes 1.4.0，@flowfuse/node-red-dashboard 1.37.0，node-red-contrib-firebase-data 0.0.12 で読み込みと動作を確認しています（MQTTの送受信，ダッシュボードの表示，Realtime Database への push / set / get）．Realtime Database は，同じ REST API を持つ模擬サーバで確認しています．
