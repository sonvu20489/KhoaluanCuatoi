#ifndef MYWARNING_H
#define MYWARNING_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <RTClib.h>

#define DOOR_SEN_PIN 34
#define LATCH_PIN   32
#define LRED_PIN    33
#define BUZZER_PIN  25
extern uint8_t prohibit;
extern bool is_door_close;
static unsigned long lastTime = 0;
static unsigned long timeBuzzer = 0;
static unsigned long Counter1 = 0;
static unsigned long timeWP = 0;
static uint8_t countdown = 0;
static bool btn_urgent = 0;

static char date[20];
/*==================== prototype function======================*/
void init_warning();
void accgranted(const char * name);
void accdenied();
void Close_door();
void Warning_door_open();
void Warning_wrongPass();
void emergency_btn();
void sendJson(const char * message, const char * time);
#endif