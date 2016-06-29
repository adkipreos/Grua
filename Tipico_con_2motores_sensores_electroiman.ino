
#include <PasoaPaso.h>

#include <Wire.h>
#include "RGB.h"
RGB tcs = RGB(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

#define Pasos 512     // change this depending on the number of steps
// per revolution of your motor
#define motorPin1 12
#define motorPin2 13
#define motorPin3 3
#define motorPin4 2
#define electroiman 4
#define trigPin 7
#define echoPin 8
#define fc1 11
#define fc3 10
#define servo 9
#define Laser 5
//int inPin = 10; 
//int Posicion;
int UltimoCubo = 0;
int as = 0;
// initialize of the Stepper library:

PasoaPaso myStepper(Pasos, motorPin1, motorPin2);
PasoaPaso myStepper2(Pasos, motorPin3, motorPin4);

void setup() {
  while (!tcs.begin()) {
    Serial.println("Buscando sensor");
  } 
  pinMode (electroiman, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // set the motor speed at 60 RPMS:
  pinMode(servo, OUTPUT);
  //TCCR1B = (TCCR1B & 0b11111000) | 0x05;
  TCCR1A=0b10100010;
  TCCR1B=0b00011010; 
  ICR1 = 40000;
  myStepper.setVel(30);
  myStepper2.setVel(30);

  // Initialize the Serial port:
  Serial.begin(9600);
  pinMode(fc1, INPUT);
  pinMode(Laser, INPUT);
  while (digitalRead(fc1)==HIGH){
    myStepper.step(int(myStepper.GetPosicion())-1);
  }
  myStepper.setPosicion(0);
  while (digitalRead(fc1)==LOW){
    myStepper.step(int(myStepper.GetPosicion())+1);
  }
  myStepper.step(200);
  delay(500);
  while (digitalRead(fc3)==HIGH){
    myStepper2.step(int(myStepper2.GetPosicion())-1);
  }
  myStepper2.setPosicion(0);
//  while (digitalRead(fc3)==LOW){
//    myStepper2.step(int(myStepper.GetPosicion())+1);
//  }
  delay(500);  
  //analogWrite(servo,2560);
  
}

void loop() {
  while (myStepper.GetPosicion()<810){
    myStepper.step(int(myStepper.GetPosicion())+1);
    //Serial.println(int(myStepper.GetPosicion()));
    //Serial.println(digitalRead(Laser));
//    long duration, distance;
//    digitalWrite(trigPin, LOW);  // Added this line
//    delayMicroseconds(2); // Added this line
//    digitalWrite(trigPin, HIGH);
//  //  delayMicroseconds(1000); - Removed this line
//    delayMicroseconds(10); // Added this line
//    digitalWrite(trigPin, LOW);
////    digitalWrite(electroiman, HIGH);    
//    duration = pulseIn(echoPin, HIGH);
//    distance = (duration/2) / 29.1;    
    if (digitalRead(Laser)==LOW){
      myStepper.step(int(myStepper.GetPosicion())+30);
    //Serial.println(distance);
//    Serial.println("cm al cubo");
//    if ((distance<=17) && (distance>5)) {  
      delay(500);
      UltimoCubo=int(myStepper.GetPosicion()-30);
      CuboEncontrado(UltimoCubo);
    }
  }
}

// Función para recolectar y depositar cubo
void CuboEncontrado(int ucubo){  
    long duration, distance;
    digitalWrite(trigPin, LOW);  // Added this line
    delayMicroseconds(2); // Added this line
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); // Added this line
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;
    Serial.print(distance);
    Serial.println(" cm al cubo");
    delay(500);
    if (distance<=20 && (distance>=3)) {
      if (distance<=14){
        myStepper.step(int(myStepper.GetPosicion())-45);
        long duration2, distance2;
        digitalWrite(trigPin, LOW);  // Added this line
        delayMicroseconds(2); // Added this line
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10); // Added this line
        digitalWrite(trigPin, LOW);
        duration2 = pulseIn(echoPin, HIGH);
        distance2 = (duration2/2) / 29.1;  
        Serial.print(distance2);
        Serial.println(" cm al cubo");
        Serial.print(distance);
        Serial.println(" cm al cubo"); 
        Serial.print(distance2-distance);     
        //myStepper2.step(73*(distance2 + 2));
        if ((distance2>20) || (distance2==distance)){
          Serial.print("CASO A");
          delay(500);
          myStepper.step(int(myStepper.GetPosicion())+45);
          if (distance>9.5){
            myStepper2.step(73*(distance + 1));
          }
          else{
            myStepper2.step(73*(distance + 2.2));
          }
        }
        else if (abs(distance2-distance)>4){
          Serial.print("CASO B: diferencia de");
          Serial.print(abs(distance2-distance));
//          while (digitalRead(Laser)==LOW){
//            myStepper.step(int(myStepper.GetPosicion())-1);
//          }
          myStepper.step(int(myStepper.GetPosicion())+45);          
          if (distance2<=9.5){
            myStepper2.step(73*(distance2 + 2.2));
          }
          else{
            myStepper2.step(min(1355,(73*(distance2 + 1))));
          }  
        }
        else {
          Serial.print("CASO C");
          delay(500);
          myStepper.step(int(myStepper.GetPosicion())+45);
          if (distance>9.5){
            myStepper2.step(73*(distance + 1));
          }
          else{
            myStepper2.step(73*(distance + 2.2));
          }        
        }        
      }
      else {
        myStepper2.step(min(1355,(73*(distance + 1))));
      }
      
      myStepper.step(int(myStepper.GetPosicion()+60));
      delay(500);
      analogWrite(servo,9472); //BAJA
      digitalWrite(electroiman, HIGH);  
      delay (1500);
      analogWrite(servo,2560); //SUBE
      delay(500);
      myStepper.step(10);
      delay(250);
      myStepper2.step(900);
      delay(1000);
      tcs.enable();  
      tcs.setInterrupt(false); 
      delay(60);  // El tiempo de integración es de 50 ms
      int col=tcs.getColor();
      tcs.setInterrupt(true);
      Serial.println("Buscando Color");
      while (col ==0){      //Serial.println("Buscando Color");
        as = as+1;
        if (as==500){
          myStepper2.step(910);
          myStepper2.step(900);          
        }
        
        col=tcs.getColor();
        tcs.setInterrupt(true);
        Serial.println(col);      
      }
      if (col == 1){
        Serial.println("ROJO");
        myStepper2.step(820);
        analogWrite(servo,9472);
        digitalWrite(electroiman, LOW);  
        analogWrite(servo,2560);
      }
      else if (col == 2){
        Serial.println("VERDE");
        myStepper2.step(1300);
        analogWrite(servo,9472);
        digitalWrite(electroiman, LOW);  
        analogWrite(servo,2560);      
      }
      else if (col == 3){
        Serial.println("AZUL");
        myStepper2.step(80);
        analogWrite(servo,9472);
        digitalWrite(electroiman, LOW);  
        analogWrite(servo,2560);      
      }        
      tcs.disable();
      myStepper2.step(0);
      myStepper.step(ucubo);
      delay(900);
    }  
}

