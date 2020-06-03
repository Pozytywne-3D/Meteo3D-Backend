#include <SDS011.h>
#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <WEMOS_SHT3X.h>
#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h>

char ssid[] = "Hallmann";
char password[] = "Hallmann246";


#define FIREBASE_HOST "meteo3d-d1068.firebaseio.com"
#define FIREBASE_AUTH "qwerty123"
#define RXpin D6
#define TXpin D7

SDS011 sds;
Adafruit_BMP085 bmp;
SHT3X sht30(0x45);

boolean startingUp = true;

String stationLocation = "2";

boolean ifError;
String data;

int airtemp, airhum;
float pm10, pm25;

unsigned long startingTime, sleepTime;
int measurementNumber;

DynamicJsonBuffer jsonBuffer;
  

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  sds.begin(RXpin, TXpin);
  sds.sleep();
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }
  
  Serial.println();
  Serial.println("Connecting to " + String(ssid));

  WiFi.begin(ssid,password);

  int wifiAttempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifiAttempts++;
    if(wifiAttempts>100){
        Serial.println("Wifi error");
        Serial.println("Restarting...");
        ESP.restart(); 
    }
  }
  Serial.println();
  Serial.println("Wi-Fi connected successfully");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  measurementNumber = Firebase.getInt("measurements/"+stationLocation+"/lastMeasurement");
}


void loop() {
  measurementNumber++;
  startingTime = millis();
  sds.wakeup();
  Serial.println("Starting SDS011...");
  for(int i=0;i<60;i++){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(800);
    digitalWrite(LED_BUILTIN, LOW);
    delay(200);
  }
  
  ifError = sds.read(&pm25,&pm10);
  if(ifError){
    Serial.println("Error sds011 <<========");
    pm10 = -100;
    pm25 = -100;
  }
  
  if(sht30.get()==0){
    airtemp = sht30.cTemp;
    airhum = sht30.humidity;
  }else{
    Serial.println("Error sht30 <<========");
    airtemp = -100;
    airhum = -100;
  }

  int atmpress = bmp.readPressure()/100;
  if(atmpress < 900 || atmpress > 1100){
    atmpress = -100;
  }
  
  Serial.println("Pressure: " + String(atmpress));
  Serial.println("Temperature: " + String(airtemp));
  Serial.println("Humidity: " + String(airhum));
  Serial.println("PM 1.0: " + String(pm10));
  Serial.println("PM 2.5: " + String(pm25));

  String jsonInput =
      "{\"airhum\":"+String(airhum)+
      ",\"airtemp\":"+String(airtemp)+
      ",\"atmpress\":"+String(atmpress)+
      ",\"iaqpm10\":"+String(pm10)+
      ",\"iaqpm25\":"+String(pm25)+"}";
  JsonObject& data = jsonBuffer.parseObject(jsonInput);

  Firebase.set("measurements/"+stationLocation+"/m"+measurementNumber, data);
  if(Firebase.success()){
      Firebase.set("measurements/"+stationLocation+"/lastMeasurement", measurementNumber);
  }else{
    Firebase.pushString("errors/", stationLocation+" "+measurementNumber);
    measurementNumber--;
    if(!Firebase.success()){
      Serial.println("Firebase error");
      if(!startingUp){
        Serial.println("Restarting...");
        ESP.restart(); 
      } 
    }
  }
  
  for(int i=0;i<20;i++){
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
  }

  // shuting down WiFi
  if(startingTime>=millis()){
    sleepTime = 59*60*1000;
    Serial.println("millis() overflow - inaccurate sleep time");
  }else{
    sleepTime = 60*60*1000 - (millis() - startingTime);
  }
  Serial.println("Sleep time: " + String(sleepTime));
  Serial.println("/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\");
  WiFi.forceSleepBegin();
  sds.sleep();
  delay(sleepTime);
  WiFi.forceSleepWake();  //WiFi on
  startingUp = false;
}
