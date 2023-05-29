#include "MyWarning.h"
#include "Mykeypad.h"
#include "MyLCD.h"
#include "global.h"

bool doorstate;
bool lastdoorstate;
extern RTC_DS1307 rtc;
char Time[50];
int sendWTimes = 0;
void init_warning()
{
  pinMode(LRED_PIN,OUTPUT);
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(LATCH_PIN,OUTPUT);
  pinMode(DOOR_SEN_PIN,INPUT);
  digitalWrite(LATCH_PIN,LOW);
  digitalWrite(LRED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
}

void accgranted(const char * name)
{
  DateTime now = rtc.now();
  sprintf(Time,"%d-%02d-%02d %02d:%02d:%02d",
  now.year(),
  now.month(),
  now.day(),
  now.hour(),
  now.minute(),
  now.second()
  );
  char message[50] = "Door is opened by ";
  strcat(message,name);
  sendJson(message,Time);
  is_door_close = 0;
  prohibit = 0;
  countdown = 20;
  sendWTimes = 0;
  lcd.clear();
  digitalWrite(LATCH_PIN,HIGH);
  digitalWrite(LRED_PIN,LOW);
  PrintL("  Successfully!    ",1,0);
  PrintL("  ACCESS GRANTED!  ",2,0);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);
  PrintL("   Door is opened  ",2,0); 
  delay(1000);
  lastTime = millis();
  Counter1 = millis();
  Serial.println("{\"photo\":\"1\"}");
}
void accdenied()
{
  prohibit++;
  lcd.clear();
  PrintL("      Failed!      ",1,0);
  PrintL("  ACCESS DENIED!   ",2,0);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);
  delay(500);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);
  delay(500);
  if(prohibit == 7)
  {
    timeWP = millis();
    timeBuzzer = millis();
    DateTime now = rtc.now();
    sprintf(Time,"%d-%02d-%02d %02d:%02d:%02d",
    now.year(),
    now.month(),
    now.day(),
    now.hour(),
    now.minute(),
    now.second()
    );
    sendJson("Illegal access warning!",Time);
    Serial.println("{\"Warning\":\"1\"}");
  }
}
void Close_door()
{
  DateTime now = rtc.now();
  sprintf(Time,"%d-%02d-%02d %02d:%02d:%02d",
  now.year(),
  now.month(),
  now.day(),
  now.hour(),
  now.minute(),
  now.second()
  );
  sendJson("Door is closed",Time);
  is_door_close = 1;
  digitalWrite(LATCH_PIN,LOW);
  digitalWrite(LRED_PIN,HIGH);
  digitalWrite(BUZZER_PIN,LOW);
  if(menu->MenuID == ACCMENU)
  {
    PrintL("|CLOSE",0,14);
    PrintL("0s ",3,7);
  }
  countdown = 0;
  btn_urgent = 0;
}
void Warning_door_open()
{
  if(is_door_close == 0)
  {
    if(countdown == 0 )
    {
      if(btn_urgent == 0)
      {
        if(millis()-Counter1>=500)
        {
          digitalWrite(BUZZER_PIN,LOW);
          Counter1 = millis();
        }
        else if(millis()-Counter1>=250)
        {
          digitalWrite(BUZZER_PIN,HIGH);
        }
      }
    }
    else
    {
      if(millis()-lastTime >= 1000)
      {
        countdown--;
        if(menu->MenuID== ACCMENU)
        {
          lcd.setCursor(7,3);
          lcd.print(countdown);
          lcd.print("s ");
        }
        lastTime = millis();
      }
    }
    if(lastdoorstate == HIGH && doorstate == LOW)
    {
      countdown = 0;
      Close_door();
    }
  }
}
void Warning_wrongPass()
{
  if((prohibit>=7))
  {
    if(millis()-timeWP >= 60000)
    {
      prohibit = 0;
      digitalWrite(BUZZER_PIN,LOW);
    }
    else
    {
      if(millis()-timeBuzzer >= 2000)
      {
        sendWTimes++;
        digitalWrite(BUZZER_PIN, LOW);
        if(sendWTimes == 5)
        {
          sendWTimes = 0;
          Serial.println("{\"Warning\":\"1\"}");
        }
        timeBuzzer = millis();
      }
      else if(millis()-timeBuzzer >= 1000)
      {
        digitalWrite(BUZZER_PIN,HIGH);
      }
    }
  }
}
void emergency_btn()
{
  if(is_door_close == 0)
  {
    if(customkey == '#')
    {
      DateTime now = rtc.now();
      sprintf(Time,"%d-%02d-%02d %02d:%02d:%02d",
      now.year(),
      now.month(),
      now.day(),
      now.hour(),
      now.minute(),
      now.second()
      );
      if(btn_urgent == 1)
      {
        sendJson("Turn off the Warning",Time);
      }
      else
      {
        sendJson("Turn on the Warning",Time);
      }
      btn_urgent =!btn_urgent;
      digitalWrite(BUZZER_PIN,LOW);
      Serial.println(btn_urgent);      
    }
  }
}
void sendJson(const char * message, const char * time)
{
  String datasend;
  datasend = "{\"message\":\"" + String(message) + "\"," +
             "\"time\":\"" + String(time) + "\"}";
  Serial.println(datasend);
  Serial.flush();
}