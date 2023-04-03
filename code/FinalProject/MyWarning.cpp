#include "Arduino.h"
#include "MyWarning.h"
void init_warning()
{
  pinMode(LRED_PIN,OUTPUT);
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(LATCH_PIN,OUTPUT);
  pinMode(BTN_URGENT_PIN,INPUT);
  pinMode(DOOR_SEN_PIN,INPUT);
  digitalWrite(LATCH_PIN,LOW);
  digitalWrite(LRED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
}

void accgranted()
{
  is_door_close = 0;
  prohibit = 0;
  countdown = 20;
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
  }
}
void Close_door()
{
  is_door_close = 1;
  PrintL("0s ",3,7);
  digitalWrite(LATCH_PIN,LOW);
  digitalWrite(LRED_PIN,HIGH);
  digitalWrite(BUZZER_PIN,LOW);
  if(menu->MenuID == ACCMENU)
    PrintL("|CLOSE",0,14);
}
void Warning_door_open()
{
  if(is_door_close == 0)
  {
    if(countdown == 0 )
    {
      if(btn_urgent == 0)
      {
        if(millis()-Counter1>=4000)
        {
          digitalWrite(BUZZER_PIN,LOW);
          Counter1 = millis();
        }
        else if(millis()-Counter1>=2000)
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
          lastTime = millis();
        }
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
  if((prohibit>=7)&&(btn_urgent == 0))
  {
    if(millis()-timeWP >= 60000)
    {
      prohibit = 0;
      digitalWrite(BUZZER_PIN,LOW);
      Serial.println("WP: off Warning");
    }
    else
    {
      if(millis()-timeBuzzer >= 8000)
      {
        digitalWrite(BUZZER_PIN, LOW);
        Serial.println("WP: BUZZER OFF");
        timeBuzzer = millis();
      }
      else if(millis()-timeBuzzer >= 4000)
      {
        digitalWrite(BUZZER_PIN,HIGH);
        Serial.println("WP: BUZZER ON");
      }
    }
  }
}
void emergency_btn()
{
  if(is_door_close == 0 || prohibit >= 7)
  {
    if(digitalRead(BTN_URGENT_PIN) == HIGH)
    {
      while(BTN_URGENT_PIN == HIGH);
      btn_urgent =!btn_urgent;
      digitalWrite(BUZZER_PIN,LOW);
      Serial.println(btn_urgent);      
    }
  }
}