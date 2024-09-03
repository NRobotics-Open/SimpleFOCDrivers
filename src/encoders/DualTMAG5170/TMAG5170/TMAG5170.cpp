#include "TMAG5170.h"


TMAG5170::TMAG5170() {
}

void TMAG5170::begin(SPIClass* _spi, int SPI_CSpin)
{
  spi = _spi;
  _SPI_CS = SPI_CSpin;
  pinMode(_SPI_CS, OUTPUT);
  digitalWrite(_SPI_CS, HIGH);
  // spi->beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
  spi->beginTransaction(TMAG5170_Settings);
  spi->begin();
  readWriteFrame();  // write initial frame to disable CRC
  outBuf[3] = 0x00;
}

void TMAG5170::readWriteFrame(void) {
  digitalWrite(_SPI_CS, LOW);
  spi->beginTransaction(TMAG5170_Settings);
  for (int i = 0; i < 4; i++) {  // transfer 4 bytes
    inBuf[i] = spi->transfer(outBuf[i]);
  }
  spi->endTransaction();
  digitalWrite(_SPI_CS, HIGH);

  return;
}

void TMAG5170::generateFrame(uint8_t read, uint8_t offset,
                             uint16_t regContent) {
  outBuf[0] = (read << 7) | offset;
  outBuf[1] = (regContent & 0xff00) >> 8;
  outBuf[2] = regContent & 0x00ff;

  outBuf[3] = 0x00;
  // outBuf[3] = calculateCRC(outBuf);

  return;
}

void TMAG5170::setAverage(uint16_t convAvg) {
  readReg(DEVICE_CONFIG);
  _inReg &= ~CONV_AVG_MASK;
  _inReg |= convAvg;
  generateFrame(0, DEVICE_CONFIG, _inReg);
  readWriteFrame();

  return;
}

void TMAG5170::setOperatingMode(uint16_t operatingMode) {
  readReg(DEVICE_CONFIG);
  _inReg &= ~OPERATING_MODE_MASK;
  _inReg |= operatingMode;
  generateFrame(0, DEVICE_CONFIG, _inReg);
  readWriteFrame();

  return;
}

void TMAG5170::setTriggerMode(uint16_t triggerMode) {
  readReg(SYSTEM_CONFIG);
  _inReg &= ~TRIGGER_MODE_MASK;  // clear TRIGGER_MODE bits
  _inReg |= triggerMode;
  generateFrame(0, SYSTEM_CONFIG, _inReg);
  readWriteFrame();
  return;
}

void TMAG5170::enableChannels(bool enableX, bool enableY, bool enableZ) {
  // form MAG_CH_EN bits from individual boolean value
  uint16_t MAG_CH_EN = 0;
  MAG_CH_EN |= (enableZ << 2) | (enableY << 1) | enableX;
  MAG_CH_EN <<= 6;

  readReg(SENSOR_CONFIG);
  _inReg &= ~MAG_CH_EN_MASK;  // clear MAG_CH_EN bits
  _inReg |= MAG_CH_EN;        // configure MAG_CH_EN bits
  generateFrame(0, SENSOR_CONFIG, _inReg);
  readWriteFrame();

  return;
}

void TMAG5170::setRange(uint8_t rangeX, uint8_t rangeY, uint8_t rangeZ) {
  _magneticRange[0] = rangeX;
  _magneticRange[1] = rangeY;
  _magneticRange[2] = rangeZ;
  // form RANGE bits from individual value
  uint16_t RANGE = 0;
  RANGE |= ((rangeZ) << 4) | (rangeY << 2) | rangeX;

  readReg(SENSOR_CONFIG);
  _inReg &= ~(Z_RANGE_MASK | Y_RANGE_MASK | X_RANGE_MASK);
  // clear RANGE bits
  _inReg |= RANGE;  // configure RANGE bits
  generateFrame(0, SENSOR_CONFIG, _inReg);
  readWriteFrame();

  return;
}

uint16_t TMAG5170::readReg(uint8_t offset) {
  generateFrame(1, offset, 0);
  // set the first bit to 1 for reading
  readWriteFrame();
  _inReg = (uint16_t)inBuf[1] << 8 | (uint16_t)inBuf[2];

  return _inReg;
}

int16_t TMAG5170::getBRaw(uint8_t axis) {
  readReg(axis);
  return _inReg;
}

float TMAG5170::getB(uint8_t axis) {
  readReg(axis);  // read channel result register

  int16_t raw;
  raw = (int16_t)inBuf[1] << 8 | (int16_t)inBuf[2];
  float result;
  result = raw / 65536.f * 2;  // convert into ratio

  int range;
  switch (axis) {
    case AXIS_X:
      range = _magneticRange[0];
      break;
    case AXIS_Y:
      range = _magneticRange[1];
      break;
    case AXIS_Z:
      range = _magneticRange[2];
      break;
  }
  switch (range) {
    case RANGE_150mT:
      result *= 150;
      break;
    case RANGE_75mT:
      result *= 75;
      break;
    case RANGE_300mT:
      result *= 300;
      break;
  }
  return result;
}


uint8_t TMAG5170::calculateCRC( uint8_t data[] )
{
    int i = 0;
    uint8_t crc = 0x00;
    uint32_t n;

    // Build TX and RX byte array
    uint8_t d[32] = { 0 };

    n = (data[0] << 24)|(data[1] << 16)|(data[2] << 8)|(data[3]);

    while (n>0)
    {
        d[i] = n&1;
        n = n>>1;
        i++;
    }

    crc |= d[30] ^ d[26] ^ d[25] ^ d[24] ^ d[23] ^ d[21] ^ d[19] ^ d[18] ^ d[15] ^ d[11] ^ d[10] ^ d[9] ^ d[8] ^ d[6] ^ d[4] ^ d[3] ^ d[0] ^ 1;
    crc |= (d[31] ^ d[30] ^ d[27] ^ d[23] ^ d[22] ^ d[21] ^ d[20] ^ d[18] ^ d[16] ^ d[15] ^ d[12] ^ d[8] ^ d[7] ^ d[6] ^ d[5] ^ d[3] ^ d[1] ^ d[0] ^ 1 ^ 1) << 1;
    crc |= (d[31] ^ d[28] ^ d[24] ^ d[23] ^ d[22] ^ d[21] ^ d[19] ^ d[17] ^ d[16] ^ d[13] ^ d[9] ^ d[8] ^ d[7] ^ d[6] ^ d[4] ^ d[2] ^ d[1] ^ 1 ^ 1) << 2;
    crc |= (d[29] ^ d[25] ^ d[24] ^ d[23] ^ d[22] ^ d[20] ^ d[18] ^ d[17] ^ d[14] ^ d[10] ^ d[9] ^ d[8] ^ d[7] ^ d[5] ^ d[3] ^ d[2] ^ 1) << 3;

    return crc;
}
