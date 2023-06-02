#include <Arduino.h>
#include <HTTPClient.h>
#include "esp_camera.h"
#include <WiFi.h>
#include "esp_timer.h"
#include "img_converters.h"
#include <pgmspace.h>
String apikey = "2f2eb859-e213-4cee-909d-c358a59bc9c5";
String serverName = "khoacuaonline.000webhostapp.com";   //REPLACE WITH YOUR DOMAIN NAME
String Upload_Path = "/upload.php";     // The default serverPath should be upload.php
const int serverPort = 80; //server port for HTTPS
WiFiClient client;
/*=================================IP to String=====================================================*/
String IpAddress2String(const IPAddress& ipAddress)
{
    return String(ipAddress[0]) + String(".") +
           String(ipAddress[1]) + String(".") +
           String(ipAddress[2]) + String(".") +
           String(ipAddress[3]);
}
/*=========================================Send message of ESp32 tranfer ================================*/
static void send_message(const char * message, const char * time)
{
  if(!client.connect(serverName.c_str(),serverPort))
  {
    Serial.println("connection failed");
    return ;
  }
  String path_url = "/history_door/";
  path_url += "sendmes.php";
  String data_encode = "ser_code=" + apikey + 
  "&message=" + message + "&time=" + time;
  client.print("POST " + path_url + " HTTP/1.1\r\n" +
               "Host: " + serverName + "\r\n" + 
               "Content-Type: application/x-www-form-urlencoded" + "\r\n"+
               "Content-Length: " + String(data_encode.length()) + "\r\n\r\n");
  Serial.println(data_encode);
  client.print(data_encode);
  unsigned long timeout = millis();
  while (client.available() == 0) {
      if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
      }
  }
  while(client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
  }
  client.stop();
}
/*=============================Get private address of ESP Cam TO watch the Camera===================*/
static void update_url_stream(String URL)
{
  if(WiFi.status()==WL_CONNECTED)
  {
    String getBody;
    String getAll;
    if (client.connect(serverName.c_str(), serverPort)) {
      Serial.println("Connection successful!"); 
      String Path_url_strstream = "/action.php?ser_code=" + apikey +"&url=" + URL;
      client.println("GET " + Path_url_strstream + " HTTP/1.1\r\n" + 
                    "Host: " + serverName + "\r\n" + 
                    "Connection: close\r\n\r\n");
      int timoutTimer = 10000;
      long startTimer = millis();
      boolean state = false;
      while ((startTimer + timoutTimer) > millis()) {
        Serial.print(".");
        delay(10);      
        while (client.available()) {
          char c = client.read();
          if (c == '\n') {
            if (getAll.length()==0) { state=true; }
            getAll = "";
          }
          else if (c != '\r') { getAll += String(c); }
          if (state==true) { getBody += String(c); }
          startTimer = millis();
        }
        if (getBody.length()>0) { break; }
      }
      client.stop();
    }
    else
    {
      Serial.println("failed Connection");
    }
  }
}
/*===================================Send data init RFID Card============================================*/
static bool sendReadRFID(String sendRFID, String sendslot, String sendName)
{
  if(WiFi.status()!= WL_CONNECTED)
  {
//    Serial.print("Not connect to WiFi");
    return 0;
  }
  if(!client.connect(serverName.c_str(),serverPort))
  {
//    Serial.println("connection failed");
    return 0;
  }
  String url = "/monitor/read_RFID.php?";
  url += "ser_code=" + apikey;
  String data_encode = "RFID=" + sendRFID + "&slot=" + sendslot + "&Name=" + sendName;
  client.print("POST " + url + " HTTP/1.1\r\n" +
               "Host: " + serverName + "\r\n" + 
               "Content-Type: application/x-www-form-urlencoded" + "\r\n"+
               "Content-Length: " + String(data_encode.length()) + "\r\n\r\n");
//  Serial.println(data_encode);
  client.print(data_encode);
  unsigned long timeout = millis();
  while (client.available() == 0) {
      if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return 0;
      }
  }
  while(client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
  }
  client.stop();
  return 1;
}/*================================Send data init Finger ===================================*/
static bool sendReadFinger(String sendID, String sendName)
{
  if(WiFi.status()!= WL_CONNECTED)
  {
    return 0;
  }
  if(!client.connect(serverName.c_str(),serverPort))
  {
    return 0;
  }
  String url = "/monitor/read_Finger.php?";
  url += "ser_code=" + apikey;
  String data_encode = "id=" + sendID + "&Name=" + sendName;
  Serial.println(data_encode);
  client.print("POST " + url + " HTTP/1.1\r\n" +
               "Host: " + serverName + "\r\n" + 
               "Content-Type: application/x-www-form-urlencoded" + "\r\n"+
               "Content-Length: " + String(data_encode.length()) + "\r\n\r\n");
//  Serial.println(data_encode);
  client.print(data_encode);
  unsigned long timeout = millis();
  while (client.available() == 0) {
      if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return 0;
      }
  }
  while(client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
  }
  client.stop();
  return 1;
}
/*=========================================*/
static bool sendaction(String type, String action, String id, String name, String Code)
{
  if(WiFi.status()!= WL_CONNECTED)
  {
    return 0;
  }
  if(!client.connect(serverName.c_str(),serverPort))
  {
    return 0;
  }
  String url = "/monitor/store.php";
  String data_encode = "ser_code=" + apikey + "&type=" + type + "&action=" + action + "&id=" +id 
  + "&Name="+name + "&RFID_code=" + Code ;

  Serial.println(data_encode);

  client.print("POST " + url + " HTTP/1.1\r\n" +
               "Host: " + serverName + "\r\n" + 
               "Content-Type: application/x-www-form-urlencoded" + "\r\n"+
               "Content-Length: " + String(data_encode.length()) + "\r\n\r\n");
//  Serial.println(data_encode);
  client.print(data_encode);
  unsigned long timeout = millis();
  while (client.available() == 0) {
      if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return 0;
      }
  }
  while(client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
  }
  client.stop();
  return 1;
}
/*============================================Send email warning=================================*/
static bool sendPhoto(const char * email)
{
  if(WiFi.status()!=WL_CONNECTED)
  {
    Serial.println("Not connect to wifi");
    return 0;
  }
  if(!client.connect(serverName.c_str(),serverPort))
  {
    Serial.println("connection failed");
    return 0;
  }
  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  if(!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }
  
  Serial.println("Connecting to server: " + serverName);
  String url = "/mail/mail.php?mailto=";
  url += String(email);
  
  String head = "--KhoaCuaAnNinh\r\nContent-Disposition: form-data; name=\"attachment\"; filename=\"imageShot.png\"\r\nContent-Type: image/jpeg\r\n\r\n";
  String tail = "\r\n--KhoaCuaAnNinh--\r\n";
  uint32_t totalLen = head.length() + fb->len+tail.length();
  client.println("POST " + url + " HTTP/1.1");
  client.println("Host: " + serverName);
  client.println("Content-Length: " + String(totalLen));
  client.println("Content-Type: multipart/form-data; boundary=KhoaCuaAnNinh");
  client.println();

  client.print(head);
  uint8_t *fbBuf = fb->buf;
  size_t fbLen = fb->len;
  for (size_t n=0; n<fbLen; n=n+1024) {
    if (n+1024 < fbLen) {
      client.write(fbBuf, 1024);
      fbBuf += 1024;
    }
    else if (fbLen%1024>0) {
      size_t remainder = fbLen%1024;
      client.write(fbBuf, remainder);
    }
  }
  client.print(tail);   
  esp_camera_fb_return(fb);
  unsigned long timeout = millis();
  while (client.available() == 0) {
      if (millis() - timeout > 5000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return 0;
      }
  }
  while(client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
  }
  client.stop();
  return 1;
}
/*====================================Sendphoto shot on open door ==============================*/
static bool sendPhoto() {
  String getAll;
  String getBody;
  if(WiFi.status()==WL_CONNECTED)
  {
    camera_fb_t * fb = NULL;
    fb = esp_camera_fb_get();
    if(!fb) {
      Serial.println("Camera capture failed");
      delay(1000);
      ESP.restart();
    }
    
    Serial.println("Connecting to server: " + serverName);
    
  //  client.setInsecure(); //skip certificate validation
    if (client.connect(serverName.c_str(), serverPort)) {
      Serial.println("Connection successful!");    
      String head = "--KhoaCuaAnNinh\r\nContent-Disposition: form-data; name=\"imageFile\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
      String tail = "\r\n--KhoaCuaAnNinh--\r\n";
  
      String apikey_header = String("--KhoaCuaAnNinh") + "\r\n";
      apikey_header+= "Content-Disposition: form-data; name=\"api\"\r\n\r\n";
      apikey_header+= String(apikey) + "\r\n";
      uint32_t imageLen = fb->len;
      uint32_t extraLen = apikey_header.length() + head.length() + tail.length() ;
      uint32_t totalLen = imageLen + extraLen;
     
      client.println("POST " + Upload_Path + " HTTP/1.1");
      client.println("Host: " + serverName);
      client.println("Content-Length: " + String(totalLen));
      client.println("Content-Type: multipart/form-data; boundary=KhoaCuaAnNinh");
      client.println();
      client.print(apikey_header);
      client.print(head);
      Serial.print(head);
      uint8_t *fbBuf = fb->buf;
      size_t fbLen = fb->len;
      for (size_t n=0; n<fbLen; n=n+1024) {
        if (n+1024 < fbLen) {
          client.write(fbBuf, 1024);
          fbBuf += 1024;
        }
        else if (fbLen%1024>0) {
          size_t remainder = fbLen%1024;
          client.write(fbBuf, remainder);
        }
      }   
      client.print(tail);
      
      esp_camera_fb_return(fb);
      
      int timoutTimer = 10000;
      long startTimer = millis();
      boolean state = false;
      
      while ((startTimer + timoutTimer) > millis()) {
        Serial.print(".");
        delay(10);      
        while (client.available()) {
          char c = client.read();
          if (c == '\n') {
            if (getAll.length()==0) { state=true; }
            getAll = "";
          }
          else if (c != '\r') { getAll += String(c); }
          if (state==true) { getBody += String(c); }
          startTimer = millis();
        }
        if (getBody.length()>0) { break; }
      }
      Serial.println();
      client.stop();
      Serial.println(getBody);
    }
    else {
      getBody = "Connection to " + serverName +  " failed.";
      esp_camera_fb_return(fb);
      Serial.println(getBody);
      return 0;
       
    }
  
  }
  return 1;
}
