#include <CH926.h>
#include <DetScreen.h>
#include <Dispenser.h>
#include <Global.h>
#include <config.h>

// TODO
/* When inserting the money the credit updates with steps of 10 cents. To
 * display directly the final credit whe have to insert a timer that control the
 * insertion ending. For exampe after 3 times the pulse time of the coin
 * acceptor we update the credit
 */
/* The error array must be saved in the SD and uploaded in the setup. To reset
 * the errors implement a specific procedure
 */
/* Implement log and error files in the sd. In the log file append all events,
 * in the error file append all errors
 */
/* Implement routine for RTC update
 *
 */

int SRF05_measureDistance(uint8_t, uint8_t);

// ATTENZIONE: nome prodotto deve avere meno di 20 caratteri
Dispenser disp_1(BTN_PIN_1, FLOW_PIN_1, PUMP_PIN_1, PULSESXLITER_1, "Test_1",
                 DETPRICE_1, DET_CNTMAX,
                 10000.0 / PULSESXLITER_1 * TIMEXLITER_1);
Dispenser disp_2(BTN_PIN_2, FLOW_PIN_2, PUMP_PIN_2, PULSESXLITER_2, "Test_2",
                 DETPRICE_2, DET_CNTMAX,
                 10000.0 / PULSESXLITER_2 * TIMEXLITER_2);
Dispenser disp_3(BTN_PIN_3, FLOW_PIN_3, PUMP_PIN_3, PULSESXLITER_3, "Test_3",
                 DETPRICE_3, DET_CNTMAX,
                 10000.0 / PULSESXLITER_3 * TIMEXLITER_3);
Dispenser disp_4(BTN_PIN_4, FLOW_PIN_4, PUMP_PIN_4, PULSESXLITER_4, "Test_4",
                 DETPRICE_4, DET_CNTMAX,
                 10000.0 / PULSESXLITER_4 * TIMEXLITER_4);
Dispenser disp_5(BTN_PIN_5, FLOW_PIN_5, PUMP_PIN_5, PULSESXLITER_5, "Test_5",
                 DETPRICE_5, DET_CNTMAX,
                 10000.0 / PULSESXLITER_5 * TIMEXLITER_5);
Dispenser disp_6(BTN_PIN_6, FLOW_PIN_6, PUMP_PIN_6, PULSESXLITER_6, "Test_6",
                 DETPRICE_6, DET_CNTMAX,
                 10000.0 / PULSESXLITER_6 * TIMEXLITER_6);

// array for dispenser object
Dispenser* dispArr[DISPENSER_N] = {&disp_1, &disp_2, &disp_3,
                                   &disp_4, &disp_5, &disp_6};
// array for buttons pins
int btnArr[DISPENSER_N + 1] = {BTN_PIN_1, BTN_PIN_2, BTN_PIN_3,     BTN_PIN_4,
                               BTN_PIN_5, BTN_PIN_6, BTN_PIN_BOTTLE};
// array for leds pins
int ledArr[DISPENSER_N + 1] = {BTN_LED_1, BTN_LED_2, BTN_LED_3,     BTN_LED_4,
                               BTN_LED_5, BTN_LED_6, BTN_LED_BOTTLE};

// array for status: 0=OK, -1=EMPTY, -3=ERROR, -4=EMPTY+ERROR
int statusArr[DISPENSER_N + 1] = {0};

int bottErr = 0;
int bottCnt = BOTTLE_CNTMAX;

CH926 coin(COIN_SIGPIN, COIN_PWRPIN, NO, ACTIVE_HIGH);
uint32_t CH926::_counter = 0;

DetScreen lcd(0x27, 20, 4);

uint credit = 0;
uint credit_new = 0;

int btn_pressed = -1;

void setup() {
  DEBUGBEGIN(115200);

  for (int i = 0; i < DISPENSER_N + 1; i++) {
    pinMode(btnArr[i], INPUT);
    pinMode(ledArr[i], OUTPUT);
  }
  // setup pin for bottle dispenser motor
  pinMode(BOTTLE_PIN, OUTPUT);
  // setup pin for bottle ultrasonic sensor
  pinMode(BOTTLE_TRIG, OUTPUT);
  pinMode(BOTTLE_ECHO, INPUT);

  lcd.begin();
  lcd.backlight();
  lcd.start_system(VERSION_FIRMWARE);
  delay(START_SCREEN_TIMEOUT);

  DEBUG("Setup done!\n");
}

void loop() {
  // turn on all button's leds
  for (int i = 0; i < DISPENSER_N + 1; i++) {
    // light up only available pump's led
    if (statusArr[i] == 0) {
      digitalWrite(ledArr[i], HIGH);
    } else {
      digitalWrite(ledArr[i], LOW);
    }
  }

  // enable coin acceptor
  coin.startInterrupt();

  lcd.clear();

  // check for button pression
  while (btn_pressed == -1) {
    // update lcd with current credit
    credit_new = coin.counter() * 10;

    lcd.main_screen(0, credit + credit_new);

    // check all pins
    for (int i = 0; i < DISPENSER_N + 1; i++) {
      if (digitalRead(btnArr[i])) {
        DEBUG("Button " + (String)(i + 1) + " pressed\n");
        btn_pressed = i;
        break;
      }
    }
  }  // while loop

  // Button was pressed!

  // add inserted coins to the credit
  credit += credit_new;
  // disable coin acceptor
  coin.stopInterrupt();

  // print current credit
  DEBUG("Credit: " + (String)credit + "euro\n");

  // turn off all button's leds
  for (int i = 0; i < DISPENSER_N + 1; i++) {
    digitalWrite(ledArr[i], LOW);
  }
  // light up pressed button
  digitalWrite(ledArr[btn_pressed], HIGH);

  // BOTTLE DISPENSING
  if (btn_pressed == DISPENSER_N) {
    DEBUG("Bottle pin pressed\n");

    // error check
    switch (bottErr) {
      case 0:  // no error
        break;

      case -1:  // no more bottles
        // display unavailable product screen and return to main screen
        lcd.clear();
        lcd.unavailableProduct_screen();
        delay(ERR_SCREEN_TIMEOUT);
        btn_pressed = -1;
        return;
        break;
    }

    // credit ok
    if (credit >= BOTTLEPRICE) {
      // check bottle sensor
      int distance = SRF05_measureDistance(BOTTLE_TRIG, BOTTLE_ECHO);
      DEBUG("Distance: " + (String)distance + "\n");

      // distance under minimum = there is a bottle to remove
      if (distance <= BOTTLE_DISTANCE) {
        DEBUG("Remove bottle\n");

        lcd.clear();
        lcd.bottleRemove_screen();

        bool bottle_removed = false;

        // check every seconds until timeout if the bottle is removed
        for (int i = 0; i <= BOTTLE_TIMEOUT; i++) {
          if (SRF05_measureDistance(BOTTLE_TRIG, BOTTLE_ECHO) >
              BOTTLE_DISTANCE) {
            // bottle removed
            bottle_removed = true;
            break;
          }
          delay(1000);
        }

        // bottle not removed after timeout
        if (!bottle_removed) {
          // reset button pressed variable
          btn_pressed = -1;
          // return to the main loop
          return;
        }
      }

      // dispense bottle
      DEBUG("Credit ok!\n");
      DEBUG("No bottle to remove\n");
      lcd.clear();
      lcd.dispense_screen("BOTTIGLIA");
      // subtract bottle price from credit
      credit -= BOTTLEPRICE;
      delay(2000);

      DEBUG("Dispensing bottle...\n");

      // turn on bottle dispenser motor
      digitalWrite(BOTTLE_PIN, HIGH);
      // wait for the endstop to change state two times (=> bottle dispensed)
      while (digitalRead(BOTTLE_ENDSTOP))
        ;
      while (!digitalRead(BOTTLE_ENDSTOP))
        ;

      // turn off bottle dispenser motor
      digitalWrite(BOTTLE_PIN, LOW);
      DEBUG("Bottle dispensed!\n");

      // update bottle counter
      bottCnt--;

      // check if there are no more bottles
      if (bottCnt == 0) {
        bottErr = -1;
      }

    }
    // low credit for bottle
    else {
      DEBUG("Credit low\n");
      lcd.clear();
      lcd.creditLow_screen(credit, BOTTLEPRICE);
      delay(LOWCREDIT_SCREEN_TIMEOUT);
    }
  }

  // DETERGENT DISPENSING
  else {
    DEBUG("Dispense pin pressed\n");

    switch (statusArr[btn_pressed]) {
      case OK:  // no error
        break;
      case PUMP_AND_EMPTY:  // -2 and -1 errors at the same time
                            // same as error -2, but when the -2 error is
                            // solved the error code change to -1 (until
                            // detergent is filled)
      case PUMP_ERR:        // error with pump or flow meter
        // display error screen and return to main screen
        lcd.clear();
        lcd.dispenserErr_screen();
        delay(ERR_SCREEN_TIMEOUT);
        btn_pressed = -1;
        return;
        break;

      case TANK_EMPTY:  // no more detergent
        // display unavailable product screen and return to main screen
        lcd.clear();
        lcd.unavailableProduct_screen();
        delay(ERR_SCREEN_TIMEOUT);
        btn_pressed = -1;
        return;
        break;

      case QTY_LOW:  // quantity to dispense > available quantity
        // this case must not appear ever, because we dispense only one liter
        // at time and the max quantity is integer number of liters
        // TODO handle this case just in case
        break;
    }

    // get selected detergent's name and price
    uint curr_detPrice = dispArr[btn_pressed]->getPrice();
    String curr_detName = dispArr[btn_pressed]->getDetName();

    // credit ok
    if (credit >= curr_detPrice) {
      DEBUG("Credit ok!\n");

      // warn user for bottle position and button press
      // endtime = now + 10 seconds
      unsigned long endTime = millis() + DET_CONFIRM_TIMEOUT;

      bool btn_pressedTwice = false;
      // add small delay for the user to release the button
      // TODO better display another screen, like this the main screen is still
      // for the amount of the delay
      lcd.clear();
      lcd.bottlePosition_screen();
      delay(800);

      // wait for btn pression
      while ((long)(millis() - endTime) <= 0) {
        // TODO change the screen or wait longer, the text doesn't show all up

        if (digitalRead(btnArr[btn_pressed])) {
          btn_pressedTwice = true;
          DEBUG("Button pressed twice!");
          break;
        }
      }

      // if btn not pressed reset btn_pressed and return to main
      if (!btn_pressedTwice) {
        DEBUG("Button not pressed twice");
        btn_pressed = -1;
        return;
      }

      credit -= curr_detPrice;

      lcd.dispense_screen(curr_detName);

      DEBUG("Dispense detergent...\n");

      // TODO insert small delay before dispensing????

      // TODO maybe flash led during dispensing
      // use ticker library https://github.com/sstaub/Ticker
      // return 0 if all goes ok, negative number otherwise
      int dispRet = dispArr[btn_pressed]->dispense(LT);

      // update status array
      statusArr[btn_pressed] = dispRet;

      DEBUG("Dispense returned: " + (String)dispRet + " (0 = no err)");

      // error during dispensing, display a message and return to main
      if (dispRet != OK || dispRet != TANK_EMPTY) {
        lcd.clear();
        lcd.dispensingErr_screen();
        delay(ERR_SCREEN_TIMEOUT);
        btn_pressed = -1;
        return;
      }

      // dispensing end
      DEBUG("Detergent dispensed!\n");
      lcd.clear();
      lcd.dispenseEnd_screen();
      delay(SCREEN_TIMEOUT);

    }
    // credit low for selected detergent
    else {
      DEBUG("Credit low: " + (String)credit + "euro\n");
      lcd.clear();
      lcd.creditLow_screen(credit, curr_detPrice);
      delay(LOWCREDIT_SCREEN_TIMEOUT);
    }
  }

  // TODO give remainder back

  // reset button variable
  btn_pressed = -1;
}

/**
 * @brief Measure distance with SFR05 ultrasonic sensor
 *
 * @param trigPin trigger pin
 * @param echoPin echo pin
 * @return int distance in cm
 */
int SRF05_measureDistance(uint8_t trigPin, uint8_t echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  const unsigned long duration = pulseIn(echoPin, HIGH);
  int distance = duration / 29.0 / 2.0;

  return distance;
}