#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>
/*===================Global variable for keypad======================*/
extern char customkey;
extern bool ret_fr_fcntp;
/*===================Global variable for lCD======================*/

typedef struct linker Menu;
extern  Menu * menu;
/*====================Global variable for Warning======================*/
extern bool doorstate;
extern bool lastdoorstate;

#endif