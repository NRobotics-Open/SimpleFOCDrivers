
#ifndef __MAGNETIC_SENSOR_AS5145_H__
#define __MAGNETIC_SENSOR_AS5145_H__

#include "Arduino.h"
#include "SPI.h"
#include "common/base_classes/Sensor.h"

#ifndef MSBFIRST
#define MSBFIRST BitOrder::MSBFIRST
#endif

#define AS5311_BITORDER MSBFIRST
#define AS5311_CPP 4096
#define _2PI 6.28318530718f


// if CLK is HIGH at CS-LOW -> Normal operation (SPI_MODE2) ( 1 bit Offset?? )
// if CLK is LOW  at CS-LOW -> Output of the Magnetic Field Strength Data instead of angle !!! (SPI_MODE0) 
static SPISettings AS5311SSISettings(1000000, AS5311_BITORDER, SPI_MODE2);// @suppress("Invalid arguments")


class MagneticSensorAS5311 : public Sensor {
public:
	MagneticSensorAS5311(SPISettings settings = AS5311SSISettings);
	virtual ~MagneticSensorAS5311();

    virtual float getSensorAngle() override;

	virtual void init(uint16_t _poles, uint8_t _CS_PIN, SPIClass* _spi);

    uint16_t readRawAngleSSI();
	void updateRAW();

    uint8_t OCF;
    uint8_t COF;

    uint8_t LIN;

    uint8_t MagINC;
    uint8_t MagDEC;

	uint16_t lastRAW;
	int16_t ppCounter; 

private:
	uint8_t CS_PIN;
	uint16_t poles;
	float pAngle;

	SPIClass* spi;
	SPISettings settings;
};



#endif