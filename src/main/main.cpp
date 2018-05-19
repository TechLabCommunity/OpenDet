#include <CH926.h>
#include <DetScreen.h>
#include <Dispenser.h>
#include <Global.h>
#include "config.h"

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

CH926 coin(COIN_SIGPIN, COIN_PWRPIN, NO, ACTIVE_HIGH);

DetScreen lcd(0x27, 20, 4);

uint credit = 200;

uint btn_pressed = 0;

void setup() {
  DEBUGBEGIN(115200);
  lcd.begin();
  lcd.backlight();
  lcd.start_system(VERSION_FIRMWARE);
  lcd.clear();

  delay(500);

  DEBUG("Setup!\n");
}

void loop() {
  digitalWrite(BTN_LED_1, HIGH);
  digitalWrite(BTN_LED_2, HIGH);
  digitalWrite(BTN_LED_3, HIGH);
  digitalWrite(BTN_LED_4, HIGH);
  digitalWrite(BTN_LED_5, HIGH);
  digitalWrite(BTN_LED_6, HIGH);

  lcd.clear();
  lcd.main_screen(0, credit);

  while (btn_pressed == 0) {
    DEBUG("While cycle\n");
    coin.start();  // enable coin acceptor

    // check all pins. Use adiacent pin to use a for loop
    if (digitalRead(BTN_PIN_1)) {
      btn_pressed = BTN_PIN_1;
      DEBUG("Button 1 pressed");
    } else if (digitalRead(BTN_PIN_2)) {
      btn_pressed = BTN_PIN_2;
      DEBUG("Button 2 pressed");
    } else if (digitalRead(BTN_PIN_3)) {
      btn_pressed = BTN_PIN_3;
      DEBUG("Button 3 pressed");
    } else if (digitalRead(BTN_PIN_4)) {
      btn_pressed = BTN_PIN_4;
      DEBUG("Button 4 pressed");
    } else if (digitalRead(BTN_PIN_5)) {
      btn_pressed = BTN_PIN_5;
      DEBUG("Button 5 pressed");
    } else if (digitalRead(BTN_PIN_6)) {
      btn_pressed = BTN_PIN_6;
      DEBUG("Button 6 pressed");
    }

    // control for coin insertion
    if (!digitalRead(COIN_SIGPIN)) {
      // if no signal after 400 ms stop counting
      int pulses = coin.countPulses(400);
      // pulses counted with no errors
      if (pulses != -1) {
        // 1 pulse = 5 cent
        credit += pulses * 5;
        DEBUG("Credit: " + (String)credit + "\n");
      }
      // error in countPulses()
      else {
        DEBUG("Coin signal error\n");
      }
    }  // end coin insertion
  }    // button pressed

  // TODO disable coin acceptor here
  coin.stop();

  digitalWrite(BTN_LED_1, LOW);
  digitalWrite(BTN_LED_2, LOW);
  digitalWrite(BTN_LED_3, LOW);
  digitalWrite(BTN_LED_4, LOW);
  digitalWrite(BTN_LED_5, LOW);
  digitalWrite(BTN_LED_6, LOW);

  // TODO do same thing for all dispenser
  int curr_detPrice = 0;
  String curr_detName = "";

  //Tramite questo metodo di classe è possibile ottenere un dispenser dando il pulsante premuto
  Dispenser& dispChosen = Dispenser::getDispFromButton(btn_pressed);
  curr_detPrice = dispChosen.getPrice();
  curr_detName = dispChosen.getDetName();
  //Da migliorare le condizioni e usare funzioni.
  if (credit >= dispChosen.getPrice()) {
    DEBUG("Dispensing det "+String(dispChosen.getNumberId()));
    credit -= dispChosen.getPrice();
    digitalWrite(dispChosen.getButton(), HIGH);
    lcd.clear();
    lcd.dispense_screen("");
    dispChosen.dispense(1000);
    // TODO display a SUCCESS message
    btn_pressed = 0;
  }
  DEBUG("Finish dispensing\n");
  DEBUG("Credit: " + (String)credit + "\n");
  if (btn_pressed == 0) {
    // display FINE EROGAZIONE e RIMUOVERE PRODOTTO
  } else if (credit > 0) {
    lcd.clear();
    lcd.smartprint("Credito", 0);
    lcd.smartprint("Insufficiente", 1);

    String priceStr = "Prezzo: ";
    // mettere prezzo a seconda del prodotto selezionato
    priceStr += (String)curr_detPrice;
    lcd.smartprint(priceStr, 3);

    DEBUG("Insufficient Credit\n");
    DEBUG("Credit: " + (String)credit + "\n");

    delay(6000);
  } else {
    lcd.clear();
    lcd.smartprint("Prodotto selezionato", 0);
    lcd.smartprint(curr_detName, 1);

    String priceStr = "Prezzo: ";
    // mettere prezzo a seconda del prodotto selezionato
    priceStr += (String)curr_detPrice;
    lcd.smartprint(priceStr, 3);

    DEBUG("Credito nullo\n");

    delay(6000);
  }

  // reset variable
  btn_pressed = 0;
}