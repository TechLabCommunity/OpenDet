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

int SRF05_measureDistance(uint8_t, uint8_t);

// ATTENZIONE: nome prodotto deve avere meno di 20 caratteri
Dispenser disp_1(BTN_PIN_1, FLOW_PIN_1, PUMP_PIN_1, PULSESXLITER_1, "Test_1",
                 DETPRICE_1);
Dispenser disp_2(BTN_PIN_2, FLOW_PIN_2, PUMP_PIN_2, PULSESXLITER_2, "Test_2",
                 DETPRICE_2);
Dispenser disp_3(BTN_PIN_3, FLOW_PIN_3, PUMP_PIN_3, PULSESXLITER_3, "Test_3",
                 DETPRICE_3);
Dispenser disp_4(BTN_PIN_4, FLOW_PIN_4, PUMP_PIN_4, PULSESXLITER_4, "Test_4",
                 DETPRICE_4);
Dispenser disp_5(BTN_PIN_5, FLOW_PIN_5, PUMP_PIN_5, PULSESXLITER_5, "Test_5",
                 DETPRICE_5);
Dispenser disp_6(BTN_PIN_6, FLOW_PIN_6, PUMP_PIN_6, PULSESXLITER_6, "Test_6",
                 DETPRICE_6);

Dispenser* dispArr[DISPENSER_N] = {&disp_1, &disp_2, &disp_3,
                                   &disp_4, &disp_5, &disp_6};
int btnArr[DISPENSER_N + 1] = {BTN_PIN_1, BTN_PIN_2, BTN_PIN_3,     BTN_PIN_4,
                               BTN_PIN_5, BTN_PIN_6, BTN_PIN_BOTTLE};

int ledArr[DISPENSER_N + 1] = {BTN_LED_1, BTN_LED_2, BTN_LED_3,     BTN_LED_4,
                               BTN_LED_5, BTN_LED_6, BTN_LED_BOTTLE};

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

  delay(500);

  DEBUG("Setup done!\n");
}

void loop() {
  for (int i = 0; i < DISPENSER_N + 1; i++) {
    digitalWrite(ledArr[i], HIGH);
  }

  // enable coin acceptor
  coin.startInterrupt();

  lcd.clear();

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

  DEBUG("Credit: " + (String)credit + "euro\n");

  // turn off all button's
  for (int i = 0; i < DISPENSER_N + 1; i++) {
    digitalWrite(btnArr[i], LOW);
  }
  // light up pressed button
  digitalWrite(ledArr[btn_pressed], HIGH);

  // bottle pin pressed
  if (btn_pressed == DISPENSER_N) {
    DEBUG("Bottle pin pressed\n");

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

    }
    // low credit for bottle
    else {
      DEBUG("Credit low\n");
      lcd.creditLow_screen(credit, BOTTLEPRICE);
    }
  }
  // detergent pin pressed
  else {
    DEBUG("Dispense pin pressed\n");

    // get selected detergent's name and price
    uint curr_detPrice = dispArr[btn_pressed]->getPrice();
    String curr_detName = dispArr[btn_pressed]->getDetName();

    // credit ok
    if (credit >= curr_detPrice) {
      DEBUG("Credit ok!\n");

      credit -= curr_detPrice;

      lcd.dispense_screen(curr_detName);

      DEBUG("Dispense detergent...\n");

      // TODO maybe flash led during dispensing
      // use ticker library https://github.com/sstaub/Ticker
      dispArr[btn_pressed]->dispense(LT);

      // dispensing end
      DEBUG("Detergent dispensed!\n");
      lcd.clear();
      lcd.dispenseEnd_screen();
      delay(2000);

    }
    // credit low for selected detergent
    else {
      DEBUG("Credit low: " + (String)credit + "euro\n");

      // credit 0, display det price only
      if (credit == 0) {
        DEBUG("Credit 0\n");
        lcd.clear();
        lcd.smartprint(curr_detName, 1);
        lcd.smartprint("Prezzo: " + (String)curr_detPrice, 2);
        delay(5000);
      }
      // credit > 0, display a "credit low" message
      else {
        DEBUG("Credit > 0\n");
        lcd.clear();
        lcd.creditLow_screen(credit, curr_detPrice);
        delay(5000);
      }
    }
  }
  // reset button variable
  btn_pressed = -1;
}

/**
 * @brief Measure distance with SFR=% ultrasonic sensor
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