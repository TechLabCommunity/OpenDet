#include <Arduino.h>
#include "Controllino.h"
#include "Dispenser.h"

#define DISPENSER_N 6
#define  FLOW_PIN CONTROLLINO_D1
#define  BTN_PIN CONTROLLINO_D2
#define  PUMP_PIN CONTROLLINO_R0
#define  PULSESXLITER 200


Dispenser disp1(FLOW_PIN, BTN_PIN, PUMP_PIN, PULSESXLITER);


void setup() {
    /* Load basics constant from SD:
     *  pulses/liter for all fluxostat, strings for detergents (linked to buttons pin)
     *  and some setting about the pumps (?)
     */

}

void loop() {
    /* check button
     *  if pressed check credit
     *    if credit ok release detergent
     *    if credit bad display message on display
     * check credit pin (ask Savere how it works)
     *  add credit
     */

     /*for (int i = 0; i < DISPENSER_N; i++) {
       //chkBtn(); //check button status
       //store the number of the button pressed
       //to pass it to the next if
     }

     if (btnPressed) {
       //chkCredit();
       if(credit >= thisDetergentCost){
         decreaseCredit();
         dispense(1);
         giveRest();
       }
     }
     else{
       insuffCredit();
     }

     chkCoin();
     updateCredit();
     */
}
