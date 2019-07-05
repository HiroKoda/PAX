//Based on BLE client example
//サーバーが停止するとエラーで回復できない
 
#include "BLEDevice.h"
#include "Wire.h"
#include "ESP32Servo.h"
#include "esp_system.h"

//#include <Ticker.h>
//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(10, 11); // RX, TX

#define SERVICE_UUID        "e3344d55-d8b4-44f7-afa0-2813b39048cf"
#define CHARACTERISTIC_UUID "66315508-1570-47db-b6c6-84a952b860d4"
#define SERVER_NAME         "esp32devc"

Servo myservo;
//Servo miniservo;
//#define LEDC_CHANNEL_0 0
////use 10 bit precission for LEDC timer
//#define LEDC_TIMER_BIT 10
////use 50 Hz as a LEDC base frequency
//#define LEDC_BASE_FREQ 50
////ServoPWM pin
//#define SRV_PIN 25

int servoPin = 25;
int miniservoPin = 26;

static BLEUUID  serviceUUID(SERVICE_UUID);
static BLEUUID  charUUID(CHARACTERISTIC_UUID);
static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
int flag = 0; //フラグを立てる
volatile int16_t axRaw_i = 0, ayRaw_i = 0 , azRaw_i = 0 ; //初期設定加速度
volatile int16_t axRaw, ayRaw, azRaw ;
volatile int mal = 0;
int pos = 0;
int val = 0;
int aaa;
int bbb;

//Ticker tick;
 
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
//    Serial.print("Notify callback for characteristic ");
//    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
//    Serial.print(" of data length ");
//    Serial.println(length);
}
 
 
bool connectToServer(BLEAddress pAddress) {
//    Serial.print("Forming a connection to ");
//    Serial.println(pAddress.toString().c_str());
    BLEClient*  pClient  = BLEDevice::createClient();
    pClient->connect(pAddress);
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
//    Serial.print(" - Connected to server :");
//    Serial.println(serviceUUID.toString().c_str());
   if (pRemoteService == nullptr) {
//      Serial.print("Failed to find our service UUID: ");
//      Serial.println(serviceUUID.toString().c_str());
     return false;
    }
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
//    Serial.print(" - Found our characteristic UUID: ");
//    Serial.println(charUUID.toString().c_str());
    if (pRemoteCharacteristic == nullptr) {
//      Serial.print("Failed to find our characteristic UUID: ");
      return false;
    }
//    Serial.println(" - Found our characteristic");
    pRemoteCharacteristic->registerForNotify(notifyCallback);
}
 
 
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
//    Serial.print("BLE Advertised Device found: ");
//    Serial.println(advertisedDevice.toString().c_str());//Name: esp32devc, Address: 30:ae:a4:02:a3:fe, txPower: -21
//    Serial.println(advertisedDevice.getName().c_str());//esp32devc
    
    if(advertisedDevice.getName()==SERVER_NAME){ //
//      Serial.println(advertisedDevice.getAddress().toString().c_str());
      advertisedDevice.getScan()->stop();
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;
    }
  }
};
 
 
void setup() {
  pinMode(13,OUTPUT); //バッテリロックピン
  pinMode(12,OUTPUT);
 
  pinMode(2, OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(17,OUTPUT);
  pinMode(4,OUTPUT);
  Serial.begin(115200);
  Wire.begin(21,22);
//  Serial.println("BLE_DataClient180130");
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
//  Serial.println("getScan");
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
//  Serial.println("setAdvertisedDeviceCallbacks");
  pBLEScan->setActiveScan(true);
  pBLEScan->start(10);
//  Serial.println("");
//  Serial.println("End of setup");

  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);
  Wire.write(0x08);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();

// ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
// ledcAttachPin(SRV_PIN, LEDC_CHANNEL_0);
// ledcWrite(0, 70);
  myservo.setPeriodHertz(50);    
  myservo.attach(servoPin, 800, 5000); 
 
}
 
 
void loop() {
//  Wire.beginTransmission(0x68);
//  Wire.write(0x3B);
//  Wire.endTransmission();
//  Wire.requestFrom(0x68, 14);
//  Wire.available() < 14;
//
//if(flag == 0){
//  for(int i=0;i<100;i++){
//    axRaw_i += Wire.read() << 8 | Wire.read();
//    ayRaw_i += Wire.read() << 8 | Wire.read();
//    azRaw_i += Wire.read() << 8 | Wire.read();
//    delay(10);
//    }
//    axRaw_i = axRaw_i / 100;
//    ayRaw_i = ayRaw_i / 100;
//    azRaw_i = azRaw_i / 100;
//    flag = 1 ;
// }
 
  if (doConnect == true) {
    if (connectToServer(*pServerAddress)) {
//      Serial.println("We are now connected to the BLE Server.");
      connected = true;
    } else {
//      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
      connected = false;
    }
    doConnect = false;
  }
  if (connected) {
//    digitalWrite(5, HIGH);
    std::string value = pRemoteCharacteristic->readValue();
    String strVal=value.c_str();
    int strNum=strVal.toInt();
    Serial.println(strNum);
    digitalWrite(4,HIGH);
//    acceleration();
    digitalWrite(4,LOW);
//    
//    if(strNum <= 20 && aaa == 0){
      if(strNum == 1){
//       digitalWrite(17,LOW); //ロック解除
//       digitalWrite(16,HIGH);
       aaa = 1;
       
     digitalWrite(13,HIGH); //バッテリーロック施錠
     digitalWrite(12,LOW);
      delay(20);
     digitalWrite(13,LOW); 
     digitalWrite(12,LOW);  
      //ledcWrite(0, 130);
       
       
//      } else if(strNum >20 && mal == 0 && aaa == 1){
        } else if(strNum == 0){
//        digitalWrite(16,LOW); //ロック施錠
//        digitalWrite(17,HIGH);
        aaa = 0;
        bbb = 0;

     digitalWrite(13,LOW); //バッテリーロック解除
     digitalWrite(12,HIGH);
      delay(20);
     digitalWrite(13,LOW); 
     digitalWrite(12,LOW);  
     
   // ledcWrite(0, 30);
     
       } else if(strNum == 2){
        aaa = 2; 
        }

  
// if(val == 0 && bbb == 1){
//  
//   digitalWrite(13,HIGH); //バッテリーロック施錠
//   digitalWrite(12,LOW);
//    delay(50);
//   digitalWrite(13,LOW); 
//   digitalWrite(12,LOW);  
//  
//  ledcWrite(0, 188);
//   delay(1500);
// } else if(val == 1 && bbb == 1){
//
//   digitalWrite(13,LOW); //バッテリーロック解除
//   digitalWrite(12,HIGH);
//    delay(50);
//   digitalWrite(13,LOW); 
//   digitalWrite(12,LOW);  
//   
//  ledcWrite(0, 3);
//   delay(1500);
// }
// bbb = 0;
satsuei();
  } else{
//    Serial.println("Not connected");
    doConnect = true;
     delay(100);
  }
//  delay(1000);
//  digitalWrite(5, LOW);
 }

void acceleration() {
  int axRaw_sum = 0;
  int ayRaw_sum = 0;
  int azRaw_sum = 0;
  
 for(int i=0;i<100;i++){
  axRaw = Wire.read() << 8 | Wire.read();
  ayRaw = Wire.read() << 8 | Wire.read();
  azRaw = Wire.read() << 8 | Wire.read();
  axRaw_sum += axRaw - axRaw_i;
  ayRaw_sum += ayRaw - ayRaw_i;
  azRaw_sum += azRaw - azRaw_i;
   delay(10);
 }
  axRaw = axRaw_sum / 100;
  ayRaw = ayRaw_sum / 100;
  azRaw = azRaw_sum / 100;
    
//  Serial.print("accel_X = ");
//  Serial.print(axRaw); Serial.print(" , ");
//  Serial.print("accel_Y = ");
//  Serial.print(ayRaw); Serial.print(" , ");
//  Serial.print("accel_Z = ");
//  Serial.print(azRaw); Serial.print(" , "); 
    
  if(abs(axRaw) <= 20 || abs(ayRaw) <= 20){
    mal = 1;
//    Serial.println(mal);
    } else{
      mal = 0;
      }
}

void satsuei(){
  if(aaa == 1 && val == 1 && bbb == 0){ 
  val = 0;
  bbb = 1;
 myservo.attach(servoPin, 800, 5000);
  for (pos = 0; pos <= 220; pos += 1) { 
    myservo.write(pos);   
    delay(15);             
  }
  Serial.println("Right");
  
  } else if(aaa == 1 && val == 0 && bbb == 0){
  val = 1;
  bbb = 1;
 myservo.attach(servoPin, 800, 5000);
  for (pos = 220; pos >= 0; pos -= 1) {  
    myservo.write(pos);    
    delay(15);             
  }
  Serial.println("Left");
   } 

//  if(aaa == 2 && val == 1 && bbb == 0){
//    val = 0;
//    bbb = 1;
//    myservo.attach(miniservoPin, 800, 5000);
//    for (pos = 0; pos <= 180; pos += 1) { 
//      myservo.write(pos);   
//      delay(2);             
//    }
//  } else if(aaa == 2 && val == 0 && bbb == 0)
//      val = 1;
//      bbb = 1;
//     myservo.attach(miniservoPin, 800, 5000);
//      for (pos = 180; pos >= 0; pos -= 1) {  
//        myservo.write(pos);    
//        delay(15);             
//       }
 }      
