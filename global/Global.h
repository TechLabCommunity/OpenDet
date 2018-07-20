#ifndef UTILITY_H
#define UTILITY_H

#ifdef FORCE_ARDUINO
#include <Arduino.h>
#else
#include <Controllino.h>
#endif

// Depending on DEBUGGER build_flags in platformio.ini
#ifdef DEBUGGER
#define DEBUGBEGIN(b) (Serial.begin(b))
#define DEBUG(s) (Serial.print(s))
#else
#define DEBUGBEGIN(b)
#define DEBUG(s)
#endif

#define uint unsigned int

#define VERSION_FIRMWARE "0.1a"

#include <MsTimer2.h>
#include <Print.h>
#include <SPI.h>
#include <Wire.h>
#include <inttypes.h>

void inline SYSERR(String s) {
  DEBUG("SYSERR : " + s + "\n");
  while (1) {
  }
}

#endif
