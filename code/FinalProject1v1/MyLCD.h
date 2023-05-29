#ifndef MYLCD_H
#define MYLCD_H
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

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
extern LiquidCrystal_I2C lcd;
void init_LCD();
void Functionpointer(uint8_t MODULE, uint8_t Action);
void PrintL(const char *str,unsigned line, unsigned col);

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
} Menu;

void MenuDisplay(const Menu *menu);
static Menu AccMenu = 
{
  ACCMENU,
  "              |CLOSE",NULL,NULL,
  "   Access control   ",NULL,NULL,
  "                    ",NULL,NULL,
  "TiWrn: 0s  | B: acpt",NULL,NULL
};
static Menu PassOption = 
{
  OPTIONPASS,
  "  PASS OPTION MENU  ",NULL,NULL,
  " A | Change Password",NULL,&Functionpointer,
  " B | Reset Password ",NULL,&Functionpointer,
  " C | Change AD Pass ",NULL,&Functionpointer
};
static Menu RFIDOption = 
{
  OPTIONRFID,
  "  RFID OPTION MENU  ",NULL,NULL,
  " A | Add RFID       ",NULL,&Functionpointer,
  " B | Delete RFID    ",NULL,&Functionpointer,
  " C | Reset Factory  ",NULL,&Functionpointer
};
static Menu FingerOption = 
{
  OPTIONFINGER,
  " FINGER OPTION MENU ",NULL,NULL,
  " A | Add Finger     ",NULL,&Functionpointer,
  " B | Delete Finger  ",NULL,&Functionpointer,
  " C | ---UPDATING--- ",NULL,&Functionpointer
}; 
static Menu AdminMenu = 
{
  ADMINMENU,
  "     ADMIN MENU     ",&AccMenu,NULL,
  " A | Password Option",&PassOption,NULL,
  " B | RFID Option    ",&RFIDOption,NULL,
  " C | Finger Option  ",&FingerOption,NULL
};

static Menu OldPass = 
{
  NULL,
  " IDENTIFICATION PASS",&AdminMenu,NULL,
  " Input old password ",NULL,NULL,
  "                    ",NULL,NULL,
  "B | Acpt, D | Return",NULL,NULL
};
static Menu NewPass = 
{
  NULL,
  " Type your password ",&AdminMenu,NULL,
  "   (character 0-9 ) ",NULL,NULL,
  "                    ",NULL,NULL,
  "|User|   D | Return ",NULL,NULL  
};
static Menu NewPassAdmin = 
{
  NEWPASSADMENU,
  " Type your password ",&AdminMenu,NULL,
  "   (character 0-9 ) ",NULL,NULL,
  "                    ",NULL,NULL,
  "|ADMIN|  D | Return ",NULL,NULL  
};
static Menu AddRFID = 
{
  NULL,
  "     ADDING RFID    ",&AdminMenu,NULL,
  "   Scan your RFID   ",NULL,NULL,
  "                    ",NULL,NULL,
  "        D | Return  ",NULL,NULL  
};
static Menu DeleteRFID = 
{
  NULL,
  "    DELETING RFID   ",&AdminMenu,NULL,
  "Scan RFID or type ID",NULL,NULL,
  "                    ",NULL,NULL,
  "B | Acpt,D | Return ",NULL,NULL  
};
static Menu AddFinger = 
{
  NULL,
  "    ADDING FINGER   ",&AdminMenu,NULL,
  "  Scan your Finger  ",NULL,NULL,
  "                    ",NULL,NULL,
  "        D | Return  ",NULL,NULL  
};
static Menu DeleteFinger = 
{
  NULL,
  "   DELETING FINGER  ",&AdminMenu,NULL,
  "Type your ID to del ",NULL,NULL,
  "                    ",NULL,NULL,
  "        D | Return  ",NULL,NULL
};

#endif
