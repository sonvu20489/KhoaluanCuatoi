#include "Myfinger.h"
#include "Mykeypad.h"
#include "global.h"
#include "MyLCD.h"
#include "MyWarning.h"
void finger_init()
{
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);
  if(EEPROM.read(ADD_NUM_FINGER)==0)
  {
    Serial.println("{\"DelFinger\":\"1\"}");
    return ;
  }
  DynamicJsonDocument doc(1024);
  String str_store;
  JsonArray id = doc.createNestedArray("id");
  JsonArray Name = doc.createNestedArray("Name");
  for(int i = 0; i<MAXFINGER;i++)
  {
    for(int j = 0; j<10;j++)
    {
      Finger_name[i][j]=(char)(EEPROM.read((10*i)+ADD_F_NAME_STORE+j));
    }
  }
  for(int i = 0;i<MAXFINGER;i++)
  {
    if(EEPROM.read(ADD_MAP_FINGER + i) == 0)
    {
      continue;
    }
    if(strcmp(Finger_name[i],"")==0)
    {
      continue;
    }
    
    id.add(i+1);
    Name.add(Finger_name[i]);
  }
  serializeJson(doc,str_store);
  Serial.println(str_store);
}
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      // Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      // Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      // Serial.println("Imaging error");
      return p;
    default:
      // Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      // Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      // Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      // Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      // Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      // Serial.println("Could not find fingerprint features");
      return p;
    default:
      // Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    // Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    // Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    // Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  

  return finger.fingerID;
}
///////////////////////////////////////////////////////////
uint8_t getFingerprintEnroll() {
  int id=-1;
  uint8_t num = EEPROM.read(ADD_NUM_FINGER);
  for(int i = 0;i<MAXFINGER;i++)
  {
    if(EEPROM.read(ADD_MAP_FINGER+i)==0)
    {
      id = i+1;
      break;
    }
  }
  if(id == -1)
  {
    PrintL("  NO SLOT ACCEPT  ",1,0);
    PrintL("CAN'T ADD YOUR ID!",2,0);
    delay(1000);
    return true;    
  }
  int p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      // Serial.println("Image taken");
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("     Image taken!   ");
      lcd.setCursor(0, 2);
      lcd.print("                    ");
      delay(100);
      //first_read = true;
      break;
    case FINGERPRINT_NOFINGER:
      // Serial.println(".");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("  Add new as ID#    ");
      lcd.setCursor(16, 1);
      lcd.print(id);
      lcd.setCursor(0, 2);
      lcd.print("     Place finger   ");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("    Comunication  ");
      lcd.setCursor(0, 2);
      lcd.print("       ERROR!     ");
      delay(1000);
      break;
    case FINGERPRINT_IMAGEFAIL:
      // Serial.println("Imaging error");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("       -Image     ");
      lcd.setCursor(0, 2);
      lcd.print("       ERROR!     ");
      delay(1000);
      break;
    default:
      // Serial.println("Unknown error");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("      -Unknown      ");
      lcd.setCursor(0, 2);
      lcd.print("        ERROR!      ");
      delay(1000);
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("  Image converted!  ");
      lcd.setCursor(0, 2);
      lcd.print("                    ");
      break;
    case FINGERPRINT_IMAGEMESS:
      // Serial.println("Image too messy");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("  Image too messy!"  );
      lcd.setCursor(0, 2);
      lcd.print("                    ");
      delay(1000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      // Serial.println("Communication error");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("    Comunication    ");
      lcd.setCursor(0, 2);
      lcd.print("       ERROR!       ");
      delay(1000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      // Serial.println("Could not find fingerprint features");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("   No fingerprint   ");
      lcd.setCursor(0, 2);
      lcd.print("  features found    ");
      delay(1000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      // Serial.println("Could not find fingerprint features");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("   No fingerprint   ");
      lcd.setCursor(0, 2);
      lcd.print("  features found    ");
      delay(1000);
      return p;
    default:
      // Serial.println("Unknown error");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("      -Unknown      ");
      lcd.setCursor(0, 2);
      lcd.print("       ERROR!       ");
      delay(1000);
      return p;
  }

  // Serial.println("Remove finger");
  // lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("    Remove finger!  ");
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  delay(2000);
  p = 0;
  
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  
  // Serial.print("ID "); Serial.println(id);
  p = -1;
  
  // Serial.println("Place same finger again");
  // lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("  ID#               ");
  lcd.setCursor(4, 1);
  lcd.print(id);
  p = -1;
  lcd.setCursor(0, 1);
  lcd.print("   Place again the  ");
  lcd.setCursor(0, 2);
  lcd.print("   same finger      ");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      // Serial.println("Image taken");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("    Image taken!    ");
      lcd.setCursor(0, 2);
      lcd.print("                    ");
      break;
    case FINGERPRINT_NOFINGER:
      // Serial.print(".");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("   Place again the  ");
      lcd.setCursor(0, 2);
      lcd.print("    same finger     ");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      // Serial.println("Communication error");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("    Communication   ");
      lcd.setCursor(0, 2);
      lcd.print("       ERROR!       ");
      delay(1000);
      break;
    case FINGERPRINT_IMAGEFAIL:
      // Serial.println("Imaging error");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("       -Image       ");
      lcd.setCursor(0, 2);
      lcd.print("       ERROR!       ");
      delay(1000);
      break;
    default:
      // Serial.println("Unknown error");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("      -Unknown      ");
      lcd.setCursor(0, 2);
      lcd.print("       ERROR!       ");
      delay(1000);
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      // Serial.println("Image converted");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("  Image converted!  ");
      lcd.setCursor(0, 2);
      lcd.print("                    ");
      break;
    case FINGERPRINT_IMAGEMESS:
      // Serial.println("Image too messy");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("  Image too messy!  ");
      lcd.setCursor(0, 2);
      lcd.print("                    ");
      delay(1000);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      // Serial.println("Communication error");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("    Comunication    ");
      lcd.setCursor(0, 2);
      lcd.print("       ERROR!       ");
      delay(1000);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      // Serial.println("Could not find fingerprint features");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("   No fingerprint   ");
      lcd.setCursor(0, 2);
      lcd.print("  features found    ");
      delay(1000);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      // Serial.println("Could not find fingerprint features");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("    Comunication    ");
      lcd.setCursor(0, 2);
      lcd.print("       ERROR!       ");
      delay(1000);
      return p;
    default:
      // Serial.println("Unknown error");
      // lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("      -Unknown      ");
      lcd.setCursor(0, 2);
      lcd.print("       ERROR!       ");
      delay(1000);
      return p;
  }

  // OK converted!
  //Serial.print("Creating model for #");  Serial.println(id);
  // lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("   Print matched!   ");
  lcd.print("    Creating model  ");
  lcd.setCursor(0, 2);
  lcd.print("  for ID#           ");
  lcd.setCursor(8, 2);
  lcd.print(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    // lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("   Print matched!   ");
    lcd.setCursor(0, 2);
    lcd.print("                    ");
    delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    // Serial.println("Communication error");
    // lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("     Comunication   ");
    lcd.setCursor(0, 2);
    lcd.print("        ERROR!      ");
    delay(1000);
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    // Serial.println("Fingerprints did not match");
    // lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("  Fingerprint did   ");
    lcd.setCursor(0, 2);
    lcd.print("  not match         ");
    delay(1000);
    return p;
  } else {
    // Serial.println("Unknown error");
    // lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("      -Unknown    ");
    lcd.setCursor(0, 2);
    lcd.print("       ERROR!     ");
    delay(1000);
    return p;
  }

  // Serial.print("ID "); Serial.println(id);
  // lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print("ID# ");
  lcd.setCursor(4, 2);
  lcd.print(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    // Serial.println("Stored!");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("       Stored      ");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("    New ID saved   ");
    lcd.setCursor(0, 2);
    lcd.print("      As ID#       ");
    lcd.setCursor(13, 2);
    lcd.print(id);
    delay(3000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    // Serial.println("Communication error");
    // lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("    Communication   ");
    lcd.setCursor(0, 2);
    lcd.print("       ERROR!       ");
    delay(1000);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    // Serial.println("Could not store in that location");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("  Could not store   ");
    lcd.setCursor(0, 2);
    lcd.print("  in that location  ");
    delay(1000);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    // Serial.println("Error writing to flash");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("  Error writing to  ");
    lcd.setCursor(0, 2);
    lcd.print("    flash           ");
    delay(1000);
    return p;
  } else {
    // Serial.println("Unknown error");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("      -Unknown      ");
    lcd.setCursor(0, 2);
    lcd.print("       ERROR!       ");
    delay(1000);
    return p;
  }
  EEPROM.write((ADD_MAP_FINGER + (id-1)),100);
  EEPROM.commit();
  sprintf(Finger_initname,"Finger%02d",id);
  for(int i = 0; i<strlen(Finger_initname);i++)
  {
    EEPROM.write((id-1)*10+ADD_F_NAME_STORE+i,Finger_initname[i]);
    EEPROM.commit();
    Finger_name[id-1][i]=Finger_initname[i];
  }

  String JsonToSend;
  DynamicJsonDocument doc(1024);
  doc["type"] = "Finger";
  doc["action"] = "add";
  doc["id"] = String(id);
  doc["name"] = Finger_name[id-1];
  serializeJson(doc,JsonToSend);
  Serial.println(JsonToSend);

  num++;
  EEPROM.write(ADD_NUM_FINGER,num);
  EEPROM.commit();
  return true;
}
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -2;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -2;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  // Serial.print("Found ID #"); Serial.print(finger.fingerID);
  // Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
int deleteFingerprint(uint16_t id) {
  int p = -1;
  uint8_t num = EEPROM.read(ADD_NUM_FINGER);
  if(EEPROM.read(ADD_MAP_FINGER+(id-1))!= 100)
  {
    lcd.setCursor(0, 1);
    lcd.print("Something went wrong");
    lcd.setCursor(0, 2);
    lcd.print("Cannt delete your ID");
    delay(1000);
    return p;    
  }
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) {
    //Serial.println("Deleted!");
    num--;
    EEPROM.write(ADD_NUM_FINGER,num);
    EEPROM.commit();
    EEPROM.write(ADD_MAP_FINGER+(id-1),0);
    EEPROM.commit();
    String JsonToSend;
    DynamicJsonDocument doc(1024);
    doc["type"] = "Finger";
    doc["action"] = "del";
    doc["id"] = String(id);
    doc["name"] = Finger_name[id-1];
    serializeJson(doc,JsonToSend);
    Serial.println(JsonToSend);
    //clear name of RFID
    for(int i = 0;i<10;i++)
    {
      EEPROM.write((id-1)*10+ADD_F_NAME_STORE+i,0);
      EEPROM.commit();
      Finger_name[id-1][i] = 0;
    }
    lcd.setCursor(0, 1);
    lcd.print("   Successfully!    ");
    lcd.setCursor(0,2);
    lcd.print("Delete Finger ID#");
    lcd.print(id);    
    delay(1000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    lcd.setCursor(0, 1);
    lcd.print("Something went wrong");    
    lcd.setCursor(0, 2);
    lcd.print("Communication error ");
    delay(1000);
  } else if (p == FINGERPRINT_BADLOCATION) {
    //Serial.println("Could not delete in that location");
    lcd.setCursor(0, 1);
    lcd.print("  Could not delete  ");
    lcd.setCursor(0, 2);
    lcd.print("  in that location  ");
    delay(1000);
  } else if (p == FINGERPRINT_FLASHERR) {
    // Serial.println("Error writing to flash");
    lcd.setCursor(0, 1);
    lcd.print("    Error writing   ");
    lcd.setCursor(0, 2);
    lcd.print("      to flash!     ");
    delay(1000);
  } else {
    // Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    lcd.setCursor(0, 1);
    lcd.print("   Unknown error!   ");
    lcd.setCursor(0, 2);
    lcd.print("ERROR CODE:");
    lcd.print(p);
    delay(1000);
  }

  return p;
}
void Finger_EnrollID()
{
  ret_fr_fcntp = 1;
  lcd.setCursor(18, 0);
  lcd.print(EEPROM.read(ADD_NUM_FINGER));
  while(!getFingerprintEnroll());
}
void Finger_deleteID()
{
  ret_fr_fcntp = 1;
  lcd.setCursor(18, 0);
  lcd.print(EEPROM.read(ADD_NUM_FINGER));
  uint16_t a = input_value();
  if(a != 0)
    deleteFingerprint(a);
}
void handle_finger()
{
  int checkfinger = getFingerprintIDez();
  if(checkfinger == -1)
  {
    accdenied();
    cleardata_return();
  }
  else if (checkfinger == -2)
  {
    /*don't do sth*/
  }
  else
  {
    accgranted((const char*)Finger_name[checkfinger-1]);
    lcd.clear();
    PrintL("    Welcom user!   ",1,0);
    PrintL((const char*)Finger_name[checkfinger-1],2,4);
    delay(1000);
    cleardata_return();
  }
}