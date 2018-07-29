#include <Controllino.h>

// NUMBER OF DISPENSER
#define DISPENSER_N 6

// MAXIMUM QUANTITY FOR DETERGENTS [liters] AND BOTTLES [pieces]
#define DET_CNTMAX 24
#define BOTTLE_CNTMAX 5  // test value, real value = 24

// COIN VALIDATOR PINS
#define COIN_SIGPIN CONTROLLINO_IN0
#define COIN_PWRPIN CONTROLLINO_R15

// NOTICE it takes about 37 second for pump 6 to dispense one liter. use this to
// make calculation for dispense checking

// PULSES FOR ONE LITER
#define PULSESXLITER_1 1106
#define PULSESXLITER_2 837
#define PULSESXLITER_3 823
#define PULSESXLITER_4 1105
#define PULSESXLITER_5 1115  // calibrate this pump
#define PULSESXLITER_6 1129

// TIME FOR ONE LITER in seconds
#define TIMEXLITER_1 37
#define TIMEXLITER_2 65
#define TIMEXLITER_3 54
#define TIMEXLITER_4 197
#define TIMEXLITER_5 100  // default value, calibrate he pump
#define TIMEXLITER_6 210

// PRICES in cents of euro
#define DETPRICE_1 200
#define DETPRICE_2 200
#define DETPRICE_3 200
#define DETPRICE_4 200
#define DETPRICE_5 200
#define DETPRICE_6 200
#define BOTTLEPRICE 10

// FLOW METERS SIGNAL PINS
#define FLOW_PIN_1 CONTROLLINO_A0
#define FLOW_PIN_2 CONTROLLINO_A1
#define FLOW_PIN_3 CONTROLLINO_A2
#define FLOW_PIN_4 CONTROLLINO_A3
#define FLOW_PIN_5 CONTROLLINO_A4
#define FLOW_PIN_6 CONTROLLINO_A5

// BUTTONS SIGNAL PINS
#define BTN_PIN_1 CONTROLLINO_A6
#define BTN_PIN_2 CONTROLLINO_A7
#define BTN_PIN_3 CONTROLLINO_A8
#define BTN_PIN_4 CONTROLLINO_A9
#define BTN_PIN_5 CONTROLLINO_A10
#define BTN_PIN_6 CONTROLLINO_A11
#define BTN_PIN_BOTTLE CONTROLLINO_A13

// BUTTON'S LED PINS
#define BTN_LED_1 CONTROLLINO_D0
#define BTN_LED_2 CONTROLLINO_D1
#define BTN_LED_3 CONTROLLINO_D2
#define BTN_LED_4 CONTROLLINO_D3
#define BTN_LED_5 CONTROLLINO_D4
#define BTN_LED_6 CONTROLLINO_D5
#define BTN_LED_BOTTLE CONTROLLINO_D6

// PUMPS RELAYS
#define PUMP_PIN_1 CONTROLLINO_R4
#define PUMP_PIN_2 CONTROLLINO_R5
#define PUMP_PIN_3 CONTROLLINO_R6
#define PUMP_PIN_4 CONTROLLINO_R7
#define PUMP_PIN_5 CONTROLLINO_R8
#define PUMP_PIN_6 CONTROLLINO_R9

// KEY SELECTOR PIN
#define MAINT_PIN CONTROLLINO_D8  // change this

// BOTTLE MOTOR PIN
#define BOTTLE_PIN CONTROLLINO_R14
// BOTTLE ENDSTOP SIGNAL PIN
#define BOTTLE_ENDSTOP CONTROLLINO_A15
// BOTTLE ULTRASONIC SENSOR PINS
#define BOTTLE_TRIG CONTROLLINO_D18
#define BOTTLE_ECHO CONTROLLINO_D19
// MINIMUM DISTANCE FOR BOTTLE PRESENCE in cm
#define BOTTLE_DISTANCE 25
// TIMEOUT FOR BOTTLE REMOVING in seconds
#define BOTTLE_TIMEOUT 10

// TIMEOUT FOR SECOND BUTTON PRESSION in ms
#define DET_CONFIRM_TIMEOUT 10000

// SCREENS TIMEOUTS in ms
#define SCREEN_TIMEOUT 3000
#define START_SCREEN_TIMEOUT 2000
#define ERR_SCREEN_TIMEOUT 3000
#define LOWCREDIT_SCREEN_TIMEOUT 3000

// BUTTON PRESSION TIME, in seconds
#define LONGPRESS_TIME 3