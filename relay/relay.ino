
int relayInput = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(relayInput,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(relayInput,HIGH);
  delay(1000);
  digitalWrite(relayInput,LOW);
  delay(1000);
}
