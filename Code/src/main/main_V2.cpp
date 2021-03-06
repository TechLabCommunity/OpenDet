#include <CH926.h>
#include <DetScreen.h>
#include <Dispenser.h>
#include <Global.h>
#include <SD.h>
#include <SPI.h>
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
void softwareReset();
String Controllino_timestamp();

struct Config {
  uint prices[DISPENSER_N + 1];
  uint timeouts[6];
  String detNames[DISPENSER_N];
};

// ATTENZIONE: nome prodotto deve avere meno di 20 caratteri
Dispenser disp_1(BTN_PIN_1, FLOW_PIN_1, PUMP_PIN_1, PULSESXLITER_1, DETNAME_1,
                 DETPRICE_1, DET_CNTMAX,
                 static_cast<unsigned int>(10000.0 / PULSESXLITER_1 *
                                           TIMEXLITER_1));
Dispenser disp_2(BTN_PIN_2, FLOW_PIN_2, PUMP_PIN_2, PULSESXLITER_2, DETNAME_2,
                 DETPRICE_2, DET_CNTMAX,
                 static_cast<unsigned int>(10000.0 / PULSESXLITER_2 *
                                           TIMEXLITER_2));
Dispenser disp_3(BTN_PIN_3, FLOW_PIN_3, PUMP_PIN_3, PULSESXLITER_3, DETNAME_3,
                 DETPRICE_3, DET_CNTMAX,
                 static_cast<unsigned int>(10000.0 / PULSESXLITER_3 *
                                           TIMEXLITER_3));
Dispenser disp_4(BTN_PIN_4, FLOW_PIN_4, PUMP_PIN_4, PULSESXLITER_4, DETNAME_4,
                 DETPRICE_4, DET_CNTMAX,
                 10000.0 / PULSESXLITER_4 * TIMEXLITER_4);
Dispenser disp_5(BTN_PIN_5, FLOW_PIN_5, PUMP_PIN_5, PULSESXLITER_5, DETNAME_5,
                 DETPRICE_5, DET_CNTMAX,
                 10000.0 / PULSESXLITER_5 * TIMEXLITER_5);
Dispenser disp_6(BTN_PIN_6, FLOW_PIN_6, PUMP_PIN_6, PULSESXLITER_6, DETNAME_6,
                 DETPRICE_6, DET_CNTMAX,
                 10000.0 / PULSESXLITER_6 * TIMEXLITER_6);

// array for dispenser object
Dispenser* dispArr[DISPENSER_N] = {&disp_1, &disp_2, &disp_3,
                                   &disp_4, &disp_5, &disp_6};
// array for buttons pins
uint8_t btnArr[DISPENSER_N + 1] = {BTN_PIN_1,     BTN_PIN_2, BTN_PIN_3,
                                   BTN_PIN_4,     BTN_PIN_5, BTN_PIN_6,
                                   BTN_PIN_BOTTLE};
// array for leds pins
uint8_t ledArr[DISPENSER_N + 1] = {BTN_LED_1,     BTN_LED_2, BTN_LED_3,
                                   BTN_LED_4,     BTN_LED_5, BTN_LED_6,
                                   BTN_LED_BOTTLE};

// array for status: 0=OK, -1=EMPTY, -3=ERROR, -4=EMPTY+ERROR
uint8_t statusArr[DISPENSER_N + 1] = {0};

uint8_t bottStatus = OK;
uint8_t bottCnt = BOTTLE_CNTMAX;

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

  // setup pin for key selector
  pinMode(MAINT_PIN, INPUT);

  pinMode(RTC_SET_PIN, INPUT);

  // RTC initialization
  Controllino_RTC_init(0);
  // day, day of week, month, year, hour, minute, second
  // TODO think for a better way to setup the RTC
  if (digitalRead(RTC_SET_PIN)) {
    Controllino_SetTimeDate(31, 2, 7, 18, 19, 17, 00);
  }

  lcd.begin();
  lcd.backlight();
  lcd.start_system(VERSION_FIRMWARE);
  delay(START_SCREEN_TIMEOUT);

  // MAINTENANCE MODE

  if (digitalRead(MAINT_PIN)) {
    DEBUG("Maintenance mode");
    // TODO
    // - check all buttons
    // - unavailable pump need long pression to recover the pump error, and
    // then their tank can be refilled
    // - single pression to refill detergent tanks or bottle, reset the
    // counter
    // - long pression for bottle button -> enter calibration mode
    // - press bottle button again to exit calibration mode
    // - while in calibration mode keep pump's button to dispense exactly 1
    // liter. Then save flow meter pulses and time for dispensing (needed to
    // calculate timeouts for error checking during dispensing)

    // display maintenance mode screen
    lcd.clear();
    lcd.maintMode_screen();
    delay(SCREEN_TIMEOUT);

    while (true) {
      // display maintenance mode screen
      lcd.clear();
      lcd.maintInstr_screen();

      // turn on all button's leds
      for (int i = 0; i < DISPENSER_N + 1; i++) {
        // light up only available pump's led
        if (statusArr[i] == 0) {
          digitalWrite(ledArr[i], HIGH);
        } else {
          digitalWrite(ledArr[i], LOW);
        }
      }

      // reset button variable
      btn_pressed = -1;
      // TODO compiler give "unused but set variable" warning, but this variable
      // is used!
      bool longPress = false;

      // check for button pression
      while (btn_pressed == -1) {
        // check all buttons
        for (int i = 0; i < DISPENSER_N + 1; i++) {
          uint held = 0;

          while (digitalRead(btnArr[i]) && held < LONGPRESS_TIME) {
            held++;
            delay(100);
          }

          // DEBUG("Held: " + (String)held + "\n");

          // check for long or short press and exit while loop
          if (held == LONGPRESS_TIME) {
            DEBUG("Button " + (String)(i + 1) + " long pressed\n");
            btn_pressed = i;
            longPress = true;
            break;
          } else if (held != 0) {
            DEBUG("Button " + (String)(i + 1) + " pressed\n");
            btn_pressed = i;
            break;
          }
        }

        if (!digitalRead(MAINT_PIN)) {
          DEBUG("exiting maintenance mode");
          // reset if key selector return in normal position
          softwareReset();
        }
      }

      DEBUG("Button: " + (String)(btn_pressed + 1) + " pressed\n");

      // BUTTON PRESSED
      // turn off all button's leds
      for (unsigned char i : ledArr) {
        digitalWrite(i, LOW);
      }
      // light up pressed button
      digitalWrite(ledArr[btn_pressed], HIGH);

      // display confirmation screen for refilling
      lcd.clear();

      // different actions for long or short press
      if (longPress) {
        DEBUG("long press\n");
        // BOTTLE BTN: enter CALIBRATION MODE
        if (btn_pressed == DISPENSER_N) {
          // display calibration mode screen
          lcd.calibr_screen();
          // screen timeout also give time for button release
          delay(SCREEN_TIMEOUT);

          // loop until bottle btn pression or key selector return to normal
          // mode
          while (!digitalRead(btnArr[DISPENSER_N]) && digitalRead(MAINT_PIN)) {
            // display calibration instruction screen
            lcd.calibrInfo_screen();
            // check all dispenser's buttons
            for (uint8_t i = 0; i < DISPENSER_N; i++) {
              // calibrate pump only if button is high
              uint ret = dispArr[i]->calibrate(btnArr[i], HIGH);
              // calibration done correctly
              if (ret != 0) {
                DEBUG("New pulses: " + (String)dispArr[i]->getPulses() + "\n");
                // display calibration success screen
                lcd.clear();
                lcd.calibrEnd_screen(i + 1);
                delay(SCREEN_TIMEOUT);
              }
            }
          }
          // display screen before returning to refill mode
          lcd.clear();
          lcd.calibrModeEnd_screen();
          delay(SCREEN_TIMEOUT);
        } else {
          // PUMP WITH ERROR: reset error
          if (statusArr[btn_pressed] != OK &&
              statusArr[btn_pressed] != TANK_EMPTY) {
            // pumpErr_reset() correct only pump error, not EMPTY error
            statusArr[btn_pressed] = dispArr[btn_pressed]->pumpErr_reset();
            DEBUG("Status: " + (String)statusArr[btn_pressed] + "\n");
            // display screen
            lcd.pumpFixed_screen();
            delay(SCREEN_TIMEOUT);
          } else {
            // NORMAL PUMP: no error, only display screen
            lcd.pumpNoErr_screen();
            delay(SCREEN_TIMEOUT);
          }
        }  // long press on dispenser button
      } else {
        DEBUG("short press\n");

        // selected dispenser has some error
        if (statusArr[btn_pressed] != OK &&
            statusArr[btn_pressed] != TANK_EMPTY) {
          // display error screen and return to while
          lcd.refillErr_screen();
          delay(SCREEN_TIMEOUT);
        } else {
          // display screen for refilling tanks
          lcd.refill_screen();

          // DOUBLE PRESSION CHECK

          bool btn_pressedTwice = false;

          // TODO maybe add instead a while(!digitalRead(button)); to wait for
          // button to return to normal position (watch out for debounce
          // problems)

          // add small delay to avoid double check on single button pression
          delay(800);

          // warn user for bottle position and button press
          // endtime = now + timeout
          unsigned long endTime = millis() + BTN_CONFIRM_TIMEOUT;

          // wait for second btn pression
          while ((long)(millis() - endTime) <= 0) {
            if (digitalRead(btnArr[btn_pressed])) {
              btn_pressedTwice = true;
              DEBUG("Button pressed twice!");
              break;
            }
          }

          // if btn not pressed reset return to top of while loop
          if (btn_pressedTwice) {
            DEBUG("Button pressed twice");

            // fill bottle or detergent
            if (btn_pressed == DISPENSER_N) {
              bottCnt = BOTTLE_CNTMAX;
              // reset bottle status
              bottStatus = OK;
            } else {
              // fill dispenser and update its status
              statusArr[btn_pressed] = dispArr[btn_pressed]->fillTank();
              DEBUG("Status: " + (String)statusArr[btn_pressed] + "\n");
            }

            // display refill completed screen
            lcd.clear();
            lcd.refillEnd_screen();
            delay(SCREEN_TIMEOUT);
          }  // btn pressed twice
        }    // dispenser empty or ok
      }      // short btn pression
    }        // while loop
  }          // maintenance mode

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

  // reset button variable
  btn_pressed = -1;

  lcd.clear();
  // check for button pression
  while (btn_pressed == -1) {
    // update lcd with current credit
    credit_new = static_cast<unsigned int>(coin.counter() * 10);

    lcd.main_screen(0, credit + credit_new);

    // check all buttons
    for (int i = 0; i < DISPENSER_N + 1; i++) {
      if (digitalRead(btnArr[i])) {
        DEBUG("Button " + (String)(i + 1) + " pressed\n");
        btn_pressed = i;
        break;
      }
    }

    // check key selector
    if (digitalRead(MAINT_PIN)) {
      // reset all to go to maintenance mode
      softwareReset();
    }
  }  // while loop

  // BUTTON PRESSED

  // add inserted coins to the credit
  credit += credit_new;
  // disable coin acceptor
  coin.stopInterrupt();

  // print current credit
  DEBUG("Credit: " + (String)credit + "euro\n");

  // turn off all button's leds
  for (unsigned char i : ledArr) {
    digitalWrite(i, LOW);
  }
  // light up pressed button
  digitalWrite(ledArr[btn_pressed], HIGH);

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
      // btn_pressed = -1;
      return;
      break;

    case TANK_EMPTY:  // no more detergent
      // display unavailable product screen and return to main screen
      lcd.clear();
      lcd.unavailableProduct_screen();
      delay(ERR_SCREEN_TIMEOUT);
      // btn_pressed = -1;
      return;
      break;

    case QTY_LOW:  // quantity to dispense > available quantity
      // this case must not appear ever, because we dispense only one liter
      // at time and the max quantity is integer number of liters
      // TODO handle this case just in case
      break;
    default:
      break;
  }

  // BOTTLE DISPENSING
  if (btn_pressed == DISPENSER_N) {
    DEBUG("Bottle pin pressed\n");

    // credit ok
    if (credit >= BOTTLEPRICE) {
      // check bottle sensor
      int distance = SRF05_measureDistance(BOTTLE_TRIG, BOTTLE_ECHO);
      DEBUG("Distance: " + (String)distance + "\n");

      // distance under minimum = there is a bottle to remove
      if (distance <= BOTTLE_DISTANCE || distance >= BOTTLE_MAXDISTANCE) {
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
          // btn_pressed = -1;
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
        bottStatus = TANK_EMPTY;
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

    // get selected detergent's name and price
    uint curr_detPrice = dispArr[btn_pressed]->getPrice();
    String curr_detName = dispArr[btn_pressed]->getDetName();

    // credit ok
    if (credit >= curr_detPrice) {
      DEBUG("Credit ok!\n");

      bool btn_pressedTwice = false;

      // display screen for bottle positioning
      lcd.clear();
      lcd.bottlePosition_screen();

      // add small delay to avoid double check on single button pression
      delay(1500);

      // warn user for bottle position and button press
      // endtime = now + 10 seconds
      unsigned long endTime = millis() + BTN_CONFIRM_TIMEOUT;

      // wait for second btn pression
      while ((long)(millis() - endTime) <= 0) {
        if (digitalRead(btnArr[btn_pressed])) {
          btn_pressedTwice = true;
          DEBUG("Button pressed twice!");
          break;
        }
      }

      // if btn not pressed reset btn_pressed and return to main
      if (!btn_pressedTwice) {
        DEBUG("Button not pressed twice");
        // btn_pressed = -1;
        return;
      }

      credit -= curr_detPrice;

      lcd.clear();
      lcd.dispense_screen(curr_detName);

      DEBUG("Dispense detergent...\n");

      uint8_t dispRet = dispArr[btn_pressed]->dispense(LT);

      // update status array
      statusArr[btn_pressed] = dispRet;

      DEBUG("Dispense returned: " + (String)dispRet + " (0 = no err)");

      // error during dispensing, display a message and return to main
      if (dispRet != OK && dispRet != TANK_EMPTY) {
        lcd.clear();
        lcd.dispensingErr_screen();
        delay(ERR_SCREEN_TIMEOUT);
        // btn_pressed = -1;
        return;
      } else {
        // dispensing end
        DEBUG("Detergent dispensed!\n");
        lcd.clear();
        lcd.dispenseEnd_screen();
        delay(SCREEN_TIMEOUT);
      }  // dispensing ok
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
  int distance = static_cast<int>(duration / 29.0 / 2.0);

  return distance;
}

void softwareReset() {
  // start watchdog with the provided prescaler
  // possible value for prescaler are defined in wdt.h (WDTO_15MS,...)
  wdt_enable(WDTO_15MS);
  while (1)
    ;
}

String Controllino_timestamp() {
  return String(Controllino_GetYear()) + "_" + String(Controllino_GetMonth()) +
         "_" + String(Controllino_GetDay()) + ":" +
         String(Controllino_GetHour()) + "_" + String(Controllino_GetMinute());
}