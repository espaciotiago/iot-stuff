#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Servo.h>
//--------------------------
//LAN Configuration
//--------------------------
#define WIFI_SSID "TIAGO"
#define WIFI_PASSWORD "shalom307"
//--------------------------
//Firebase DB Constants
//--------------------------
#define FIREBASE_HOST "wake-me-up-f7bec.firebaseio.com"
#define FIREBASE_AUTH "WWiUhQs8NaO5he4eGMH053jYt080w9IJfjIvxGhH"
//--------------------------
//Constants
//--------------------------
const int RELAY_INPUT = 2;
const int SERVO_INPUT = 5;
const int OPEN = 90;
const int CLOSE = 0;
const int ON = LOW;
const int OFF = HIGH;
const int GENERAL_DELAY = 1000;
const int OPEN_CLOSE_DELAY = 200;
//--------------------------
//Status list
//--------------------------
const int DO_NOTHING = 0;
const int PERFORM_DISPACTH_COFFEE = 1;
const int TURN_ON_THE_COFFEE = 2;
const int KEEP_IT_THAT_WAY = 3;
//--------------------------
//Variables
//--------------------------
Servo servo;
int wifiStatus;
int cups;
String dispatchCoffeeOn;
int counter = 0;
int currentStatus = DO_NOTHING;
boolean canContinue = false;
//--------------------------
//Life cycle
//--------------------------
void setup()
{
  Serial.begin(115200);
  pinMode(RELAY_INPUT, OUTPUT); 
  servo.attach(SERVO_INPUT);
  servo.write(CLOSE);
  canContinue = true;

  delay(GENERAL_DELAY);
  Serial.println('\n');
  digitalWrite(RELAY_INPUT, OFF);
  wifiConnect();

  Serial.println("Connecting to firebase...");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(10);
}
//--------------------------
void loop(){  
  //Get the status on the remote database
  Serial.println("Getting data...");

  String isLedOn = Firebase.getString("casa14/node1/value1");
  dispatchCoffeeOn = Firebase.getString("casa14/node1/dispatch_cups");
  cups = Firebase.getInt("casa14/node1/cups");
  
  if (Firebase.failed()) {
    Serial.print("setting /message failed:");
    Serial.println(Firebase.error());
    return;
  }
  
  Serial.println(isLedOn);
  Serial.println(cups);
  Serial.println(dispatchCoffeeOn);

  //Perform an action, acording the action
  if (isLedOn == "ON"){
    if(currentStatus == DO_NOTHING){
      currentStatus = PERFORM_DISPACTH_COFFEE;
      canContinue = true;
    }else if(currentStatus == PERFORM_DISPACTH_COFFEE){
      dispatchCoffee();
    }else if(currentStatus == TURN_ON_THE_COFFEE){
      digitalWrite(RELAY_INPUT, ON);
      currentStatus = KEEP_IT_THAT_WAY;
    }
  }else{
    currentStatus = DO_NOTHING;
    servo.write(CLOSE);
    digitalWrite(RELAY_INPUT, OFF);
  }

  //Verify wifi status
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi disconnected...");
    wifiConnect();
  }

  delay(GENERAL_DELAY);

}
//--------------------------
//Methods
//--------------------------
void wifiConnect()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID); Serial.println(" ...");

  int teller = 0;
  while (WiFi.status() != WL_CONNECTED){
    delay(GENERAL_DELAY);
    Serial.print(++teller); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
}
//--------------------------
void dispatchCoffee(){
  if(canContinue){
    if(counter < cups){
      if(dispatchCoffeeOn == "YES"){
        servo.write(OPEN);
        delay(OPEN_CLOSE_DELAY);
        servo.write(CLOSE);
        counter++;
      }else{
        Firebase.setString("casa14/node1/dispatch_cups","NO");
        currentStatus = TURN_ON_THE_COFFEE;
        counter = 0;
        canContinue = false;
      }
    }else{
      Firebase.setString("casa14/node1/dispatch_cups","NO");
      currentStatus = TURN_ON_THE_COFFEE;
      counter = 0;
      canContinue = false;
    }
  }
}
