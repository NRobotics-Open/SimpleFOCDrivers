#include "DualTMAG5170.h"

// #if defined(_STM32_DEF_)

DualTMAG5170::DualTMAG5170(SPIClass* _spi,int ENC_CS1, int ENC_CS2, int _polPairs)
{
  spi = _spi;
  enc_CS1 = ENC_CS1;
  enc_CS2 = ENC_CS2;
  polPairs = _polPairs;
  polesAngle = _2PI / polPairs;
}

/*
  Shaft angle calculation
*/
float DualTMAG5170::getSensorAngle() { 

  // get magnetic fields 
   b1X = sensor1.getB(AXIS_X);
   b1Y = sensor1.getB(AXIS_Y);
   b1Z = sensor1.getB(AXIS_Z);

   b2X = sensor2.getB(AXIS_X);
   b2Y = sensor2.getB(AXIS_Y);
   b2Z = sensor2.getB(AXIS_Z);

  // calculate amplitude scale
  float amp = sqrt((b1X * b1X ) + (b2X * b2X));
  
  // get normalised and filtered value between -1 and +1
  float sinVal = filter1(b1X/amp);
  float cosVal = filter2(b2X/amp);

  // calcualte subpolpairs angle 
  float alpha = atan(sinVal/cosVal);

  // correct for quadrand sinc atan is only -pi/2 to pi/2 defined
  if((sinVal > 0 ) && (cosVal > 0))
  {
    alpha = alpha;
  }
  else if ((sinVal > 0 ) && (cosVal < 0))
  {
    alpha = alpha + M_PI ;
  }
  else if((sinVal < 0 ) && (cosVal < 0))
  {
    alpha = alpha + M_PI ;
  }
  else if ((sinVal < 0 ) && (cosVal > 0))
  {
    alpha = alpha + 4*_PI_2 ;
  }


  // calculate the polpair turns
  if ((alpha < _PI_2) && ( lastAlpha >= (3*_PI_2))) {
      alphaTurns += 1;
  }
  else if ((alpha > (3*_PI_2)) && ( lastAlpha <= _PI_2)) {
      alphaTurns -= 1;
  }
  lastAlpha = alpha;


  // account for overflow 
  if ( alphaTurns< 0 ) 
  {
    alphaTurns  = polPairs-1;
  }
  else if (alphaTurns >= polPairs)
  {
    alphaTurns = 0;
  }


  // calculate final angle 
  alpha =  (alpha/_2PI) * polesAngle + ( (float)alphaTurns * polesAngle) ;

  return (alpha);
}


// getter for index pin
int DualTMAG5170::needsSearch() { return false; }

// private function used to determine if encoder has index
int DualTMAG5170::hasIndex() { return 0; }

// encoder initialisation of the hardware pins
void DualTMAG5170::init() {

// GPIO configuration
pinMode(enc_CS1,OUTPUT);
digitalWrite(enc_CS1, HIGH);

pinMode(enc_CS2,OUTPUT);
digitalWrite(enc_CS2, HIGH);

// init sensors with default values
sensor1.begin(spi,enc_CS1);
sensor1.setOperatingMode(OPERATING_MODE_ConfigurationMode);
sensor1.enableChannels(true,true,true);
sensor1.setOperatingMode(OPERATING_MODE_activeMeasureMode);
sensor1.setRange(X_RANGE_300mT, Y_RANGE_75mT, Z_RANGE_300mT);

sensor2.begin(spi,enc_CS2);
sensor2.setOperatingMode(OPERATING_MODE_ConfigurationMode);
sensor2.enableChannels(true,true,true);
sensor2.setOperatingMode(OPERATING_MODE_activeMeasureMode);
sensor2.setRange(X_RANGE_300mT, Y_RANGE_75mT, Z_RANGE_300mT);

initialized = true;
}

// #endif