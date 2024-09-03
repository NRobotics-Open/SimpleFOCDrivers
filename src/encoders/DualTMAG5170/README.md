# SimpleFOC Driver for Dual TMAG5170


## Warning


## Hardware setup


## Software setup

```c++
#include <SimpleFOC.h>
#include "SimpleFOCDrivers.h"
#include "encoders/DualTMAG5170/DualTMAG5170.h"

#define ENC_MISO  PB4
#define ENC_MOSI  PB5
#define ENC_SCK   PB3

#define ENC_CS1 PC14
#define ENC_CS2 PC15

DualTMAG5170 encoder = DualTMAG5170(&SPI, ENC_CS1, ENC_CS2, 10);

void setup() {

  Serial.begin(115200);

  delay(1000);
  Serial.println("TMAG5170 Demo");

  SPI.setMISO(ENC_MISO);
  SPI.setMOSI(ENC_MOSI);
  SPI.setSCLK(ENC_SCK);

  SPI.begin();

  encoder.init();
}

uint32_t lastTime = 0;
void loop() {

  float val = encoder.getSensorAngle();

  int timeNow = millis();

  // Feedback Loop
  if(timeNow - lastTime > 100)
  {
    lastTime = timeNow;

    // Serial.print(encoder.b1X);
    // Serial.print(" ");

    // Serial.print(encoder.b2X);
    // Serial.print(" ");   

    Serial.print(val);
    Serial.print(" ");

    Serial.println();
  }

  delay(1);
}
```