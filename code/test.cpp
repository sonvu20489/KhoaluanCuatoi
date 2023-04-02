#include <iostream>
#include <string>
#define MAX_USER 20
#define MAX_RFID 20
using namespace std;
typedef struct User_info {
    int nRIFD;
    int nfinger;
    string name;
    unsigned char RFID[MAX_RFID][4];
    unsigned char finger_ID[10];
} User_info;
User_info User[MAX_USER];
int main(){
    
    return 0;
}