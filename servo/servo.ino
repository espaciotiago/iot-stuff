#include <Servo.h>
//--------------------------
//Constants
//--------------------------
const int CUPS = 3;
const int OPEN = 0;
const int CLOSE = 90;
const int GENERAL_DELAY = 1000;
const int OPEN_CLOSE_DELAY = 1000;
//--------------------------
//Variables
//--------------------------
Servo servo;
int counter = 0;
boolean canContinue = false;
//--------------------------
//Life cycle
//--------------------------
void setup() {
  Serial.begin(115200);
  servo.attach(2 );
  servo.write(CLOSE);
  canContinue = true;
  delay(GENERAL_DELAY);
}
//--------------------------
void loop() {
  dispatchCoffee();
  delay(GENERAL_DELAY);
}
//--------------------------
//Methods
//--------------------------
void dispatchCoffee(){
  if(canContinue){
    if(counter < CUPS){
      servo.write(OPEN);
      delay(OPEN_CLOSE_DELAY);
      servo.write(CLOSE);
      counter++;
    }else{
      counter = 0;
      canContinue = false;
    }
  }
}
