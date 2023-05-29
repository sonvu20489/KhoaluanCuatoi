#include "Mykeypad.h"
#include "global.h"
#include "MyLCD.h"
#include "Myfinger.h"
#include "MyWarning.h"

char customkey;
bool ret_fr_fcntp = 0;

void init_keypadcheck()
{
  EEPROM.begin(EEPROMSIZE);
  if(EEPROM.read(ADD_PASS_INIT)!= 144)
  {
    for(int i = 0;i<6;i++)
    {
      EEPROM.write(ADD_PASS_STORE+i,Password[i]);
      EEPROM.commit();
    }
    EEPROM.write(ADD_PASS_INIT,144);
    EEPROM.commit();
  }
  
  if(EEPROM.read(ADD_ADPASS_INIT)!= 144)
  {
    for(int i = 0;i<8;i++)
    {
      EEPROM.write(ADD_ADPASS_STORE+i, keysofadmin[i]);
      EEPROM.commit();
    }
    EEPROM.write(ADD_ADPASS_INIT,144);
    EEPROM.commit();    
  }
  for(int i = 0;i<8;i++)
  {
    keysofadmin[i] = EEPROM.read(ADD_ADPASS_STORE + i);
  }
  for(int i = 0 ; i<6;i++)
  {
    Password[i] = EEPROM.read(ADD_PASS_STORE+i);
  }
  //Serial.println(keysofadmin);
  //Serial.println(Password);
}

boolean checkpass(char pass[],char input[])
{
  if(strstr(input,pass))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void changepassword(char * pass, char input[])
{
  strcpy(pass, input);
  for(int i = 0;i<6;i++)
  {
    EEPROM.write(ADD_PASS_STORE+i,pass[i]);
    EEPROM.commit();
  }
}
void changeADpassword(char * adpass, char input[])
{
  strcpy(adpass, input);
  for(int i = 0;i<8;i++)
  {
    EEPROM.write(ADD_ADPASS_STORE+i,adpass[i]);
    EEPROM.commit();
  }
}

int char2num (char str[])
{
  int x = 0;
  for(int i = 0;i<strlen(str);i++)
  {
    x = (str[i] - '0') + x*10;
  }
  return x;
}

void input_pass_value()
{
  if(customkey>='0' && customkey<='9')
  {
    data[datacount] = customkey;
    lcd.setCursor(datacount,2);
    lcd.write(data[datacount]);
    delay(300);
    lcd.setCursor(datacount,2);
    lcd.write('*');
    datacount = (datacount == MAXLPASS-1)?datacount:datacount+1;
    
  }
  else if(customkey == 'A')
  {
    datacount = (datacount == 0) ? datacount : datacount-1;
    lcd.setCursor(datacount,2);
    data[datacount] = '\0';
    lcd.write(' ');   
  }
}
uint16_t input_value()
{
  uint16_t number = 0;
  while(true)
  {
  //   Serial.println("in input");
    customkey = customKeypad.getKey();
    if(customkey >= '0' && customkey <= '9')
    {
      data[datacount] = customkey;
      lcd.setCursor(datacount,2);
      lcd.write(data[datacount]);
      datacount = (datacount == MAXLPASS-1)?datacount:datacount+1;
    }
    else if (customkey == 'B')
    {
      number = char2num(data);
      memset(data, 0,MAXLPASS);
      datacount = 0;
      break;
    }
    else if(customkey == 'A')
    {
      datacount = (datacount == 0) ? datacount : datacount-1;
      lcd.setCursor(datacount,2);
      data[datacount] = '\0';
      lcd.write(' ');
    }
    else if(customkey == 'D')
    {
      memset(data, 0,MAXLPASS);
      datacount = 0;
      break;
    }
  }
  return number;
}


void pass_rst_key()
{
    ret_fr_fcntp = 1;    
    changepassword(Password, "123456");
    lcd.clear();
    PrintL("   Successfully!   ", 1,0);
    PrintL("    The password   ",2,0);
    PrintL("  has been reset!  ",3,0);
    delay(1000);
}
void pass_ch_key()
{
    menu = &OldPass;
    MenuDisplay(menu);
    ret_fr_fcntp = 1;
    while (1)
    {
        customkey = customKeypad.getKey();
        input_pass_value();
        if(customkey == 'B' || datacount == MAXLPASS-1)
        {
            if(checkpass(Password,data))
            {
                menu = &NewPass;
                MenuDisplay(menu);
                memset(data, 0,MAXLPASS);
                datacount = 0;
                while (1)
                {
                    customkey = customKeypad.getKey();
                    input_pass_value();
                    if(datacount == LPASS - 1 )
                    {
                        changepassword(Password,data);
                        lcd.clear();
                        PrintL("Change successful!",1,0);
                        delay(1000);
                        break;
                    }
                }
            }
            else
            {
                lcd.clear();
                PrintL(" Change pass Failed ",1,0);
                PrintL(" Changes aren't made",2,0);
                delay(1000);
            }
            memset(data, 0,MAXLPASS);
            datacount = 0;
            //customkey = '\0';
            break;
        }
        else if(customkey == 'D')
        {
            memset(data, 0,MAXLPASS);
            datacount = 0;
            //customkey = '\0';
            break;
        }
    }
}
void adpass_ch_key()
{
  menu = &OldPass;
  MenuDisplay(menu);
  ret_fr_fcntp = 1;
  while(1)
  {
    customkey = customKeypad.getKey();
    input_pass_value();
    if(customkey == 'B' || datacount == MAXLPASS-1)
    {
      if(checkpass(keysofadmin,data))
      {
        menu = &NewPassAdmin;
        MenuDisplay(menu);
        memset(data,'\0',MAXLPASS);
        datacount = 0;
        while(1)
        {
          customkey = customKeypad.getKey();
          input_pass_value();
          if(datacount == 8)          
          {
            changeADpassword(keysofadmin,data);
            lcd.clear();
            PrintL("Change successful!",1,0);
            delay(1000);
            break;
          }
          
        }
      }  
      else
      {
        lcd.clear();
        PrintL(" Change pass Failed ",1,0);
        PrintL(" Changes aren't made",2,0);
      }
      memset(data, 0,MAXLPASS);
      datacount = 0;
      break;
    }
    else if(customkey == 'D')
    {
        memset(data, 0,MAXLPASS);
        datacount = 0;
        break;
    }
  }
}
void cleardata_return()
{
  memset(data,'\0',MAXLPASS);
  datacount=0;
  MenuDisplay(menu);
  lcd.setCursor(14,0);
  if(is_door_close == 0)
  {
    lcd.print("|OPEN ");
  }
  else
    lcd.print("|CLOSE");
}
void reset_factory()
{
  for(int i = 0;i<520;i++) // not clear bit store init DS1307 
  {
    EEPROM.write(i,0);
    EEPROM.commit();
  }
  lcd.clear();
  finger.emptyDatabase(); //clear all finger
  Serial.print("{\"Rst_WiFi\":\"1\"}");
  PrintL("    Factory Reset   ",1,0);
  PrintL("  ESP will restart  ",2,0);
  delay(2000);
  ESP.restart();
}
void handle_key()
{
  input_pass_value();
  if(customkey == 'B'||datacount == MAXLPASS - 1)
    if(checkpass(keysofadmin,data))
    {
      
      menu = &AdminMenu;
      MenuDisplay(menu);
      memset(data,'\0',MAXLPASS);
      datacount=0;
    }
    else
    {
      if(is_door_close == 1)
      {
        if(checkpass(Password,data))
        {
          accgranted("Pass");  
        }
        else
        {
          accdenied();
        }
        cleardata_return();
      }
    }
}
