#include <Wire.h>
#include "RGB.h"


// set to false if using a common cathode LED
#define commonAnode true

int boton=7;
RGB tcs = RGB(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(9600);


  //No partir sin el sensor
  while (!tcs.begin()) {
    Serial.println("Buscando sensor");
  } 
  pinMode(boton, INPUT);

}


void loop() {
  if (digitalRead(boton)==HIGH){
  tcs.enable();  
  //float H, S, V;
  tcs.setInterrupt(false);      // turn on LED

  delay(60);  // El tiempo de integración es de 50 ms
  
  //tcs.getRawData(&red, &green, &blue, &clear);
  //tcs.getHSV(&H, &S, &V);

  tcs.setInterrupt(true);  // turn off LED
  
  //String color="";
  //if (H>315 || H< 45)
  //  color="Rojo";
  //else if (H>180 && H<270)
  //  color="Azul";
  //else if (H>75 && H<150)
  //  color="Verde";
  //else
  //  color="Prueba de nuevo";

  int col=tcs.getColor();

  Serial.print("Color:\t"); 
  Serial.print(col); 
  Serial.print("\n");
  tcs.disable();  
  }

}
