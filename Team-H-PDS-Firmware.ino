#include <Arduino.h>
#include <Firebase_ESP_Client.h>

#include <ESP8266WiFi.h>

#include <Servo.h>

Servo myservo;

//#if defined(ESP32)
//#include <WiFi.h>
//#elif defined(ESP8266)
//#include <ESP8266WiFi.h>
//#endif


//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;
int intValue;
unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;


//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define WIFI_SSID "Tenda_618090"
#define WIFI_PASSWORD "rideabove256"

#define API_KEY "AIzaSyA-J0odHUJxJEkUjTc5xBaJBJsE-xP3J2w"
#define DATABASE_URL "https://touri-65f07-default-rtdb.firebaseio.com/"

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  myservo.attach(5);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}



int getMotorCommand() {
  if (Firebase.RTDB.getInt(&fbdo, "/ee_pos")) {
    if (fbdo.dataType() == "int") {
      int value = fbdo.intData();
      return value;
    }
  }

  return -1;
}


//TODO: add servo logic
void eePosListerner() {

}

void loop() {
  if (Firebase.ready() && signupOK) {

    int a = getMotorCommand();
    //    eePosListerner(a);
    myservo.write(a);
    Serial.println(a*2);

    delay(500);

  }
}
