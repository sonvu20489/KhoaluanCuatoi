#ifndef MYRFID_H
#define MYRFID_H
#include <Arduino.h>
#include <EEPROM.h>
#include <MFRC522.h>


#include<SPI.h>
#include<Wire.h>
#define SS_PIN    5
#define RST_PIN   26
#define MAXRFID   20
#define ADD_RFID_NSTORE  2
// #define Add_DEF_MASTER  1
#define ADD_UID_CARD   20
#define ADD_NAME_STORE 100
static MFRC522 mfrc522(SS_PIN, RST_PIN);
///////////////Define Variable for handle//////////
static bool match = false;
static uint8_t successRead;
static byte storedCard[4];
static byte readCard[4];
static byte masterCard[4];
static char RFID_initname[10]="Hello";
static char RFID_name[20][11];
/*==================== prototype function======================*/
void init_RFID();
uint8_t getID();
void ShowReaderDetails();
void readID(uint8_t number);
void writeID(byte a[]);
void deleteID(byte a[]);
void deleteRFID_ID(uint8_t slot);
bool checkTwo(byte a[],byte b[]);
uint8_t findIDSLOT(byte find[]);
bool findID(byte find[]);
void RFID_changename(char name[], uint8_t RFIDCard[]);
void RFID_addRecord();
void RFID_deleteCard();
void handle_RFID();
#endif