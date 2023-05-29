#ifndef MYFINGER_H
#define MYFINGER_H

#include <Arduino.h>
#include <Adafruit_Fingerprint.h>
#include <EEPROM.h>
#include "global.h"
#define ADD_NUM_FINGER    3
#define ADD_MAP_FINGER    500
#define ADD_F_NAME_STORE  300
#define MAXFINGER         20
#define mySerial Serial2
static Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
static char Finger_initname[10]="Hello";
static char Finger_name[20][11];
/*==================== prototype function======================*/
void finger_init();
uint8_t getFingerprintID();
uint8_t getFingerprintEnroll();
int getFingerprintIDez();
int deleteFingerprint(uint16_t id);
void Finger_EnrollID();
void Finger_deleteID();
void handle_finger();
#endif