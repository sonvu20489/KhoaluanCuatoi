#include <iostream>
#define MAINMENU        1
#define ADMINMENU       2
#define OPTIONPASS     3
#define OPTIONRFID     4
#define OPTIONFINGER   5
#define OPTION0    0
#define OPTION1    1
#define OPTION2    2
#define OPTION3    3
typedef struct linker {
    unsigned char MenuID;
    const struct linker *pre;
    char list0[21];
    const struct linker *Menulist0;
    char List1[21];
    const struct linker *Menulist1;
    char List2[21];
    const struct linker *Menulist2;
    char List3[21];
    const struct linker *Menulist3;
}Menu;
const Menu *menu;
Menu MainMenu, AdminMenu, PassOption,RFIDOption,FingerOption;
MainMenu.MenuID = MAINMENU;


const Menu MainMenu = 
{
    MAINMENU,
    NULL,
    "                    ",NULL,
    "   Access control   ",NULL,
    "                    ",NULL,
    "Close: 0s  | B: acpt",NULL
};
const Menu AdminMenu = 
{
    ADMINMENU,
    &MainMenu,
    "     ADMIN MENU     ",NULL,
    " A | Password Option",&PassOption,
    " B | RFID Option    ",&RFIDOption,
    " C | Finger Option  ",&FingerOption
};
const Menu PassOption = 
{
    OPTIONPASS,
    &AdminMenu,
    "  PASS OPTION MENU  ",NULL,
    " A | Change Password",NULL,
    " B | Reset Password ",NULL,
    " C | ---UPDATING--- ",NULL,
};
const Menu OldPass = 
{
    NULL,
    &AdminMenu,
    " IDENTIFICATION PASS",NULL,
    " Input old password ",NULL,
    "                    ",NULL,
    "B | Acpt, D | Return",NULL,
};
const Menu NewPass = 
{
    NULL,
    &AdminMenu,
    " Type your password ",NULL,
    "   (character 0-9 ) ",NULL,
    "                    ",NULL,
    "B | Acpt, D | Return",NULL   
};
const Menu RFIDOption = 
{
    OPTIONRFID,
    &AdminMenu,
    "  RFID OPTION MENU  ",NULL,
    " A | Add RFID       ",NULL,
    " B | Delete RFID    ",NULL,
    " C | ---UPDATING--- ",NULL
};
const Menu FingerOption = 
{
    OPTIONFINGER,
    &AdminMenu,
    " FINGER OPTION MENU ",NULL,
    " A | Add Finger     ",NULL,
    " B | Delete Finger  ",NULL,
    " C | ---UPDATING--- ",NULL
};

void Functionpointer(uint8_t MODULE, uint8_t Action)
{
    switch (MODULE)
    {
    case OPTIONPASS:
        switch (Action)
        {
        case OPTION1:
            pass_ch_key();
            menu = (menu->pre==NULL)?menu:menu->pre;
            MenuDisplay(menu);
            break;
        case OPTION2:
            pass_rst_key();
            menu = &AdminMenu;
            MenuDisplay(menu);
            break;
        case OPTION3:
            break;
        default:
            break;
        }
    case OPTIONRFID:
        switch (Action)
        {
        case OPTION1:
            
            break;
        case OPTION2:
            break;
        case OPTION3:
            break;
        default:
            break;
        }
    case OPTIONFINGER:
        switch (Action)
        {
        case OPTION1:
            break;
        case OPTION2:
            break;
        case OPTION3:
            break;
        default:
            break;
        }
    default:
        break;
    }
}
void PrintL(const char *str,unsigned line, unsigned col)
{
    lcd.setCursor(col, line);
    lcd.print(str);
}
// void MenuDisplay(const Menu *menu)
// {
//     PrintL(menu->list0,0,0);
//     PrintL(menu->List1,1,0);
//     PrintL(menu->List2,2,0);
//     PrintL(menu->List3,3,0);
// }
void pass_rst_key()
{
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
    while (1)
    {
        customkey = customKeypad.getKey();
        input_pass_value();
        if(customkey == 'B' || datacount == maxlpass-1)
        {
            if(checkpass(Password,data))
            {
                menu = &NewPass;
                MenuDisplay(menu);
                memset(data, 0,7);
                datacount = 0;
                while (1)
                {
                    customkey = customKeypad.getKey();
                    input_pass_value();
                    if(datacount == lpass - 1 )
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
                
            }
            memset(data, 0,7);
            datacount = 0;
            menu = &AdminMenu;
            MenuDisplay(menu);
            break;
        }
        if(customkey == 'D')
        {
            memset(data, 0,7);
            datacount = 0;
            menu = (menu->pre!=NULL)?menu->pre:menu;
            MenuDisplay(menu);
        }
    }
}


void MenuDisplay(unsigned char menu)
{
    if(menu == MAINMENU)
    {
        PrintL("                    ",0,0);
        PrintL("   Access control   ",1,0);
        PrintL("                    ",2,0);
        PrintL("Close: 0s  | B: acpt",3,0);
    }
    if(menu == ADMINMENU)
    {
        PrintL("     ADMIN MENU     ",0,0);
        PrintL(" A | Password Option",1,0);
        PrintL(" B | RFID Option    ",2,0);
        PrintL(" C | Finger Option  ",3,0);
    }
    if(menu==ADMINMENU)
    {
        PrintL(" IDENTIFICATION PASS",0,0);
        PrintL(" Input old password ",1,0);
        PrintL("                    ",2,0);
        PrintL("B | Acpt, D | Return",3,0);
    }
    if(menu==)
}