#include <SoftwareSerial.h>
SoftwareSerial BT(3,1);
int LED = LED_BUILTIN;

void setup() {
 pinMode(LED, OUTPUT);
 Serial.begin(9600); /* Define baud rate for serial communication */
 BT.begin(9600);
}

void loop() {
  if(BT.available())    // Si llega un dato por el puerto BT se envía al monitor serial
  {
    Serial.println("BT is on fire");
    char data = BT.read();
    switch (data)
    {
      case 'B':
        digitalWrite(LED, HIGH);
        break;
      case 'S':
        digitalWrite(LED, LOW);
        break;
      default:
        break;
    }
  }
 
  if(Serial.available())  // Si llega un dato por el monitor serial se envía al puerto BT
  {
     BT.write(Serial.read());
  }
}
