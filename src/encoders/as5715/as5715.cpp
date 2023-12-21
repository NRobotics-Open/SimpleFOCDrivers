#include "as5715.h"

// #if defined(_STM32_DEF_)

/*
  HardwareEncoder(int cpr)
*/
AS5715::AS5715(unsigned int epp, int pinSinP, int pinSinM, int pinCosP,int pinCosM) {

    epoles = epp * 2.0f ;
    nAlphaTurns = (int32_t)(epoles * 2  - 1);
    
    polesAngle = _2PI / (float)epp;

    // this does strange things
    // _pinSinP = digitalPinToPinName(pinSinP);
    // _pinSinM = digitalPinToPinName(pinSinM);
    // _pinCosP = digitalPinToPinName(pinCosP);
    // _pinCosM = digitalPinToPinName(pinCosM);

    _pinSinP = pinSinP;
    _pinSinM = pinSinM;
    _pinCosP = pinCosP;
    _pinCosM = pinCosM;    

}

/*
  Shaft angle calculation
*/
float AS5715::getSensorAngle() { 


// do filtering ???
//   sinm_raw += analogRead(_pinSinM);
//   sinp_raw += analogRead(_pinSinP);

//   cosm_raw += analogRead(_pinCosM);
//   cosp_raw += analogRead(_pinCosP);

//   sinm_raw /= 2.0f;
//   sinp_raw /= 2.0f;

//   cosm_raw /= 2.0f;
//   cosp_raw /= 2.0f;

  sinp_raw = analogRead(_pinSinP);
  sinm_raw = analogRead(_pinSinM);
  
  cosp_raw = analogRead(_pinCosP);
  cosm_raw = analogRead(_pinCosM);

  // Serial.print("sinm_raw: ");
  // Serial.print(sinm_raw);
  // Serial.print("\tsinp_raw: ");
  // Serial.print(sinp_raw);
  
  // Serial.print("\tcosm_raw: ");
  // Serial.print(cosm_raw);
  // Serial.print("\tcosp_raw: ");
  // Serial.print(cosp_raw);


  // get the sin and cos values from -1 to 1
  float sind = (sinp_raw - sinm_raw) * (3.3f / (1.0f * 4096.0f));
  float cosd = (cosp_raw - cosm_raw) * (3.3f / (1.0f * 4096.0f));

  // Serial.print("\tsind: ");
  // Serial.print(sind);
  // Serial.print("\tcosd: ");
  // Serial.print(cosd);

  float scale = 1.0f;

  // Serial.print("\tscale: ");
  // Serial.print(scale);

  // scale and constrain 
  sind = constrain(sind / scale, -1.0f, 1.0f);
  cosd = constrain(cosd / scale, -1.0f, 1.0f);


  // get our polpair angle
  float alpha = atan(sind/cosd);
  
  // Serial.print("\talpha: ");
  // Serial.print(alpha);
  

  // calculate the polpair turns
  if ((alpha < -_PI_6) && ( lastAlpha >= _PI_6)) {
      alphaTurns += 1;
  }
  else if ((alpha > _PI_6) && ( lastAlpha <= -_PI_6)) {
      alphaTurns -= 1;
  }
  lastAlpha = alpha;

  // Serial.print("\talphaTurns: ");
  // Serial.print(alphaTurns);
  

  if ( alphaTurns< 0 ) 
  {
    alphaTurns  = nAlphaTurns;
  }
  else if (alphaTurns > nAlphaTurns)
  {
    alphaTurns = 0;
  }
  
  // offset of arctan shift to a range for alpha of 0 to PI 
  alpha += _PI_2;  
  
  // 
  alpha = (alpha + (float)alphaTurns * polesAngle) / epoles;

  // Serial.print("\talpha+Turns: ");
  // Serial.print(alpha);

  return (alpha);
}


// getter for index pin
int AS5715::needsSearch() { return false; }

// private function used to determine if encoder has index
int AS5715::hasIndex() { return 0; }

// encoder initialisation of the hardware pins
void AS5715::init() {
    // GPIO configuration



#if defined(_STM32_DEF_)
    pinMode(_pinSinP, INPUT_ANALOG);
    pinMode(_pinSinM, INPUT_ANALOG);
    pinMode(_pinCosP, INPUT_ANALOG);
    pinMode(_pinCosM, INPUT_ANALOG);    // INPUT_ANALOG
    analogReadResolution(12);
#else
    pinMode(_pinSinP, INPUT);
    pinMode(_pinSinM, INPUT);
    pinMode(_pinCosP, INPUT);
    pinMode(_pinCosM, INPUT);    // INPUT_ANALOG
#endif

    // do first reading in case we do filtering.
    sinm_raw = analogRead(_pinSinM);
    sinp_raw = analogRead(_pinSinP);

    cosm_raw = analogRead(_pinCosM);
    cosp_raw = analogRead(_pinCosP);

    initialized = true;
}

// #endif