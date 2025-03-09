#include <HardwareSerial.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <PZEM004Tv30.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <TridentTD_LineNotify.h>
#include <WiFiClientSecure.h>
#if 0
#include <SPI.h>
#include <PN532_SPI.h>
#include "PN532.h"
PN532_SPI pn532spi(SPI, 10);
PN532 nfc(pn532spi);
#elif 0
#include <PN532_HSU.h>
#include <PN532.h>
PN532_HSU pn532hsu(Serial3);
PN532 nfc(pn532hsu);
#else
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
PN532_I2C pn532i2c(Wire);  //SDA 21 SCL22
PN532 nfc(pn532i2c);
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <driver/gpio.h>
const uint16_t kIrLed = 23;  // Pin ที่ต่อกับ  IR
IRsend irsend(kIrLed);
#define dataPin 4  //DTH22
#define DHTTYPE DHT22
#define relay 2
#define ssr 15
DHT dht(dataPin, DHTTYPE);
int analogPin = 36;  //MQ-2
int val = 0;
#define PZEM_RX_PIN 18  //TX on PZEM
#define PZEM_TX_PIN 19  //RX on PZEM
#define PZEM_SERIAL Serial2
bool nfcScanComplete = false;
PZEM004Tv30 pzem(PZEM_SERIAL, PZEM_RX_PIN, PZEM_TX_PIN);
#define LINE_TOKEN "xxxx" //Line Notify Token
unsigned long previousMillisUPS = 0, previousMillis = 0, previousMillis_netpie = 60000, x, lastCheckNfcTime = 0;
String payload;
float Batt, Load, Estimated_runtime;
String Statusups;
const char* ssid = "Karn";
const char* password = "11111111";
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "xxxx"; //Netpie Client ID
const char* mqtt_username = "xxxx"; //Netpie Token
const char* mqtt_password = "xxxx"; //Netpie Client Secret
WiFiClient espClient;
PubSubClient client(espClient);
char msg[256];
int Settemp, Sethumi;
int SetActemp, SetAcfans;
int Actemp = 16;
int Acfans = 3;
HardwareSerial UPS(1);
unsigned long lastSendTime = 0;  // เก็บเวลาที่ส่งสัญญาณ IR ล่าสุด
bool powerOn = true;
bool ssrEnabled = true;
bool tempAlertSent = false;
bool humiAlertSent = false;
bool smokeAlertSent = false;
bool Ac_status = false;
bool SendSoft_shutdown = false;

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      // subscribe topic
      client.subscribe("@msg/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]:");
  String message;
  String tpc;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];  //for all data sent
  }
  Serial.println(message);
  tpc = getMsg(topic, message);
  tpc.toCharArray(msg, (tpc.length() + 1));
  if (String(topic) == "@msg/temp") {
    Settemp = message.toInt();
    EEPROM.put(0, Settemp);  // บันทึกตัวแปร Settemp ตำแหน่ง 0
    EEPROM.commit();         // Save changes in EEPROM
    Serial.print("Settemp saved: ");
    Serial.println(Settemp);
  } else if (String(topic) == "@msg/humi") {
    Sethumi = message.toInt();
    EEPROM.put(sizeof(Settemp), Sethumi);  // บันทึก humi หลังตำแหน่ง Settemp
    EEPROM.commit();                       // Save changes in EEPROM
    Serial.print("Sethumi saved: ");
    Serial.println(Sethumi);
  } else if (String(topic) == "@msg/Actemp") {
    SetActemp = message.toInt();
    EEPROM.put(sizeof(Settemp) + sizeof(Sethumi), SetActemp);
    EEPROM.commit();
    Serial.print("Actemp saved: ");
    Serial.println(SetActemp);
  } else if (String(topic) == "@msg/Acfans") {
    SetAcfans = message.toInt();
    EEPROM.put(sizeof(Settemp) + sizeof(Sethumi) + sizeof(SetActemp), SetAcfans);//รวมขนาดตัวแปร เพื่อให้ได้ตำแหน่งที่ข้อมูลจะไม่ซ้อนทับ
    EEPROM.commit();
    Serial.print("Acfans saved: ");
    Serial.println(SetAcfans);
  }
  tpc = "{\"data\": {\"Settemp\":" + String(Settemp) + ",\"Sethumi\":" + String(Sethumi) + ",\"Actemp\":" + String(SetActemp) + ",\"Acfans\":" + String(SetAcfans) + "}}";
  tpc.toCharArray(msg, (tpc.length() + 1));
  client.publish("@shadow/data/update", msg);
}


void setup() {
  Serial.begin(115200);
  dht.begin();
  UPS.begin(2400, SERIAL_8N1, 16, 17); //8N1 = Bits Per Character = 8 , Stop Bits = 1 ,Parity = none
  EEPROM.begin(512);  // Set EEPROM size to 512 bytes
  EEPROM.get(0, Settemp);
  EEPROM.get(sizeof(Settemp), Sethumi);
  EEPROM.get(sizeof(Settemp) + sizeof(Sethumi), SetActemp);
  EEPROM.get(sizeof(Settemp) + sizeof(Sethumi) + sizeof(Actemp), SetAcfans);
  Wire.begin();
  LINE.setToken(LINE_TOKEN);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  pinMode(ssr, OUTPUT);
  digitalWrite(ssr, HIGH);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  LINE.notify("ระบบเชื่อมต่อกับอินเตอร์เน็ตแล้ว");
  Serial.print("Previous set temperature: ");
  Serial.println(Settemp);
  Serial.print("Previous set humidity: ");
  Serial.println(Sethumi);
  Serial.print("Previous set temperature AC: ");
  Serial.println(SetActemp);
  Serial.print("Previous set temperature Fan: ");
  Serial.println(SetAcfans);
  nfc.begin();
  irsend.begin();
   irsend.sendNEC(0xFFA25D, 32);
  Ac_status = true;

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("PN53x board not found");
    LINE.notify("ไม่พบบอร์ด PN532");
    return;
  }

  Serial.print("Found PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware version. ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.');
  Serial.println((versiondata >> 8) & 0xFF, DEC);

  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();
  Serial.println("Waiting for NFC card scan");
}

void loop() {
  unsigned long currentMillisUPS = millis();
  unsigned long currentMillisNfc = millis();
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
  publishToMQTT();
  checkSensorValues();
  RemoteAc();
  checkPowerStatus();

  if (currentMillisNfc - lastCheckNfcTime >= 500) {
    lastCheckNfcTime = currentMillisNfc;
    checkNFC();
  }

  if (currentMillisUPS - previousMillisUPS >= 7000) {  // หน่วงเวลา 7 วินาที
    previousMillisUPS = currentMillisUPS;
    sendAndDisplayResult('Y');  // Smart Mode
    delay(100);
    sendAndDisplayResult('P');  // Load %
    delay(100);
    sendAndDisplayResult('f');  // Batt %
    delay(100);
    sendAndDisplayResult('j');  // Estimated runtime
    delay(100);
    sendAndDisplayResult('Q');  // Status Flag
  }
}


void publishToMQTT() {
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  val = analogRead(analogPin);
  int smoke = map(val, 0, 1023, 0, 100);
  float s = smoke;
  if (isnan(voltage)) voltage = 0.0;
  if (isnan(current)) current = 0.0;
  if (isnan(power)) power = 0.0;
  if (isnan(energy)) energy = 0.0;
  if (isnan(frequency)) frequency = 0.0;
  if (isnan(pf)) pf = 0.0;
  if (isnan(t)) t = 0.0;
  if (isnan(h)) h = 0.0;

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis_netpie >= 2000) {
    previousMillis_netpie = currentMillis;

    // Send value to Shadow device
    //(Ac_status ? "true" : "false") ตรวจสอบ bool และคืนค่าเป็น String
    payload = "{\"data\": {\"V\":" + String(voltage) + ", \"I\":" + String(current) + ", \"P\":" + String(power) + ", \"E\":" + String(energy) + ", \"f\":" + String(frequency) + ",\"PF\":" + String(pf) + ",\"t\":" + String(t) + ",\"h\":" + String(h) + ",\"s\":" + String(s) + ",\"Batt\":" + String(Batt) + ", \"Load\":" + String(Load) + ", \"Estimated_runtime\":" + String(Estimated_runtime) + ", \"Statusups\":\"" + String(Statusups) + "\", \"Ac_status\":" + (Ac_status ? "true" : "false") + "}}"; 
    Serial.println(payload);
    payload.toCharArray(msg, (payload.length() + 1));
    client.publish("@shadow/data/update", msg);
  }
}
void checkSensorValues() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int val = analogRead(analogPin);
  int smoke = map(val, 0, 1023, 0, 100);
  float s = smoke;

  // ตรวจสอบอุณหภูมิ
  if (t > Settemp && !tempAlertSent) {
    LINE.notify("อุณหภูมิภายในตู้สูงกว่าปกติ!! โปรดตรวจสอบ!");
    tempAlertSent = true;
  } else if (t <= Settemp) {
    tempAlertSent = false;  // รีเซ็ตเมื่ออุณหภูมิต่ำลง
  }

  // ตรวจสอบความชื้น
  if (h > Sethumi && !humiAlertSent) {
    LINE.notify("ความชื้นสูงกว่าปกติ!");
    humiAlertSent = true;
  } else if (h <= Sethumi) {
    humiAlertSent = false;  // รีเซ็ตเมื่อความชื้นต่ำลง
  }

  // ตรวจสอบควัน
  if (s > 40 && !smokeAlertSent) {
    LINE.notify("ตรวจพบควัน!ตัดการจ่ายไฟแล้ว โปรตรวจสอบและ Restart ระบบใหม่!");
    smokeAlertSent = true;
    if (ssrEnabled) {
      digitalWrite(ssr, LOW);  // ตัดการจ่ายไฟ
      ssrEnabled = false;
    }
  } else if (s <= 40) {
    smokeAlertSent = false;  // รีเซ็ตเมื่อไม่มีควัน
  }

  // ตรวจสอบควันและอุณหภูมิสูงพร้อมกัน
  if (s > 45 && t > 50) {
    LINE.notify("ตรวจพบควันและอุณภูมิสูงกว่าปกติ! ตัดการจ่ายไฟแล้ว โปรดตรวจสอบและ Restart ระบบใหม่!");
    if (ssrEnabled) {
      digitalWrite(ssr, LOW);  // ตัดการจ่ายไฟ
      ssrEnabled = false;
    }
  }
}

void checkNFC() {
  uint8_t readSuccess;
  uint8_t cardUid[7] = { 0, 0, 0, 0, 0, 0, 0 };
  uint8_t uidLength;

  readSuccess = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &cardUid[0], &uidLength);
  if (readSuccess) {
    Serial.println("Card detected!");
    String uidValue = "";
    for (uint8_t i = 0; i < uidLength; i++) { //แปลงค่าไบต์ของ UID ฐาน 10 เป็น ฐาน 16
      uidValue += String(cardUid[i], HEX);
    }
    if (uidValue.equals("e61f17ad")) {
      digitalWrite(relay, HIGH);
      delay(5000);
      digitalWrite(relay, LOW);
    }
    LINE.notify("UID of RFID/NFC card: " + uidValue);
  }
}

String getMsg(String topic_, String message_) {
  String tpc;
  if (topic_ == "@msg/Relay1") {
    if (message_ == "ON") {  // netpie["???"].publish("@msg/Relay1","R01_OFF")
      digitalWrite(relay, HIGH);
      Serial.println("Relay ON");
      delay(5000);
      digitalWrite(relay, LOW);
    }
  }
  if (topic_ == "@msg/AC") {
    if (message_ == "ON") {
      if (Ac_status == true){
        Serial.println("AC OFF");
        irsend.sendNEC(0xFFA25D, 32);
        Ac_status = false;
      }else if (Ac_status == false) {
        Serial.println("AC ON");
        irsend.sendNEC(0xFFA25D, 32);
        Ac_status = true;
      }
    }
  }
  return tpc;
}

void sendAndDisplayResult(char command) {
  UPS.print(command);
  Serial.print("Command sent: ");
  Serial.println(command);
  delay(100);

  String upsResponse;
  while (UPS.available()) {  //อ่านข้อมูลจากพอร์ต RS232
    char c = UPS.read();
    Serial.print(c);
    upsResponse += c;
  }
  upsResponse.trim();  //ลบช่องว่างข้อความ ตำแหน่งเริ่มต้นและสิ้นสุด เช่น \n 
  if (command == 'P') {
    Load = upsResponse.toFloat();
    Serial.print("Load status received: ");
    Serial.println(Load);
  } else if (command == 'f') {
    Batt = upsResponse.toFloat();
    Serial.print("Batt status received: ");
    Serial.println(Batt);
  } else if (command == 'j') {
    Estimated_runtime = upsResponse.toFloat();
    Serial.print("Time received: ");
    Serial.println(Estimated_runtime);
  } else if (command == 'Q') {
    if (upsResponse == "08") {
      Statusups = "On line Battery OK";
    } else if (upsResponse == "10") {
      Statusups = "On battery Battery OK";
    } else if (upsResponse == "28") {
      Statusups = "On line Overload";
    } else if (upsResponse == "88") {
      Statusups = "On line Replace battery";
    } else if (upsResponse == "30") {
      Statusups = "On battery Overload";
    } else if (upsResponse == "50") {
      Statusups = "On battery Low battery";
    } else if (upsResponse == "90") {
      Statusups = "On battery Replace battery";
    } else if (upsResponse == "20") {
      Statusups = "Overload";
    } else if (upsResponse == "80") {
      Statusups = "Replace battery";
    } else if (upsResponse == "60") {
      Statusups = "Low battery Overload";
    } else {
      Statusups = "Status not specified:" + upsResponse;
    }
    Serial.print("Status received: ");
    Serial.println(Statusups);
  }
}
void RemoteAc() {
  // เวลาผ่าน 3 วิ ให้ยิงสัญญาณ 1 ครั้ง
  if (millis() - lastSendTime >= 3000) {
    if (SetActemp != Actemp) {
     irsend.sendNEC(0xFFC23D, 32);
      Serial.println("ยิงรีโมทปลุกแล้ว");
      lastSendTime = millis();  // อัปเดตเวลาที่ส่งรีโมทล่าสุด
       delay(1500);
    }
  }

  // เพิ่มอุณหภูมิ
  while (Actemp < SetActemp) {
    Serial.println("เพิ่มอุณหภูมิ");
    irsend.sendNEC(0xFF22DD, 32);
    delay(1700);
    Actemp++;
    lastSendTime = millis();
  }

  // ลดอุณหภูมิ
  while (Actemp > SetActemp) {
    Serial.println("ลดอุณหภูมิ");
    irsend.sendNEC(0xFFC23D, 32);
    delay(1700);
    Actemp--;
    lastSendTime = millis();
  }

  while (Acfans < SetAcfans) {
    Serial.println("เพิ่มพัดลม");
    irsend.sendNEC(0xFFE01F, 32);
    delay(1000);
    Acfans++;
  }


  while (Acfans > SetAcfans) {
    Serial.println("ลดพัดลม");
    irsend.sendNEC(0xFF609F, 32);
    delay(1000);
    Acfans--;
  }

  Serial.print("SetActemp: ");
  Serial.print(SetActemp);
  Serial.print(" Actemp: ");
  Serial.println(Actemp);

  Serial.print("SetAcfans: ");
  Serial.print(SetAcfans);
  Serial.print(" Acfans: ");
  Serial.println(Acfans);
}

void checkPowerStatus() {
  float voltage = pzem.voltage();

  if (isnan(voltage)) voltage = 0.0;

  Serial.print("Voltage: ");
  Serial.println(voltage);

  if (voltage == 0.0 && powerOn) {
    // ไฟดับ
    powerOn = false;
    LINE.notify("ไฟฟ้าจากอาคารดับ ");
    Ac_status = false;
  } else if (voltage >= 1.0 && !powerOn) {
    // ไฟกลับมา
    powerOn = true;
    LINE.notify("ไฟฟ้ากลับมาแล้ว");
    delay(1000);
    Ac_status = true;
    irsend.sendNEC(0xFFA25D, 32);
    delay(1500);
    Actemp = 16;
    Acfans = 3;
  }
  if (Batt <= 15 && !powerOn && !SendSoft_shutdown) {
    sendAndDisplayResult('S');
    LINE.notify("แบตเตอรี่ UPS เหลือน้อย กำลังจะปิดตัวลงใน 20 วินาที และจะจ่ายไฟเมื่อไฟฟ้ากลับมา");
    SendSoft_shutdown = true;
  }
}