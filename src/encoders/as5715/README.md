# SimpleFOC Driver for AS5715


## Warning


## Hardware setup



## Software setup

```c++
#include "Arduino.h"
#include "SimpleFOC.h"
#include "SimpleFOCDrivers.h"
#include "encoders/as5715/as5715.h"


#define SIN_M_PIN A4  
#define SIN_P_PIN PC2  
#define COS_M_PIN A5   
#define COS_P_PIN PC3  

AS5715 encoder = AS5715(2, SIN_P_PIN,SIN_M_PIN,COS_P_PIN, COS_M_PIN);


void setup() {
  Serial.begin(115200);
  Serial.println("init AS5715 encoder");
  
  encoder.init();
}

void loop(){

  encoder.update();
  Serial.print("\t");
  Serial.print(encoder.getAngle());
  Serial.print("\t");
  Serial.print(encoder.getVelocity());
  Serial.print("\t");
  Serial.println(encoder.getFullRotations());

  delay(10);
}
```