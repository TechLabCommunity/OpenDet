#ifndef UTILITY_H
#define UTILITY_H

//Depending on DEBUGGER build_flags in platformio.ini
#ifdef DEBUGGER
  #define DEBUGBEGIN(b) (Serial.begin(b))
  #define DEBUG(s) (Serial.print(s))
#else
  #define DEBUGBEGIN(b)
  #define DEBUG(s)
#endif

void inline SYSERR(String s){
  DEBUG("SYSERR : "+s+"\n");
  while(1){}
}

#endif
