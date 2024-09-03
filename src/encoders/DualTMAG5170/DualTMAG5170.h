
#pragma once

#include <Arduino.h>
#include "SPI.h"
#include "TMAG5170/TMAG5170.h"

#if defined(_STM32_DEF_)
  // #include <HardwareTimer.h>
#endif

#include "common/base_classes/Sensor.h"
#include "common/foc_utils.h"
#include "common/lowpass_filter.h"



class DualTMAG5170 : public Sensor {
  public:
    /**
    Encoder class constructor
    @param epp  encoder pol pairs
    */
    DualTMAG5170(SPIClass* _spi,int ENC_CS1, int ENC_CS2, int _polPairs);

    void init() override;
    int needsSearch() override;
    int hasIndex();  // !< function returning 1 if encoder has index pin and 0 if not.

    bool initialized = false;
    
    TMAG5170 sensor1 = TMAG5170();
    TMAG5170 sensor2 = TMAG5170();

    LowPassFilter filter1 = LowPassFilter(0.001); // Tf = 1ms
    LowPassFilter filter2 = LowPassFilter(0.001); // Tf = 1ms


    int _pinSinP,_pinSinM, _pinCosP,_pinCosM;

  // protected:
    float getSensorAngle() override;
    
  float b1X;
  float b1Y;
  float b1Z;

  float b2X;
  float b2Y;
  float b2Z;

  private:

	  SPIClass* spi;
	  SPISettings settings;
    int enc_CS1;
    int enc_CS2;

    int polPairs;
    float  polesAngle;

    float lastAlpha;  // last sub polpair angle
    int alphaTurns; // number of polpairs in one current angle 
};

