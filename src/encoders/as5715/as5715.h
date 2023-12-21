
#pragma once

#include <Arduino.h>


#if defined(_STM32_DEF_)
  // #include <HardwareTimer.h>
#endif

#include "common/base_classes/Sensor.h"
#include "common/foc_utils.h"

class AS5715 : public Sensor {
  public:
    /**
    Encoder class constructor
    @param epp  encoder pol pairs
    */
    explicit AS5715(unsigned int epp, int pinSinP, int pinSinM, int pinCosP,int pinCosM);

    void init() override;
    int needsSearch() override;
    int hasIndex();  // !< function returning 1 if encoder has index pin and 0 if not.

    bool initialized = false;
    
    int _pinSinP,_pinSinM, _pinCosP,_pinCosM;

  protected:
    float getSensorAngle() override;
    
  private:
    float epoles;  //!< encoder cpr number
    float polesAngle;

    float sinm_raw;
    float sinp_raw;
    
    float cosm_raw;
    float cosp_raw;

    float lastAlpha;
    
    int32_t alphaTurns;
    int32_t nAlphaTurns;
};

