
int servo=5;
int sensor=6;
int iman=7;
void setup() {
  // put your setup code here, to run once:

pinMode(servo, OUTPUT);
pinMode(sensor, INPUT);
pinMode(iman, OUTPUT);
TCCR0B = (TCCR0B & 0b11111000) | 0x05;
}

void loop() {
  // put your main code here, to run repeatedly:
if (digitalRead(sensor)==0){
  analogWrite(servo,10);
  digitalWrite(iman,LOW);
  }

else{
  analogWrite(servo,37);
  digitalWrite(iman,HIGH);
  delay(1000);
  }

}
