#ifndef MYRFID_H
#define MYRFID_H
#include "Mykeypad.h"

#include<EEPROM.h>
#include<MFRC522.h>
#include<SPI.h>
#include<Wire.h>
#define SS_PIN    5
#define RST_PIN   26
#define MAXRFID   20
#define ADD_RFID_NSTORE  2
// #define Add_DEF_MASTER  1
#define ADD_UID_CARD   20
#define ADD_NAME_STORE 100
MFRC522 mfrc522(SS_PIN, RST_PIN);
///////////////Define Variable for handle//////////
bool match = false;
uint8_t successRead;
byte storedCard[4];
byte readCard[4];
byte masterCard[4];
char RFID_initname[10]="Hello";
char RFID_name[20][11];
/* prototype function*/
void init_RFID();
uint8_t getID();
void ShowReaderDetails();
void writeID(byte a[]);
void deleteID(byte a[]);
bool checkTwo(byte a[],byte[]);
uint8_t findIDSLOT(byte find[]);
bool findID(byte find[]);
bool isMaster(byte test[]);
void RFID_addRecord();
void RFID_deleteCard();
void handle_RFID();
/* ========================== FUNCTION =======================*/
void init_RFID()
{
  SPI.begin();
  mfrc522.PCD_Init();
  ShowReaderDetails();
  for(int i = 0; i<MAXRFID;i++)
  {
    for(int j = 0; j<10;j++)
    {
      RFID_name[i][j]=(char)(EEPROM.read(ADD_NAME_STORE+(10*i)+j));
    }
  }
}
uint8_t getID(){
  if(!mfrc522.PICC_IsNewCardPresent()){ //if new Tag place to RFID reader continue
    return 0;
  }
  if(! mfrc522.PICC_ReadCardSerial()){ //Since a Tag placed get Serial and continue
    return 0;
  }
  for ( uint8_t i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
  }
  mfrc522.PICC_HaltA(); //Stop reading
  
  return 1;
}
/*--------------Check if RFID is Correctly initialized or not------*/
void ShowReaderDetails(){
  //Get the MFRC522 Software version
  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  // When 0x00 or 0xFF is returned, communication probably failed
  if ((v == 0x00) || (v == 0xFF)) {
     lcd.setCursor(0, 0);
     lcd.print("Communication Failure");
     lcd.setCursor(0, 1);
     lcd.print("Check Connections");
    // digitalWrite(BuzzerPin, HIGH);
    delay(2000);
    while (true); // do not go further
  }
}
/*--------------Read an ID from EPPROM-------------*/
void readID(uint8_t number){
  uint8_t start = (number * 4)+ ADD_UID_CARD; //figure out start positon
  for(uint8_t i = 0;i<4;i++)
  {
    storedCard[i] = EEPROM.read(start+i); // Assign values read from EEPROM to array
  }
}
/*-----------------------Add ID to EEPROM---------------------*/
void writeID(byte a[]){
  if(EEPROM.read(ADD_RFID_NSTORE)==MAXRFID)
  {
    PrintL("  FULL SLOT RFID! ",1,0);
    PrintL("CAN'T ADD YOUR ID!",2,0);
    delay(1000);
    return ;
  }
  if(!findID(a)){// Before we write to the EEPROM, check to see if we have seen this card before!
    // Get the numer of used spaces, position ADD_RFID_NSTORE stores number of ID Cards
    uint8_t num = EEPROM.read(ADD_RFID_NSTORE); 
    num++;
    EEPROM.write(ADD_RFID_NSTORE,num);    
    EEPROM.commit();
    int slot_add = -1;
    uint8_t count0;
    for(int slot = 0; slot < MAXRFID;slot++)
    {
      count0 = 0;
      for(int i = 0;i<4;i++)
      {
        if(EEPROM.read((slot*4)+ADD_UID_CARD+i) == 0)
        {
          count0++;
        }
      }
      if(count0 == 4)
      {
        slot_add = slot;
        break;
      }
    }
    if(slot_add == -1)
    {
      PrintL("  NO SLOT ACCEPT  ",1,0);
      PrintL("CAN'T ADD YOUR ID!",2,0);
      delay(1000);
      return ;
    }
    for(int i = 0;i<4;i++)
    {
      EEPROM.write((slot_add*4)+ADD_UID_CARD+i,a[i]);
      EEPROM.commit();
    }
    sprintf(RFID_initname,"RFID%02d",slot_add+1);
    for(int i = 0; i<strlen(RFID_initname);i++)
    {
      EEPROM.write((slot_add*10)+ADD_NAME_STORE+i,RFID_initname[i]);
      EEPROM.commit();
      RFID_name[slot_add][i]=RFID_initname[i];
    }
    lcd.setCursor(0, 2);
    lcd.print("Added");
    delay(1000);
  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print("  Add RFID Failed! ");
    lcd.setCursor(0, 2);
    lcd.print("Wrong ID or bad ROM");
    delay(2000);
  }
}
/*----------------Delete ID from EEPROM-----------*/
void deleteID(byte a[]){
  if(!findID(a)){ // Before we delete from the EEPROM, check to see if we have this card!
    lcd.setCursor(0, 1);
    lcd.print("Delete RFID Failed!");
    lcd.setCursor(0, 2);
    lcd.print("Wrong ID or bad ROM");
    delay(2000);
  }
  else{
    // Get the numer of used spaces, position ADD_RFID_NSTORE stores number of ID Cards
    uint8_t num = EEPROM.read(ADD_RFID_NSTORE);
    uint8_t slot; // Figure out the slot number of the card
    uint8_t start; // = ( num * 4 ) + 6; // Figure out where the next slot starts
    slot = findIDSLOT(a); // Figure out the slot number of the card to delete
    start = (slot * 4) + ADD_UID_CARD;
    uint8_t startn = (slot * 10) + ADD_NAME_STORE;
    num--;
    EEPROM.write(ADD_RFID_NSTORE,num); // Write the new count to the counter
    EEPROM.commit(); //enable for ESP32
    for(int i = 0;i<4;i++)
    {
      EEPROM.write(start+i,0);
      EEPROM.commit();
    }
    for(int i = 0;i<10;i++)
    {
      EEPROM.write(startn + i, 0);
      EEPROM.commit();
      RFID_name[slot][i] = 0;      
    }
     lcd.setCursor(0, 2);
     lcd.print("Removed");
    delay(1000);
  }
}
void deleteRFID_ID(uint8_t slot)
{
  if(slot >= 20)
  {
    lcd.setCursor(0, 2);
    lcd.print("Not exist slot");    
    return ;
  }
  uint8_t Read[4];
  bool count0 = 0;
  for(int i = 0;i<4;i++)
  {
    Read[i] = EEPROM.read((slot*4)+ADD_UID_CARD+i);
  }
  for(int i = 0;i<4;i++)
  {
    if(Read[i]==0)
    {
      count0++;
    }
  }
  if(count0 == 4)
  {
    lcd.setCursor(0, 2);
    lcd.print("Not exist slot");
    delay(1000);
    return ;
  }
  deleteID(Read);
}
/*------------------------Check Bytes---------*/
bool checkTwo(byte a[],byte b[]){
  if(a[0]!=0) // Make sure there is something in the array first
    match = true; //Assume they match at first
  for(uint8_t k = 0;k<4;k++){
    if(a[k]!=b[k]) //if a!=b then set match = false
      match = false;
  }
  if (match ){ // check to see if match is still true
    return true;
  }
  else {
    return false;
  }
}
uint8_t findIDSLOT(byte find[]){
  //uint8_t count = EEPROM.read(ADD_RFID_NSTORE);
  for(uint8_t i = 0;i<MAXRFID;i++)
  {
    readID(i);
    if(checkTwo(find,storedCard)){// Check to see if the storedCard read from EEPROM
      return i;
      break;
    }
  }
}
bool findID(byte find[]){
  for(uint8_t i = 0;i<MAXRFID;i++){
    readID(i);
    if(checkTwo(find,storedCard)){
      return true;
      break;
    }
  }
  return false;
}
bool isMaster(byte test[]){
  if(checkTwo(test,masterCard))
  {
    return true;
  }
  else
  {
    return false;
  }
}
void RFID_changename(char name[], uint8_t RFIDCard[])
{
  if(findID(RFIDCard))
  {
    uint8_t slot = findIDSLOT(RFIDCard); 
    for(int i = 0;i<10;i++)
    {
      EEPROM.write((slot*10)+ADD_NAME_STORE+i,name[i]);
      EEPROM.commit();
      RFID_name[slot][i] = name[i];
    }
    
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Change name Failed ");
    lcd.setCursor(0, 2);
    lcd.print("Wrong ID or bad ROM");
    memset(data, 0,maxlpass);
    datacount = 0;
    delay(1000);
    MenuDisplay(menu);
  }
}
void RFID_addRecord()
{
  ret_fr_fcntp = 1;
  menu = &AddRFID;
  MenuDisplay(menu);
  lcd.setCursor(18, 0);
  lcd.print(EEPROM.read(ADD_RFID_NSTORE));
  while(1)
  {
    customkey = customKeypad.getKey();
    successRead = getID();
    if(successRead)
    {
      writeID(readCard);
      menu = &AddRFID;
      MenuDisplay(menu);    
    }
    if(customkey=='D')
    {
      break;
    } 
  }  
}
void RFID_deleteCard()
{
  ret_fr_fcntp = 1;
  menu = &DeleteRFID;
  MenuDisplay(menu);
  lcd.setCursor(18, 0);
  lcd.print(EEPROM.read(ADD_RFID_NSTORE));
  do
  {
    customkey = customKeypad.getKey();
    
    if(customkey >= '0' && customkey <= '9')
    {
      data[datacount] = customkey;
      lcd.setCursor(datacount,2);
      lcd.write(data[datacount]);
      datacount = (datacount == maxlpass-1)?datacount:datacount+1;
    }
    else if (customkey == 'B')
    {
      
      deleteRFID_ID(char2num(data));
      memset(data, 0,maxlpass);
      datacount = 0;
      break;
    }
    
    else if(customkey == 'A')
    {
      datacount = (datacount == 0) ? datacount : datacount-1;
      lcd.setCursor(datacount,2);
      data[datacount] = '\0';
      lcd.write(' ');
    }
    else if(customkey=='D')
    {
      memset(data, 0,maxlpass);
      datacount = 0;
      break;
    }
    successRead = getID();  
  }
  while(!successRead);
  if(successRead)
  {
    deleteID(readCard);
  }
}
void handle_RFID()
{
  successRead = getID();
  if(successRead)
  {
    if(findID(readCard))
    {
      int slot = findIDSLOT(readCard);
      accgranted();
      lcd.clear();
      PrintL("    Welcom user!   ",1,0);
      PrintL((const char*)RFID_name[slot],2,4);
      delay(1000);
      cleardata_return();
    }
    else
    {
      accdenied();
      cleardata_return();
    }    
  }
}
#endif