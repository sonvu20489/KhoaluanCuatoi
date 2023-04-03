
#include "MyRFID.h"

#include "Myfinger.h"
#include "MyLCD.h"
#include "MyWarning.h"

#include "Mykeypad.h"
bool is_door_close = 1;
uint8_t prohibit = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("This is test LCD");
  init_LCD();
  init_keypadcheck();
  init_RFID();
  init_warning();
  finger_init();
}

void loop() {
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
    handle_key();
    handle_RFID();
    handle_finger();
  }
  emergency_btn();
  Warning_door_open();
  Warning_wrongPass();
  // put your main code here, to run repeatedly:

}



