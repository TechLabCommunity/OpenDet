#ifndef SDLOGGER_H
#define SDLOGGER_H

#include <SPI.h>
#include <SdFat.h>

class SdLogger : public SdFat {  //, public SdSpiCard {
 public:
  SdLogger(String);

  bool begin(uint8_t csPin = SS, SPISettings spiSettings = SPI_FULL_SPEED);

  int writeLog(String);
  int writeInfo(String);
  int writeErr(String);
  int writeWarn(String);

  int printLog();
  int printInfo();
  int printErr();

 private:
  String _filename;
};

#endif