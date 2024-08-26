#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h> 
#include "stringprocesses.h"
#include "Wire.h"
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 32, en = 4, d4 = 27, d5 = 22, d6 = 16, d7 = 17;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define SS_PIN 21
#define RST_PIN 33

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
const char* ssid = "xavier";
const char* password = "xyvielyonsmbugua";

// Your Domain name with URL path or IP address with path
// const char* serverName = "http://192.168.0.100:3001/api/arduino/postmessage";
const char* serveraddress = "192.168.0.100";


WiFiClient client;
HttpClient http = HttpClient(client, serveraddress, 3001);
StringProcesses stringoperation;

void setup() 
{
  WiFi.begin(ssid, password);
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  lcd.begin(20, 4);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Attendance system");
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Connecting to wifi network");
  lcd.setCursor(0,2);
  lcd.print("connecting.....");
  lcd.setCursor(0,3);
  lcd.print("please wait....");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  lcd.setCursor(0,2);
  lcd.print("network connected.");
  lcd.setCursor(0,3);
  lcd.print("successfully......");
  Serial.println("");  

  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  delay(2500);
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print("please tap your tag");
  
  

}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
    

    if(WiFi.status()==WL_CONNECTED){
      JsonDocument doc;
      String uid;
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        uid += String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        uid += String(mfrc522.uid.uidByte[i], HEX);  

      }

      uid.toUpperCase();
      lcd.setCursor(0,1);
      lcd.print("uid:");
      lcd.setCursor(5,1);
      lcd.print(uid);
    
      
      String removedfirstspace = stringoperation.removeFirstSpace(uid); 
      doc["rfiduuid"] = removedfirstspace;
      char jsonBuffer[100];
      serializeJson(doc,jsonBuffer);

      String contentType = "application/json";
      http.post("/api/arduino/postmessage",contentType,jsonBuffer);

      // read the status code and body of the response
        int statusCode = http.responseStatusCode();
        String response = http.responseBody();

        Serial.print("Status code: ");
        signed int errorcode = -2;
        signed int okcode = 200;
        Serial.println(statusCode);
        if(statusCode == errorcode){
          lcd.setCursor(0,2);
          lcd.print("server error        ");
          lcd.setCursor(0,3);
          lcd.print("try again later     ");

        }
        Serial.print("Response: ");
        Serial.println(response);

        if(statusCode == okcode){
          lcd.setCursor(0,2);
          lcd.print("your attendance     ");
          lcd.setCursor(0,3);
          lcd.print("has been logged     ");
          

        }
        
          

          delay(2000);
          lcd.setCursor(0,1);
          lcd.print("                    ");
          lcd.setCursor(0,2);
          lcd.print("                    ");
          lcd.setCursor(0,3);
          lcd.print("                    ");
      

    }else{
      Serial.println("WiFi Disconnected");
    }
} 

