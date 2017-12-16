#ifndef UTILITY_H
#define UTILITY_H

#ifdef DEBUG
  #define DEBUG(s) Serial.print(s)
#else
  #define DEBUG(s)
#endif

#endif
