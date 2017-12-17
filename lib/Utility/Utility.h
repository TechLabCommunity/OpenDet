#ifndef UTILITY_H
#define UTILITY_H

#ifdef DEBUGGER
  #define DEBUGBEGIN(b) Serial.begin(b)
  #define DEBUG(s) Serial.print(s)
#else
  #define DEBUGBEGIN(b)
  #define DEBUG(s)
#endif

#endif
