
#ifdef __AVR
  #include <avr/pgmspace.h>
#elif defined(ESP8266)
  #include <pgmspace.h>
#endif
#include <stdlib.h>
#include <math.h>

#include "RGB.h"


float powf(const float x, const float y)
{
  return (float)(pow((double)x, (double)y));
}


void RGB::write8 (uint8_t reg, uint32_t value)
{
  Wire.beginTransmission(TCS34725_ADDRESS);
  #if ARDUINO >= 100
  Wire.write(TCS34725_COMMAND_BIT | reg);
  Wire.write(value & 0xFF);
  #else
  Wire.send(TCS34725_COMMAND_BIT | reg);
  Wire.send(value & 0xFF);
  #endif
  Wire.endTransmission();
}


uint8_t RGB::read8(uint8_t reg)
{
  Wire.beginTransmission(TCS34725_ADDRESS);
  #if ARDUINO >= 100
  Wire.write(TCS34725_COMMAND_BIT | reg);
  #else
  Wire.send(TCS34725_COMMAND_BIT | reg);
  #endif
  Wire.endTransmission();

  Wire.requestFrom(TCS34725_ADDRESS, 1);
  #if ARDUINO >= 100
  return Wire.read();
  #else
  return Wire.receive();
  #endif
}


uint16_t RGB::read16(uint8_t reg)
{
  uint16_t x; uint16_t t;

  Wire.beginTransmission(TCS34725_ADDRESS);
  #if ARDUINO >= 100
  Wire.write(TCS34725_COMMAND_BIT | reg);
  #else
  Wire.send(TCS34725_COMMAND_BIT | reg);
  #endif
  Wire.endTransmission();

  Wire.requestFrom(TCS34725_ADDRESS, 2);
  #if ARDUINO >= 100
  t = Wire.read();
  x = Wire.read();
  #else
  t = Wire.receive();
  x = Wire.receive();
  #endif
  x <<= 8;
  x |= t;
  return x;
}


void RGB::enable(void)
{
  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON);
  delay(3);
  write8(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);  
}

void RGB::disable(void)
{
  /* Turn the device off to save power */
  uint8_t reg = 0;
  reg = read8(TCS34725_ENABLE);
  write8(TCS34725_ENABLE, reg & ~(TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN));
}


RGB::RGB(tcs34725IntegrationTime_t it, tcs34725Gain_t gain) 
{
  _tcs34725Initialised = false;
  _tcs34725IntegrationTime = it;
  _tcs34725Gain = gain;
}



void RGB::setIntegrationTime(tcs34725IntegrationTime_t it)
{
  if (!_tcs34725Initialised) begin();

  /* Update the timing register */
  write8(TCS34725_ATIME, it);

  /* Update value placeholders */
  _tcs34725IntegrationTime = it;
}


void RGB::setGain(tcs34725Gain_t gain)
{
  if (!_tcs34725Initialised) begin();

  /* Update the timing register */
  write8(TCS34725_CONTROL, gain);

  /* Update value placeholders */
  _tcs34725Gain = gain;
}

//Este método comprueba que el sensor está conectado.

boolean RGB::begin(void) 
{
  Wire.begin();
  
  uint8_t x = read8(TCS34725_ID);
  if ((x != 0x44) && (x != 0x10))
  {
    return false;
  }
  _tcs34725Initialised = true;

  setIntegrationTime(_tcs34725IntegrationTime);
  setGain(_tcs34725Gain);
  enable();

  return true;
}
  


void RGB::getHSV (float *H, float *S, float *V)
{
  if (!_tcs34725Initialised) begin();

  int R = read16(TCS34725_RDATAL);
  int G = read16(TCS34725_GDATAL);
  int B = read16(TCS34725_BDATAL);
  
  /* Set a delay for the integration time */
  switch (_tcs34725IntegrationTime)
  {
    case TCS34725_INTEGRATIONTIME_2_4MS:
      delay(3);
      break;
    case TCS34725_INTEGRATIONTIME_24MS:
      delay(24);
      break;
    case TCS34725_INTEGRATIONTIME_50MS:
      delay(50);
      break;
    case TCS34725_INTEGRATIONTIME_101MS:
      delay(101);
      break;
    case TCS34725_INTEGRATIONTIME_154MS:
      delay(154);
      break;
    case TCS34725_INTEGRATIONTIME_700MS:
      delay(700);
      break;
  }
  float Rm = R/65535.0;
  float Gm = G/65535.0;
  float Bm = B/65535.0;
  float Cmax=max(Rm,Gm);
  Cmax=max(Cmax,Bm);
  float Cmin=min(Rm,Gm);
  Cmin=min(Cmin,Bm);
  *V=Cmax;
  if (Cmax-Cmin==0){
    *H=0;
    }
  else if (Cmax==Rm){
    *H=60*(Gm-Bm)/(Cmax-Cmin);
  }
  else if (Cmax==Gm){
    *H=60*((Bm-Rm)/(Cmax-Cmin)+2);
  }
  else{
    *H=60*((Rm-Gm)/(Cmax-Cmin)+4);
  }

  if (Cmax==0)
    *S=0;
  else
    *S=(Cmax-Cmin)/Cmax;

  if (*H<0)
    *H+=360;

}


int RGB::getColor ()
{
  if (!_tcs34725Initialised) begin();

  int R = read16(TCS34725_RDATAL);
  int G = read16(TCS34725_GDATAL);
  int B = read16(TCS34725_BDATAL);
  
  /* Set a delay for the integration time */
  switch (_tcs34725IntegrationTime)
  {
    case TCS34725_INTEGRATIONTIME_2_4MS:
      delay(3);
      break;
    case TCS34725_INTEGRATIONTIME_24MS:
      delay(24);
      break;
    case TCS34725_INTEGRATIONTIME_50MS:
      delay(50);
      break;
    case TCS34725_INTEGRATIONTIME_101MS:
      delay(101);
      break;
    case TCS34725_INTEGRATIONTIME_154MS:
      delay(154);
      break;
    case TCS34725_INTEGRATIONTIME_700MS:
      delay(700);
      break;
  }
  float H,S,V;
  float Rm = R/65535.0;
  float Gm = G/65535.0;
  float Bm = B/65535.0;
  float Cmax=max(Rm,Gm);
  Cmax=max(Cmax,Bm);
  float Cmin=min(Rm,Gm);
  Cmin=min(Cmin,Bm);
  V=Cmax;
  if (Cmax-Cmin==0){
    H=0;
    }
  else if (Cmax==Rm){
    H=60*(Gm-Bm)/(Cmax-Cmin);
  }
  else if (Cmax==Gm){
    H=60*((Bm-Rm)/(Cmax-Cmin)+2);
  }
  else{
    H=60*((Rm-Gm)/(Cmax-Cmin)+4);
  }

  if (Cmax==0)
    S=0;
  else
    S=(Cmax-Cmin)/Cmax;

  if (H<0)
    H+=360;

  int color=0;
  if (H>315 || H< 45)
    color=1;
  else if (H>180 && H<270)
    color=3;
  else if (H>75 && H<150)
    color=2;
  else
    color=0;
  return color;

}




void RGB::setInterrupt(boolean i) {
  uint8_t r = read8(TCS34725_ENABLE);
  if (i) {
    r |= TCS34725_ENABLE_AIEN;
  } else {
    r &= ~TCS34725_ENABLE_AIEN;
  }
  write8(TCS34725_ENABLE, r);
}

void RGB::clearInterrupt(void) {
  Wire.beginTransmission(TCS34725_ADDRESS);
  #if ARDUINO >= 100
  Wire.write(TCS34725_COMMAND_BIT | 0x66);
  #else
  Wire.send(TCS34725_COMMAND_BIT | 0x66);
  #endif
  Wire.endTransmission();
}


void RGB::setIntLimits(uint16_t low, uint16_t high) {
   write8(0x04, low & 0xFF);
   write8(0x05, low >> 8);
   write8(0x06, high & 0xFF);
   write8(0x07, high >> 8);
}
