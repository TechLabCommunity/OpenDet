#include <Controllino.h>

#define DISPENSER_N 6

#define COIN_SIGPIN CONTROLLINO_A12  // A12 for MEGA
#define COIN_PWRPIN CONTROLLINO_A13  // TODO wire that pin

// for pulses and prices use variables, so we can modify them in a setup routine
#define PULSESXLITER_1 1101
#define PULSESXLITER_2 800  // da calibrare
#define PULSESXLITER_3 800  // da calibrare
#define PULSESXLITER_4 1101
#define PULSESXLITER_5 1101
#define PULSESXLITER_6 1101

#define DETPRICE_1 100  // cents of euro
#define DETPRICE_2 120  // cents of euro
#define DETPRICE_3 130  // cents of euro
#define DETPRICE_4 150  // cents of euro
#define DETPRICE_5 180  // cents of euro
#define DETPRICE_6 200  // cents of euro

#define FLOW_PIN_1 CONTROLLINO_A0
#define FLOW_PIN_2 CONTROLLINO_A1
#define FLOW_PIN_3 CONTROLLINO_A2
#define FLOW_PIN_4 CONTROLLINO_A3
#define FLOW_PIN_5 CONTROLLINO_A4
#define FLOW_PIN_6 CONTROLLINO_A5

#define BTN_PIN_1 CONTROLLINO_A6
#define BTN_PIN_2 CONTROLLINO_A7
#define BTN_PIN_3 CONTROLLINO_A8
#define BTN_PIN_4 CONTROLLINO_A9
#define BTN_PIN_5 CONTROLLINO_A10
#define BTN_PIN_6 CONTROLLINO_A11

#define PUMP_PIN_1 CONTROLLINO_R4
#define PUMP_PIN_2 CONTROLLINO_R5
#define PUMP_PIN_3 CONTROLLINO_R6
#define PUMP_PIN_4 CONTROLLINO_R7
#define PUMP_PIN_5 CONTROLLINO_R8
#define PUMP_PIN_6 CONTROLLINO_R9

#define BTN_LED_1 CONTROLLINO_D0
#define BTN_LED_2 CONTROLLINO_D1
#define BTN_LED_3 CONTROLLINO_D2
#define BTN_LED_4 CONTROLLINO_D3
#define BTN_LED_5 CONTROLLINO_D4
#define BTN_LED_6 CONTROLLINO_D5
