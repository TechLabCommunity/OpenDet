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
#include <avr/wdt.h>  //for software reset
#include <inttypes.h>

void inline SYSERR(String s) {
  DEBUG("SYSERR : " + s + "\n");
  while (1) {
  }
}

/*
void softwareReset() {
  // start watchdog with the provided prescaler
  // possible value for prescaler are defined in wdt.h (WDTO_15MS,...)
  wdt_enable(WDTO_15MS);
  // wait for the prescaler time to expire without sending the reset signal by
  // using the wdt_reset() method
  while (1) {
  }
}
*/

#endif
