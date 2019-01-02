#include "SdLogger.h"

// TODO better limit filename size
SdLogger::SdLogger(String filename) { _filename = filename; }

bool SdLogger::begin(uint8_t csPin, SPISettings spiSettings) {
  // TODO verify if CS pin is low after begin()
  return SdFat::begin(csPin, spiSettings);
}

int SdLogger::writeLog(String data) {
  // the first call to open also create the file
  File file = open(_filename + ".log", FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return -1;
  }

  file.println(data);
  return 0;
}

int SdLogger::writeInfo(String data) {
  // the first call to open also create the file
  File file = open(_filename + ".inf", FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return -1;
  }

  file.println(data);
  return 0;
}

int SdLogger::writeErr(String data) {
  // the first call to open also create the file
  File file = open(_filename + ".err", FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return -1;
  }

  file.print("ERR \t");
  file.println(data);
  return 0;
}

int SdLogger::writeWarn(String data) {
  // the first call to open also create the file
  File file = open(_filename + ".err", FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return -1;
  }

  file.print("WARN\t");
  file.println(data);
  return 0;
}

int SdLogger::printLog() {
  File file = open(_filename + ".log", FILE_READ);

  if (!file) {
    Serial.println(F("Error opening file"));
    return -1;
  }

  // read file until there's nothing else in it:
  while (file.available()) {
    Serial.write(file.read());
  }

  file.close();
}