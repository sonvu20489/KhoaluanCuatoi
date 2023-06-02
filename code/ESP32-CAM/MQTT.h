#include "GET_POSTRQ.h"
#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "sonvu20489"
#define AIO_KEY         "aio_vjMM49njcDsSqRf2mvoUOvIDXIYi"

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Define Adafruit IO MQTT feeds
String Namef_DelFinger = "/feeds/" + apikey + ".delfinger";
String Namef_DelRFID = "/feeds/" + apikey + ".delrfid";
String Namef_chNameFinger = "/feeds/" + apikey + ".chnamefinger";
String Namef_chNameRFID = "/feeds/" + apikey + ".chnamerfid";
const char * hello = "/feeds/test.test1";
//Adafruit_MQTT_Publish feed1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/test.test1");
Adafruit_MQTT_Subscribe feed2 = Adafruit_MQTT_Subscribe(&mqtt,  (String(AIO_USERNAME)+ Namef_DelFinger).c_str());
Adafruit_MQTT_Subscribe feed3 = Adafruit_MQTT_Subscribe(&mqtt,  (String(AIO_USERNAME)+ Namef_DelRFID).c_str());
Adafruit_MQTT_Subscribe feed4 = Adafruit_MQTT_Subscribe(&mqtt,  (String(AIO_USERNAME)+ Namef_chNameFinger).c_str());
Adafruit_MQTT_Subscribe feed5 = Adafruit_MQTT_Subscribe(&mqtt,  (String(AIO_USERNAME)+ Namef_chNameRFID).c_str());

bool connected = false; // Track MQTT connection status

void reconnect() {
  while (!mqtt.connected()) {
    Serial.println("Connecting to Adafruit IO MQTT...");
    if (mqtt.connect()==0) {
      connected = true;
      Serial.println("Connected to Adafruit IO MQTT");
      mqtt.subscribe(&feed2);
    } else {
      connected = false;
      Serial.println("Failed to connect to Adafruit IO MQTT");
      delay(2000);
    }
  }
}

void callback(char *data, uint16_t len) {
  // Handle received messages for subscribed feed2

  // Convert payload to string
  String message;
  for (int i = 0; i < len; i++) {
    message += (char)data[i];
  }
  Serial.println(message);
}
