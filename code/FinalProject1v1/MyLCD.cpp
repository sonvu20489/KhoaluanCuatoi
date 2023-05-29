#include "MyLCD.h"
#include "Mykeypad.h"
#include "MyRFID.h"
#include "Myfinger.h"
#include "global.h"

Menu * menu;
void init_LCD()
{
  lcd.init();
  lcd.backlight();
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
            reset_factory();
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