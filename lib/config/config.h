#include <Controllino.h>

#define DISPENSER_N 6

// Maximum quantity for detergents [liters] and bottle [pieces]
#define DET_CNTMAX 24
#define BOTTLE_CNTMAX 24

#define COIN_SIGPIN CONTROLLINO_IN0
#define COIN_PWRPIN CONTROLLINO_R15

// for pulses and prices use variables, so we can modify them in a setup routine
#define PULSESXLITER_1 1110
#define PULSESXLITER_2 813
#define PULSESXLITER_3 805
#define PULSESXLITER_4 1096
#define PULSESXLITER_5 1115
#define PULSESXLITER_6 1120

#define DETPRICE_1 200  // cents of euro
#define DETPRICE_2 200  // cents of euro
#define DETPRICE_3 200  // cents of euro
#define DETPRICE_4 200  // cents of euro
#define DETPRICE_5 200  // cents of euro
#define DETPRICE_6 200  // cents of euro

#define BOTTLEPRICE 10

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
#define BTN_PIN_BOTTLE CONTROLLINO_A13

#define PUMP_PIN_1 CONTROLLINO_R4
#define PUMP_PIN_2 CONTROLLINO_R5
#define PUMP_PIN_3 CONTROLLINO_R6
#define PUMP_PIN_4 CONTROLLINO_R7
#define PUMP_PIN_5 CONTROLLINO_R8
#define PUMP_PIN_6 CONTROLLINO_R9

#define BOTTLE_PIN CONTROLLINO_R14
#define BOTTLE_ENDSTOP CONTROLLINO_A15
#define BOTTLE_TRIG CONTROLLINO_D18
#define BOTTLE_ECHO CONTROLLINO_D19
// minimum distance for bottle presence, in cm
#define BOTTLE_DISTANCE 25
// timeout for bottle removing in seconds
#define BOTTLE_TIMEOUT 10

#define BTN_LED_1 CONTROLLINO_D0
#define BTN_LED_2 CONTROLLINO_D1
#define BTN_LED_3 CONTROLLINO_D2
#define BTN_LED_4 CONTROLLINO_D3
#define BTN_LED_5 CONTROLLINO_D4
#define BTN_LED_6 CONTROLLINO_D5
#define BTN_LED_BOTTLE CONTROLLINO_D6

// screen's timeouts in ms
#define SCREEN_TIMEOUT 3000
#define START_SCREEN_TIMEOUT 2000
#define ERR_SCREEN_TIMEOUT 5000
#define LOWCREDIT_SCREEN_TIMEOUT 3000
