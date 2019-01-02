/*
 * CH926.h - Library for ch-926 coin acceptor
 * Created by Michele Brunelli on 16 dec 2017
 * Ask the creator if you want to use this library, email me at
 * brunellim94@gmail.com
 */

#ifndef CH926_H
#define CH926_H

#include <Global.h>

#define ACTIVE_LOW 0
#define ACTIVE_HIGH 1

#define NC 0
#define NO 1

class CH926 {
 public:
  CH926(int, int, int, int);
  void start();
  void stop();
  int countPulses(unsigned long);

  void startInterrupt();
  void stopInterrupt();
  uint32_t counter();

  static uint32_t _counter;

 private:
  int _sigPin;
  int _pwrPin;
  int _sigLogic;
  int _pwrLogic;
  int _state;
  // TODO trovare modo di avere counter divisi per diverse gettoniere
  static void ISR_counter();
};

#endif
