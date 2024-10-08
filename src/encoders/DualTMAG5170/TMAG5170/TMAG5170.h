#ifndef TMAG5170_H
#define TMAG5170_H
#include <Arduino.h>
#include "SPI.h"

//------------------Register Address------------------------
#define DEVICE_CONFIG 0x0
#define SENSOR_CONFIG 0x1
#define SYSTEM_CONFIG 0x2
#define ALERT_CONFIG 0x3
#define X_THRX_CONFIG 0x4
#define Y_THRX_CONFIG 0x5
#define Z_THRX_CONFIG 0x6
#define T_THRX_CONFIG 0x7
#define CONV_STATUS 0x8
#define X_CH_RESULT 0x9
#define Y_CH_RESULT 0xA
#define Z_CH_RESULT 0xB
#define TEMP_RESULT 0xC
#define AFE_STATUS 0xD
#define SYS_STATUS 0xE
#define TEST_CONFIG 0xF
#define OSC_MONITOR 0x10
#define MAG_GAIN_CONFIG 0x11
#define ANGLE_RESULT 0x13
#define MAGNITUDE_RESULT 0x14

//------------------DEVICE_CONFIG------------------------
#define CONV_AVG_MASK 0x7000
#define CONV_AVG_1x 0x0
#define CONV_AVG_2x 0x1000
#define CONV_AVG_4x 0x2000
#define CONV_AVG_8x 0x3000
#define CONV_AVG_16x 0x4000
#define CONV_AVG_32x 0x5000

#define MAG_TEMPCO_MASK 0x300
#define MAG_TEMPCO_0pd 0x0
#define MAG_TEMPCO_0R12pd 0x100
#define MAG_TEMPCO_0R2pd 0x300

#define OPERATING_MODE_MASK 0x70
#define OPERATING_MODE_ConfigurationMode 0x0
#define OPERATING_MODE_StandbyMode 0x10
#define OPERATING_MODE_activeMeasureMode 0x20
#define OPERATING_MODE_ActiveTriggerMode 0x30
#define OPERATING_MODE_WakeupAndSleepMode 0x40
#define OPERATING_MODE_SleepMode 0x50
#define OPERATING_MODE_DeepsleepMode 0x60

#define T_CH_EN_TempChannelDisabled 0x0
#define T_CH_EN_TempChannelEnabled 0x8

#define T_RATE_sameAsOtherSensors 0x0
#define T_RATE_oncePerConversionSet 0x4

#define T_HLT_EN_tempLimitCheckOff 0x0
#define T_HLT_EN_tempLimitCheckOn 0x2

#define TEMP_COMP_EN_TempCompensationDisenabled 0x0
#define TEMP_COMP_EN_TempCompensationEnabled 0x1
//------------------SENSOR_CONFIG------------------------
#define ANGLE_EN_NoAngleCalculation 0x0
#define ANGLE_EN_X_Y 0x4000
#define ANGLE_EN_Y_Z 0x8000
#define ANGLE_EN_Z_X 0xC000

#define SLEEPTIME_MASK 0x3C00
#define SLEEPTIME_1ms 0x0
#define SLEEPTIME_5ms 0x400
#define SLEEPTIME_10ms 0x800
#define SLEEPTIME_15ms 0xC00
#define SLEEPTIME_20ms 0x1000
#define SLEEPTIME_30ms 0x1400
#define SLEEPTIME_50ms 0x1800
#define SLEEPTIME_100ms 0x1C00
#define SLEEPTIME_500ms 0x2000
#define SLEEPTIME_1000ms 0x2400

#define MAG_CH_EN_MASK 0x3C0
#define MAG_CH_EN_OFF 0x0
#define MAG_CH_EN_Xenabled 0x40
#define MAG_CH_EN_Yenabled 0x80
#define MAG_CH_EN_XYenabled 0xC0
#define MAG_CH_EN_Zenabled 0x100
#define MAG_CH_EN_ZXenabled 0x140
#define MAG_CH_EN_YZenabled 0x180
#define MAG_CH_EN_XYZenaled 0x1C0
#define MAG_CH_EN_XYXenaled 0x200
#define MAG_CH_EN_YXYenaled 0x240
#define MAG_CH_EN_YZYenaled 0x280
#define MAG_CH_EN_ZYZenaled 0x2C0
#define MAG_CH_EN_ZXZenaled 0x300
#define MAG_CH_EN_XZXenaled 0x340
#define MAG_CH_EN_XYZYXenaled 0x380
#define MAG_CH_EN_XYZZYXenaled 0x3C0

#define Z_RANGE_MASK 0x30
#define Z_RANGE_150mT 0x0
#define Z_RANGE_75mT 0x10
#define Z_RANGE_300mT 0x20

#define Y_RANGE_MASK 0xC
#define Y_RANGE_150mT 0x0
#define Y_RANGE_75mT 0x4
#define Y_RANGE_300mT 0x8

#define X_RANGE_MASK 0x3
#define X_RANGE_150mT 0x0
#define X_RANGE_75mT 0x1
#define X_RANGE_300mT 0x2
//------------------SYSTEM_CONFIG------------------------
#define DIAG_SEL_AllDataPath 0x0
#define DIAG_SEL_enabledDataPath 0x1000
#define DIAG_SEL_enabledDataPathInsequence 0x2000
#define DIAG_SEL_enabledDataPathInsequence 0x3000

#define TRIGGER_MODE_MASK 0x600
#define TRIGGER_MODE_SPI 0x0
#define TRIGGER_MODE_nCSsyncPulse 0x200
#define TRIGGER_MODE_ALERTsyncPulse 0x400

#define DATA_TYPE_32bit 0x0
#define DATA_TYPE_12bit_XY 0x40
#define DATA_TYPE_12bit_XZ 0x80
#define DATA_TYPE_12bit_ZY 0xC0
#define DATA_TYPE_12bit_XT 0x100
#define DATA_TYPE_12bit_YT 0x140
#define DATA_TYPE_12bit_ZT 0x180
#define DATA_TYPE_12bit_AM 0x1C0

#define DIAG_EN_AFEdiagnosticsDisabled 0x0
#define DIAG_EN_ExecutionOftheDiagnosticsSelectedInDEVICE_CFG 0x20

#define Z_HLT_EN_ZaxisLimitCheckoff 0x0
#define Z_HLT_EN_ZaxisLimitCheckon 0x4

#define Y_HLT_EN_YaxisLimitCheckoff 0x0
#define Y_HLT_EN_YaxisLimitCheckon 0x2

#define X_HLT_EN_XaxisLimitCheckoff 0x0
#define X_HLT_EN_XaxisLimitCheckon 0x1
//------------------ALERT_CONFIG------------------------
#define ALERT_LATCH_sourcesNotLatched 0x0
#define ALERT_LATCH_sourcesLatched 0x2000

#define ALERT_MODE_interruptMode 0x0
#define ALERT_MODE_comparatorMode 0x1000

#define STATUS_ALRT_ALERTisNotAsserted 0x0
#define STATUS_ALRT_ALERTisAsserted 0x800

#define RSLT_ALRT_ALERTisNotUsedToSignal 0x0
#define RSLT_ALRT_ALERTisUsedToSignal 0x100

#define THRX_COUNT_1_ConversionResult 0x0
#define THRX_COUNT_2_ConversionResult 0x10
#define THRX_COUNT_3_ConversionResult 0x20
#define THRX_COUNT_4_ConversionResult 0x30

#define T_THRX_ALRT_ALERTisNotUsedToSignal 0x0
#define T_THRX_ALRT_ALERTisUsedToSignal 0x8

#define Z_THRX_ALRT_ALERTisNotUsedToSignal 0x0
#define Z_THRX_ALRT_ALERTisUsedToSignal 0x4

#define Y_THRX_ALRT_ALERTisNotUsedToSignal 0x0
#define Y_THRX_ALRT_ALERTisUsedToSignal 0x2

#define X_THRX_ALRT_ALERTisNotUsedToSignal 0x0
#define X_THRX_ALRT_ALERTisUsedToSignal 0x1
//------------------TEST_CONFIG------------------------
#define CRC_DIS_CRCenabled 0x0
#define CRC_DIS_CRCdisabled 0x4

#define OSC_CNT_CTL_ResetCounters 0x0
#define OSC_CNT_CTL_StartOscCounterdrivenbyHFOSC 0x1
#define OSC_CNT_CTL_StartOscCounterdrivenbyLFOSC 0x2
#define OSC_CNT_CTL_stopCounter 0x3
//------------------MAG_GAIN_CONFIG------------------------
#define GAIN_SELECTION_noAxisSelected 0x0
#define GAIN_SELECTION_XisSelected 0x4000
#define GAIN_SELECTION_YisSelected 0x8000
#define GAIN_SELECTION_ZisSelected 0xC000
//------------------INTERFACE MACROS-----------------------
#define RANGE_150mT 0
#define RANGE_75mT 1
#define RANGE_300mT 2

#define AXIS_X X_CH_RESULT
#define AXIS_Y Y_CH_RESULT
#define AXIS_Z Z_CH_RESULT

static SPISettings TMAG5170_Settings(100000, MSBFIRST, SPI_MODE0);

class TMAG5170 {
 public:
  // Constructor
  TMAG5170();
  void begin(SPIClass* _spi, int SPI_CSpin);

  uint8_t inBuf[4];
  uint8_t outBuf[4] = {0x0F, 0x00, 0x04, 0x07};  // initial frame to disable CRC

  // functions for modifying settings on the TMAG5170
  void setAverage(uint16_t convAvg);
  void setOperatingMode(uint16_t operatingMode);
  void setTriggerMode(uint16_t triggerMode);
  void enableChannels(bool enableX, bool enableY, bool enableZ);
  void setRange(uint8_t rangeX, uint8_t rangeY, uint8_t rangeZ);

  // functions for reading data or setting
  uint16_t readReg(uint8_t offset);
  int16_t getBRaw(
      uint8_t axis);         // reads magnetic flux density and return raw value
  float getB(uint8_t axis);  // reads magnetic flux density in mT

 private:
  void readWriteFrame(void);
  void generateFrame(uint8_t read, uint8_t offset, uint16_t regContent);
  uint8_t calculateCRC( uint8_t data[] );

  SPIClass* spi;
  uint16_t _SPI_CS;
  uint16_t _inReg;
  uint8_t _magneticRange[3] = {0, 0, 0};
};  // class TMAG5170

#endif
