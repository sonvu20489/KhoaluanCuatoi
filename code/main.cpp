#include <Arduino.h>
#include <RFID.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <string>
using namespace std;

#define EEPROM_SIZE     4096        // Kích thước EEPROM
#define MAX_CARDS       10          // Số thẻ tối đa được phép lưu trữ

#define RST_PIN         14          // GPIO14
#define SS_PIN          13          // GPIO13

class RFIDManager {
  private:
    static RFIDManager* instance;
    MFRC522 mfrc522;
    byte allowedCards[MAX_CARDS][4];
    int cardCount;
    
  public:
    static RFIDManager& getInstance() {
        if (!instance) {
            instance = new RFIDManager();
        }
        return *instance;
    }

    bool cardExists(String uid) {
        for (int i = 0; i < cardCount; i++) {
            if (uid.equals(getCardUID(i))) {
                return true;
            }
        }
        return false;
    }

    void addCard(String uid) {
        if (cardCount < MAX_CARDS && !cardExists(uid)) {
            for (int i = 0; i < 4; i++) {
                allowedCards[cardCount][i] = uid.substring(i * 2, i * 2 + 2).toInt();
            }
            cardCount++;
            saveToEEPROM();
        }
    }

    void removeCard(String uid) {
        for (int i = 0; i < cardCount; i++) {
            if (uid.equals(getCardUID(i))) {
                // Di chuyển các thẻ sau thẻ bị xóa
                for (int j = i + 1; j < cardCount; j++) {
                    for (int k = 0; k < 4; k++) {
                        allowedCards[j - 1][k] = allowedCards[j][k];
                    }
                }
                cardCount--;
                saveToEEPROM();
                break;
            }
        }
    }

    bool isCardAllowed(byte* uid) {
        for (int i = 0; i < cardCount; i++) {
            if (isEqual(allowedCards[i], uid)) {
                return true;
            }
        }
        return false;
    }

    String getCardUID(int index) {
        String uid = "";
        for (int i = 0; i < 4; i++) {
            uid.concat(String(allowedCards[index][i] < 0x10 ? "0" : ""));
            uid.concat(String(allowedCards[index][i], HEX));
        }
        return uid;
    }

  private:
    RFIDManager() : mfrc522(SS_PIN, RST_PIN), cardCount(0) {
        mfrc522.PCD_Init();
        loadFromEEPROM();
    }

    void loadFromEEPROM() {
        int address = 0;
        byte data[4];
        for (int i = 0; i < MAX_CARDS; i++) {
            for (int j = 0; j < 4; j++) {
                data[j] = EEPROM.read(address++);
            }
            if (isEqual(data, (byte*)"\xFF\xFF\xFF\xFF")) {
                break;
            }
            for (int j = 0; j < 4; j++) {
                allowedCards[i][j] = data[j];
            }
            cardCount++;
        }
    }

    void saveToEEPROM() {
        int address = 0;
        for (int i = 0; i < cardCount; i++) {
            for (int j = 0; j < 4; j++) {
                EEPROM.write(address++, allowedCards[i][j]);
            }
        }
        // Lưu địa chỉ kết thúc
        for (int i = 0; i < 4; i++) {
            EEPROM.write(address++, 0xFF);
        }
        EEPROM.commit();
    }

    bool isEqual(byte* a, byte* b) {
        for (int i = 0; i < 4; i++) {
            if (a[i] != b[i]) {
                return false;
            }
        }
        return true;
    }

    void printHex(byte* buffer, byte bufferSize) {
        String uid = "";
        for (byte i = 0; i < bufferSize; i++) {
            uid.concat(String(buffer[i] < 0x10 ? "0" : ""));
            uid.concat(String(buffer[i], HEX));
        }
        Serial.println(uid);
    }

  public:
    void checkForCard() {
        if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
            return;
        }

        byte* uid = mfrc522.uid.uidByte;
        if (isCardAllowed(uid)) {
            Serial.println("Access granted");
        } else {
            Serial.println("Access denied");
        }
        printHex(uid, mfrc522.uid.size);
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
    }
  public:
  String readCard() {
    String uid = "";
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {    
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        uid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
        uid.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      Serial.print("Card UID: ");
      Serial.println(uid);
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }
    return uid;
  }
};

RFIDManager* RFIDManager::instance = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  RFIDManager::getInstance().checkForCard();
  delay(1000);
}


