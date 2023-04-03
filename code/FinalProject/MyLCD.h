#ifndef MYLCD_H
#define MYLCD_H
void RFID_deleteCard();
void RFID_addRecord();
void pass_ch_key();
void pass_rst_key();
void adpass_ch_key();
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#include "Myfinger.h"
#include "MyRFID.h"
#include "Mykeypad.h"

#define ACCMENU        1
#define ADMINMENU       2
#define OPTIONPASS     3
#define OPTIONRFID     4
#define OPTIONFINGER   5
#define NEWPASSADMENU    6
 
#define OPTION0    0
#define OPTION1    1
#define OPTION2    2
#define OPTION3    3
//prototype function
void init_LCD();
void Functionpointer(uint8_t MODULE, uint8_t Action);
void PrintL(const char *str,unsigned line, unsigned col);
/* ========================== FUNCTION =======================*/
typedef struct linker {
    unsigned char MenuID;
    char list0[21];
    struct linker *Menulist0; void (*Activation0)(uint8_t MODULE, uint8_t Action);
    char List1[21];
    struct linker *Menulist1; void (*Activation1)(uint8_t MODULE, uint8_t Action);
    char List2[21];
    struct linker *Menulist2; void (*Activation2)(uint8_t MODULE, uint8_t Action);
    char List3[21];
    struct linker *Menulist3; void (*Activation3)(uint8_t MODULE, uint8_t Action);
}Menu;
Menu * menu;
Menu AccMenu = 
{
  ACCMENU,
  "              |CLOSE",NULL,NULL,
  "   Access control   ",NULL,NULL,
  "                    ",NULL,NULL,
  "TiWrn: 0s  | B: acpt",NULL,NULL
};
Menu PassOption = 
{
  OPTIONPASS,
  "  PASS OPTION MENU  ",NULL,NULL,
  " A | Change Password",NULL,&Functionpointer,
  " B | Reset Password ",NULL,&Functionpointer,
  " C | Change AD Pass ",NULL,&Functionpointer
};
Menu RFIDOption = 
{
  OPTIONRFID,
  "  RFID OPTION MENU  ",NULL,NULL,
  " A | Add RFID       ",NULL,&Functionpointer,
  " B | Delete RFID    ",NULL,&Functionpointer,
  " C | ---UPDATING--- ",NULL,&Functionpointer
};
Menu FingerOption = 
{
  OPTIONFINGER,
  " FINGER OPTION MENU ",NULL,NULL,
  " A | Add Finger     ",NULL,&Functionpointer,
  " B | Delete Finger  ",NULL,&Functionpointer,
  " C | ---UPDATING--- ",NULL,&Functionpointer
}; 
Menu AdminMenu = 
{
  ADMINMENU,
  "     ADMIN MENU     ",&AccMenu,NULL,
  " A | Password Option",&PassOption,NULL,
  " B | RFID Option    ",&RFIDOption,NULL,
  " C | Finger Option  ",&FingerOption,NULL
};

Menu OldPass = 
{
  NULL,
  " IDENTIFICATION PASS",&AdminMenu,NULL,
  " Input old password ",NULL,NULL,
  "                    ",NULL,NULL,
  "B | Acpt, D | Return",NULL,NULL
};
Menu NewPass = 
{
  NULL,
  " Type your password ",&AdminMenu,NULL,
  "   (character 0-9 ) ",NULL,NULL,
  "                    ",NULL,NULL,
  "|User|   D | Return ",NULL,NULL  
};
Menu NewPassAdmin = 
{
  NEWPASSADMENU,
  " Type your password ",&AdminMenu,NULL,
  "   (character 0-9 ) ",NULL,NULL,
  "                    ",NULL,NULL,
  "|ADMIN|  D | Return ",NULL,NULL  
};
Menu AddRFID = 
{
  NULL,
  "     ADDING RFID    ",&AdminMenu,NULL,
  "   Scan your RFID   ",NULL,NULL,
  "                    ",NULL,NULL,
  "        D | Return  ",NULL,NULL  
};
Menu DeleteRFID = 
{
  NULL,
  "    DELETING RFID   ",&AdminMenu,NULL,
  "Scan RFID or type ID",NULL,NULL,
  "                    ",NULL,NULL,
  "B | Acpt,D | Return ",NULL,NULL  
};
Menu AddFinger = 
{
  NULL,
  "    ADDING FINGER   ",&AdminMenu,NULL,
  "  Scan your Finger  ",NULL,NULL,
  "                    ",NULL,NULL,
  "        D | Return  ",NULL,NULL  
};
Menu DeleteFinger = 
{
  NULL,
  "   DELETING FINGER  ",&AdminMenu,NULL,
  "Type your ID to del ",NULL,NULL,
  "                    ",NULL,NULL,
  "        D | Return  ",NULL,NULL
};
 
//prototype function 
void MenuDisplay(const Menu *menu);


/* ========================== FUNCTION =======================*/
void init_LCD()
{
  lcd.init();
  //lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("DATN CNKT MAY TINH ");
  lcd.setCursor(0,1);
  lcd.print("HT khoa cua an ninh");
  lcd.setCursor(0,2);
  lcd.print("    Khoa: 2019     ");
  lcd.setCursor(0,3);
  lcd.print("GVHD Huynh Hoang Ha");
  delay(1000);
  menu = &AccMenu;
  MenuDisplay(menu);
}
void Functionpointer(uint8_t MODULE, uint8_t Action)
{
    switch (MODULE)
    {
    case OPTIONPASS:
        switch (Action)
        {
        case OPTION1:
            pass_ch_key();
            break;
        case OPTION2:
            pass_rst_key();
            break;
        case OPTION3:
            adpass_ch_key();
            break;
        default:
            break;
        }
        break;
    case OPTIONRFID:
        switch (Action)
        {
        case OPTION1:
            RFID_addRecord();
            break;
        case OPTION2:
            RFID_deleteCard();
            break;
        case OPTION3:
            break;
        default:
            break;
        }
        break;
    case OPTIONFINGER:
        switch (Action)
        {
        case OPTION1:
            menu = &AddFinger;
            MenuDisplay(menu);
            Finger_EnrollID();
            break;
        case OPTION2:
            menu = &DeleteFinger;
            MenuDisplay(menu);
            Finger_deleteID();
            break;
        case OPTION3:
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    menu = &AdminMenu;
    MenuDisplay(menu);
}
void PrintL(const char *str,unsigned line, unsigned col)
{
    lcd.setCursor(col, line);
    lcd.print(str);
}
void MenuDisplay(const Menu *menu)
{
    PrintL(menu->list0,0,0);
    PrintL(menu->List1,1,0);
    PrintL(menu->List2,2,0);
    PrintL(menu->List3,3,0);
}

#endif
