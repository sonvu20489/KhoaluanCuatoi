#ifndef MYKEYPAD_H
#define MYKEYPAD_H
#include <Arduino.h>
#include <Keypad.h>
#include "EEPROM.h"

#define EEPROMSIZE 525

#define MAXLPASS  20
#define LPASS     7

#define COLS      4
#define ROWS      4

#define ADD_PASS_INIT       0
#define ADD_ADPASS_INIT     1
#define ADD_PASS_STORE      4
#define ADD_ADPASS_STORE    10
static char key [ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
static byte rowPins[ROWS]= {15,2,0,4};
static byte colPins[COLS]= {27,14,12,13};
static byte datacount = 0;
static char data[MAXLPASS+1];
static char keysofadmin[9] = "12345678";
static char Password [LPASS]= "123456";
extern uint8_t prohibit;
extern bool is_door_close;
static Keypad customKeypad(makeKeymap(key),rowPins,colPins, ROWS, COLS);
/*==================== prototype function======================*/
void init_keypadcheck();
boolean checkpass(char pass[],char input[]);
void changepassword(char * pass, char input[]);
int char2num (char str[]);
void input_pass_value();
uint16_t input_value();
void pass_rst_key();
void pass_ch_key();
void input_checkpass_admin();
void adpass_ch_key();
void cleardata_return();
void handle_key();
void reset_factory();
#endif