#include "./MagneticSensorAS5311.h"
#include "common/foc_utils.h"
#include "common/time_utils.h"

MagneticSensorAS5311::MagneticSensorAS5311(SPISettings settings) : settings(settings) {

}


MagneticSensorAS5311::~MagneticSensorAS5311() {

}

void MagneticSensorAS5311::init(uint16_t _poles, uint8_t _CS_PIN, SPIClass* _spi = &SPI) {
    this->CS_PIN = _CS_PIN;
    this->poles = _poles;
    this->pAngle = _2PI / (float)poles;
    this->spi=_spi;

    pinMode(this->CS_PIN,OUTPUT);
    digitalWrite(this->CS_PIN, HIGH);

    this->Sensor::init();
}

void MagneticSensorAS5311::updateRAW() {
    uint16_t rawData = readRawAngleSSI();

    // calculate the polpair turns
    if (lastRAW != rawData)
    {
        if ((rawData < (AS5311_CPP/4)) && ( lastRAW > (AS5311_CPP/4)*3)) {
            ppCounter += 1;
        }
        else if ((rawData > (AS5311_CPP/4)*3) && ( lastRAW < (AS5311_CPP/4))) {
            ppCounter -= 1;
        }
        lastRAW = rawData;
    
        if ( ppCounter < 0 ) 
        {
            ppCounter = (poles-1);
        }
        else if (ppCounter >= poles)
        {
            ppCounter = 0;
        }
    }
}


float MagneticSensorAS5311::getSensorAngle() {
    float angle_data = (((float)lastRAW / (float)AS5311_CPP) * pAngle) + ppCounter * pAngle;
    // return the shaft angle
    return angle_data;
}


uint16_t MagneticSensorAS5311::readRawAngleSSI() {

    uint8_t spiBuffer[3];
    digitalWrite(CS_PIN, LOW);
    spi->beginTransaction(AS5311SSISettings);
    spi->transfer(spiBuffer,3);
    spi->endTransaction();
    digitalWrite(CS_PIN, HIGH);

    // first bit is not valid 
    uint16_t encValue = ((uint16_t)(spiBuffer[0]&0x7F)<<5) + (uint16_t)(spiBuffer[1]>>3);

    // | B0_7 | B0_6 | B0_5 | B0_4 | B0_3 | B0_2 | B0_1 | B0_0 | B1_7 | B1_6 | B1_5 | B1_4 | B1_3 | B1_2 | B1_1 | B1_0 | B2_7 | B2_6 | B2_5 | B2_4 | B2_3 | B2_2 | B2_1 | B2_0 |
    // | ~~~~ | D11  | D10  | D09  | D08  | D07  | D06  | D05  | D04  | D03  | D02  | D01  | D00  | OCF  | COF  | LIN  | MINC | MDEC | PAR  | ~~~~ | ~~~~ | ~~~~ | ~~~~ | ~~~~ |
    OCF = (spiBuffer[1] & 0x04)>>2;
    COF = (spiBuffer[1] & 0x02)>>1;

    LIN = (spiBuffer[1] & 0x01);

    MagINC = (spiBuffer[2] & 0x80)>>7;
    MagDEC = (spiBuffer[2] & 0x40)>>6;

    uint8_t PAR = (spiBuffer[2] & 0x20)>>5;

    int parity = 0;
    for(int i = 0; i< 12; i++)
    {
        if((1<<i)& encValue)
        {
            parity++;
        }
    }
    parity+= OCF;
    parity+= COF;
    parity+= LIN;
    parity+= MagINC;
    parity+= MagDEC;

    parity  = (parity + PAR)%2;

    if((parity == 0)&&(OCF==1)&&(COF==0))
    {
        return(encValue);
    }
    return(lastRAW);
}; // 12bit linear polpair value
