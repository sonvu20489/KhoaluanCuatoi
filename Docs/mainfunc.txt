#include <Arduino.h>
#include <EEPROM.h>
#include "MyLCD.h"
#include "Mykeypad.h"
#include "MyWarning.h"
#include "MyRFID.h"
#include "Myfinger.h"
#include "global.h"
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
// #define EEPROMSIZE 525
#define SERIAL_BAUD 115200
#define ADD_SETTIME 520
bool is_door_close = 1;
uint8_t prohibit = 0;
char dateT [20];
RTC_DS1307 rtc;
bool InitWifi = 0;
LiquidCrystal_I2C lcd(0x27, 20, 4);
void setup() {
  // put your setup code here, to run once:
  rtc.begin();
  Serial.begin(SERIAL_BAUD);
  Serial.print("Hello this is LVTN");
  
  // EEPROM.begin(EEPROMSIZE);
  init_LCD();
  init_keypadcheck();
  lcd.clear();
  while(InitWifi == 0)
  {
    customkey = customKeypad.getKey();
    if(customkey == 'D')
    {
      InitWifi = 1;
    }
    ReadJson();
    PrintL("Wait for init WiFi",1,0);
    PrintL(" ESP32-CAM Config ",2,0);
    PrintL(" Press D to skip  ",3,0);
  }
  MenuDisplay(menu);
  init_RFID();
  init_warning();
  finger_init();
  if(EEPROM.read(ADD_SETTIME) != 127)
  {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    EEPROM.write(ADD_SETTIME,127);
    EEPROM.commit();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  customkey = customKeypad.getKey();
  lastdoorstate = doorstate;
  
  doorstate = digitalRead(DOOR_SEN_PIN);
  if(customkey == 'A' && menu->MenuID != ACCMENU)
  {
    if(menu->Activation1 != NULL)
      menu->Activation1(menu->MenuID,OPTION1); 
    if(ret_fr_fcntp != 1)
    {
      menu = (menu->Menulist1==NULL)?menu:menu->Menulist1;
      MenuDisplay(menu);
      
    }
    ret_fr_fcntp = 0;
  }
  else if(customkey == 'B' && menu->MenuID != ACCMENU)
  {
    if(menu->Activation2 != NULL)
      menu->Activation2(menu->MenuID,OPTION2); 
    if(ret_fr_fcntp != 1)
    {    
      menu = (menu->Menulist2==NULL)?menu:menu->Menulist2;
      MenuDisplay(menu);
    }
    ret_fr_fcntp = 0;
  }
  else if(customkey == 'C' && menu->MenuID != ACCMENU)
  {
    
    if(menu->Activation3 != NULL)
      menu->Activation3(menu->MenuID,OPTION3); 
    if(ret_fr_fcntp != 1)
    {
      menu = (menu->Menulist3==NULL)?menu:menu->Menulist3;
      MenuDisplay(menu);
    }
    ret_fr_fcntp = 0;
  }
  else if(customkey == 'D')
  { 
    
    if(menu->MenuID!= ADMINMENU && menu->MenuID!= ACCMENU)
    {
    menu = &AdminMenu;
    MenuDisplay(menu);
    }
    else if (menu->MenuID == ADMINMENU)
    {
      menu = menu->Menulist0;
      MenuDisplay(menu);
      lcd.setCursor(14,0);
      if(is_door_close == 0)
      {
        lcd.print("|OPEN ");
      }
      else
        lcd.print("|CLOSE");
    }
    
  }
  if(menu->MenuID == ACCMENU)
  {
    DateTime now = rtc.now();
    sprintf(dateT,"%02d/%02d %02d:%02d:%02d",
    now.day(),
    now.month(),
    now.hour(),
    now.minute(),
    now.second()
    );
    lcd.setCursor(0,0);
    lcd.print(dateT);
    handle_key();
    handle_RFID();
    handle_finger();
  }
  emergency_btn();
  Warning_door_open();
  Warning_wrongPass();

}
void ReadJson()
{
  while(Serial.available())
  {
    DynamicJsonDocument json(1024);
    DeserializationError error = deserializeJson(json, Serial);
    if(error)
    {
      return;
    }
    else
    {
      if(json["InitWiFi"] == "1")
      {
        InitWifi = 1;
      }
    }
  }
}
