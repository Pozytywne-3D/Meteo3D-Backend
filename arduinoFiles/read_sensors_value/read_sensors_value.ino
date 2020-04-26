#include <WEMOS_SHT3X.h>
#include <Adafruit_BMP085.h>
#include <SDS011.h>
#include <ESP8266WiFi.h>

char ssid[] = "#";
char password[] = "#";

#define RXpin D6
#define TXpin D7

SDS011 sds;
Adafruit_BMP085 bmp;
SHT3X sht30(0x45);

float pm10, pm25;
boolean ifError;

void setup() {
  Serial.begin(9600);
  sds.begin(RXpin, TXpin);
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid,password);

    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected successfully");

}

void loop() {
  ifError = sds.read(&pm25,&pm10);
  
  Serial.println("-----------------");
  
  if(ifError){
    Serial.println("Error sds011");
  }else{
    Serial.println("PM 1.0: " + String(pm10));
    Serial.println("PM 2.5: " + String(pm25));
  }
  
  Serial.println("Pressure: " + String(bmp.readPressure()/100));
  
  if(sht30.get()==0){
    Serial.println("Temperature: " + String(sht30.cTemp));
    Serial.println("Humidity: " + String(sht30.humidity));
  }else{
    Serial.println("Error sht30");
  }
  
  delay(3000);
}
