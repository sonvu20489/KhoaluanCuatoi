#include <EEPROM.h>
#include <RTClib.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
RTC_DS1307  rtc;
LiquidCrystal_I2C lcd(0x27,20,4);
//for keypad
#define maxlpass 20
#define lpass    7 
#define COLS     4
#define ROWS     4

//for menu 
#define mainmenu  0
#define selmenu   1
#define delmenu   2
#define addmenu   3
//for handle
#define handlepass   4
#define handlefinger 5
#define handlerfid   6
//for add
#define changepass   7
#define addfinger    8
#define addrfid      9
//for del
#define rstpass     10
#define delfinger   11
#define delrfid     12
char key [ROWS][COLS] = {
  {'7','8','9','/'},
  {'4','5','6','*'},
  {'1','2','3','-'},
  {'P','0','=','+'}
};
byte rowPins[ROWS]= {9,8,7,6};
byte colPins[COLS]= {5,4,3,2};
int datacount = 0;
char data[maxlpass];
char Password [lpass]= "123456";// +1 for char '\0'
char customkey;
char Keyrstpass[9] = "=P=PPPPP"; 
int menu = mainmenu;
int EPROMadd_password = 60;
int cnt;
int prohibit = 0;
int is_door_open = 0;
long prevMillis = 0;
Keypad customKeypad(makeKeymap(key),rowPins,colPins, ROWS, COLS);
//////////////PROTOTYPE FUNCTION/////////////////////
void inputkeypadvalue();
void handleforkeypad();
void gotomenu(byte source, char spec);
void displaymenu(int menu);
///////////INITITIALIZE FUNCTION////////////////////
void setup() {
  rtc.begin();
  if(EEPROM.read(0)!= 144)
  {
    for(int i = 0;i<6;i++)
    {
      EEPROM.write(EPROMadd_password+i,Password[i]);
    }
    EEPROM.write(0,144);
  }
  for(int i = 0 ; i<6;i++)
  {
    Password[i] = EEPROM.read(EPROMadd_password+i);
  }
  rtc.adjust(DateTime(2022, 12, 25, 1, 12, 0));
  DateTime now = rtc.now();
  lcd.init();
  // put your setup code here, to run once:
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(" Test keypad ");
  delay(1000);
  displaymenu(menu);
}
void loop() {
  // put your main code here, to run repeatedly:
  customkey = customKeypad.getKey();
  if(customkey == '+'&&menu!=mainmenu)
  {
    menu = mainmenu;
    displaymenu(menu);
  }
  else if(customkey=='/'||customkey == '*' || customkey == '-')
  {
    gotomenu(menu,customkey);
  }
  
}
void inputkeypadvalue()
{
  customkey = customKeypad.getKey();
  if(customkey>='0' && customkey<='9')
  {
    data[datacount] = customkey;
    lcd.setCursor(datacount,2);
    lcd.write(data[datacount]);
    datacount++;
  }
  else if(customkey == '/')
  {
    datacount = (datacount == 0) ? datacount : datacount-1;
    lcd.setCursor(datacount,2);
    data[datacount] = '\0';
    lcd.write(' ');   
  }
}
/////////////////////////Function Handle for keypad type password///////
void handleforkeypad()
{
//  cnt = 0;
  while(1)
  {
    inputkeypadvalue();
    if(customkey == '+')
    {
      break;
    }
    if( customkey == '*' || datacount == maxlpass-1)
    {
      if (!strcmp(Password,data))
      {
        is_door_open = 1;
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("  Successfully!    ");
        lcd.setCursor(0,2);
        lcd.print("  ACCESS GRANTED   ");
        delay(1000);
        lcd.setCursor(0,2);
        lcd.print("   Door is opened  ");
        delay(1000);
        break;
      }
      else
      {
        prohibit++;
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("      Failed!      ");
        lcd.setCursor(0,2);
        lcd.print("  ACCESS DENIED!   ");
        lcd.setCursor(0,3);
        delay(2000);
        if(prohibit == 5)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" Wrong over 5 times ");
          lcd.setCursor(0, 1);
          lcd.print("  Please try again  ");
          lcd.setCursor(0, 2);
          lcd.print("  After 1 mininute  ");
          delay(60000);
          prohibit = 0;
          break;
        }
        memset(data,'\0',7);
        datacount=0;
        displaymenu(menu);
      }
    }
  }
  memset(data,'\0',7);
  datacount=0;
  prohibit = 0;
  menu = mainmenu;
  displaymenu(menu);
}
void changepassFunc()
{
  while(1)
  {
    inputkeypadvalue();
    if(customkey == '+')
    {
      break;
    }
    if( customkey == '*' || datacount == maxlpass-1)
    {
      if (!strcmp(Password,data))
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Type Your new pass ");
        lcd.setCursor(0,1);
        lcd.print("   (character 0-9 )");
        memset(data,'\0',7);
        datacount = 0;
        while(1)
        {  
          inputkeypadvalue();
          if( customkey == '*' || datacount == lpass-1)
          {
            strcpy(Password,data);
            for(int i = 0;i<6;i++)
            {
              EEPROM.write(EPROMadd_password+i,Password[i]);
            }
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("Change successful! ");
            break;
          }
        }
        delay(1000);
        break;
      }
      else
      {
        prohibit++;
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("     Wrong pass    ");
        lcd.setCursor(0,2);
        lcd.print("     Try again!    ");
        lcd.setCursor(0,3);
        delay(2000);
        if(prohibit == 3)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" Wrong over 3 times ");
          lcd.setCursor(0, 1);
          lcd.print("  Please try again  ");
          lcd.setCursor(0, 2);
          lcd.print("  After 1 minute    ");
          delay(60000);
          prohibit = 0;
          break;
        }
        memset(data,'\0',7);
        datacount=0;
        displaymenu(menu);
      }
    }
  }
  memset(data,'\0',7);
  datacount=0;
  prohibit = 0;
  menu = mainmenu;
  displaymenu(menu);
}
void RstPassFunction()
{
//  lcd.clear();
  while(cnt != 60)
  {
   if(millis() - prevMillis >= 1000)
   {
    cnt++;
    lcd.setCursor(0,0);
    lcd.print("Time remain:");
    lcd.print(60-cnt);
    lcd.print("s  ");
    prevMillis = millis();
   }
   if(cnt == 60)
   {
      lcd.setCursor(0,1);
      lcd.print("      Timeout!      ");
      lcd.setCursor(0,2);
      lcd.print("    The password   ");
      lcd.setCursor(0,3);
      lcd.print(" hasn't been reset!");
      break;
   }
   customkey = customKeypad.getKey();
   if(customkey)
   data[datacount++] = customkey;
   if(datacount == 8)
   {
     if(!strcmp(data, Keyrstpass))
     {
        strcpy(Password,"123456");
        for(int i = 0;i<6;i++)
        {
          EEPROM.write(EPROMadd_password+i,Password[i]);
        }
        lcd.setCursor(0,1);
        lcd.print("   Successfully!   ");
        lcd.setCursor(0,2);
        lcd.print("    The password   ");
        lcd.setCursor(0,3);
        lcd.print("    has been reset!");
        break;
     }
     else
     {
        lcd.setCursor(0,1);
        lcd.print("      Failed!      ");
        lcd.setCursor(0,2);
        lcd.print("    The password   ");
        lcd.setCursor(0,3);
        lcd.print(" hasn't been reset!");
        break;
     }
   }
  }
  delay(1000);
  cnt = 0;
  memset(data,'\0',7);
  datacount=0;
  menu = mainmenu;
  displaymenu(menu);
}
//////////////////////////Goto Menu Function //////////////////
void gotomenu(byte source, char spec)
{
  switch(source)
  {
    case mainmenu:
      switch (spec)
      {
        case '/':
        menu = selmenu;
        displaymenu(menu);
        break;
        case '*':
        menu = delmenu;
        displaymenu(menu);
        break;
        case '-':
        menu = addmenu;
        displaymenu(menu);
        break;
      }
    break;
    case addmenu:
    switch (spec)
      {
        case '/':
        menu =NULL;
        displaymenu(menu);
        break;
        case '*':
        menu = changepass;
        displaymenu(menu);
        changepassFunc();
        break;
        case '-':
        menu =NULL;
        displaymenu(menu);
        break;
      }
    break;
    case selmenu:
    switch (spec)
      {
        case '/':
        menu =NULL;
        displaymenu(menu);
        break;
        case '*':
        menu =handlepass;
        displaymenu(menu);
        handleforkeypad();
        break;
        case '-':
        menu =NULL;
        displaymenu(menu);
        break;
      }
    break;
    case delmenu:
    switch (spec)
      {
        case '/':
        menu =NULL;
        displaymenu(menu);
        break;
        case '*':
        menu =rstpass;
        displaymenu(menu);
        RstPassFunction();
        break;
        case '-':
        menu =NULL;
        displaymenu(menu);
        break;
      }
    break;
  }
}
void canhbaodotnhap()
{
  
}
// //////////////////////////function for display menu//////////////////// 
void displaymenu(int menu)
{
  if(menu == mainmenu)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Door Status: ");
    if(is_door_open)
    {
      lcd.print("OPENED");
    }
    else
      lcd.print("CLOSED");
    lcd.setCursor(0,1);
    lcd.print(" / | Options menu  ");
    lcd.setCursor(0,2);
    lcd.print(" * | Delete menu   ");
    lcd.setCursor(0,3);
    lcd.print(" - | Add menu      ");
  }
  if(menu == delmenu)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    DELETE MENU    ");
    lcd.setCursor(0,1);
    lcd.print(" / | Delete finger ");
    lcd.setCursor(0,2);
    lcd.print(" * | Reset password");
    lcd.setCursor(0,3);
    lcd.print(" - | Delete RFID   ");
  }
  if(menu == addmenu)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  ADD MENU  |+: pre");
    lcd.setCursor(0,1);
    lcd.print(" / | add finger    ");
    lcd.setCursor(0,2);
    lcd.print(" * | change pass   ");
    lcd.setCursor(0,3);
    lcd.print(" - | add rfid      ");
  }
  if(menu == selmenu)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("OPTIONS MENU|+: pre");
    lcd.setCursor(0,1);
    lcd.print(" / | Scan finger   ");
    lcd.setCursor(0,2);
    lcd.print(" * | Type Password ");
    lcd.setCursor(0,3);
    lcd.print(" - | Scan RFID     ");
  }
  if(menu == handlepass)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("INPUT YOUR PASSWORD");
    lcd.setCursor(0,1);
    lcd.print("   (character 0-9 )");
    lcd.setCursor(0,3);
    lcd.print(" * : accept        ");
  }
  if(menu == handlefinger)
  {
    
  }
  if(menu == handlerfid)
  {
    
  }
  if(menu == changepass)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("IDENTIFICATION PASS");
    lcd.setCursor(0,1);
    lcd.print("INPUT OLD PASSWORD ");
    lcd.setCursor(0,2);
    lcd.print("                   ");
    lcd.setCursor(0,3);
    lcd.print(" * : accept        ");
  }
  if(menu == rstpass)
  {
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(" PLEASE PRESS THE  ");
    lcd.setCursor(0,2);
    lcd.print("KEYS FOR RESET PASS");
  }
}
