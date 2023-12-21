# AS5311 SimpleFOC driver

SSI protocol driver for the AMS AS5311 magnetic encoder. 
The SSI protocol is "emulated" using the SPI peripheral.


## Hardware setup

Wire the sensor's data (DO) line to the MISO pin, nCS, SCK as normal. 
Leave the MOSI pin unconnected.

## Software setup

```c++
#include <SimpleFOC.h>
#include "SimpleFOCDrivers.h"
#include "encoders/as5311/MagneticSensorAS5311.h"

MagneticSensorAS5311 encoder;

#define ENC_MISO  PA6
#define ENC_MOSI  PA7
#define ENC_SCK   PA5
#define ENC_CS    PA4


void setup() {

  Serial.begin(115200);

  // SPI Pins
  SPI.setMISO(ENC_MOSI);
  SPI.setMISO(ENC_MISO);
  SPI.setSCLK(ENC_SCK);

  SPI.begin();
  encoder.init(4, ENC_CS, &SPI);

}

void loop() {

  encoder.update();
  Serial.print(encoder.getAngle());
  Serial.print("\t");
  Serial.print(encoder.getVelocity());
  Serial.print("\tRAW:");
  Serial.print(encoder.lastRAW);
  Serial.print("\tppC:");
  Serial.print(encoder.ppCounter);  
  Serial.print("\tOCF:");
  Serial.print(encoder.OCF);
  Serial.print("\tCOF:");
  Serial.print(encoder.COF);
  Serial.print("\tLIN:");
  Serial.print(encoder.LIN);
  Serial.print("\tINC:");
  Serial.print(encoder.MagINC);
  Serial.print("\tDEC:");
  Serial.print(encoder.MagDEC);

  Serial.println();

  delay(10);
}
```