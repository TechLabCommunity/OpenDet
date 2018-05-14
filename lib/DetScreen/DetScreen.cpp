#include "DetScreen.h"

DetScreen::DetScreen(uint paddress, uint pcols, uint prows)
    : LiquidCrystal_I2C(paddress, pcols, prows) {
  if (pcols == 0) SYSERR("DetScreen must have 1 column at least.");
  if (prows == 0) SYSERR("DetScreen must have 1 row at least.");
  address = paddress;
  cols = pcols;
  rows = prows;
}

void DetScreen::clear() {
  timer = 0;
  slider = 0;
  LiquidCrystal_I2C::clear();
}

String DetScreen::cents_to_string(const uint& cents) {
  String cc = String(cents);
  if (cents < 100) {
    for (uint i = 0; i < 4 - cc.length(); i++) cc = "0" + cc;
  }
  return cc.substring(0, cc.length() - 2) + "." +
         cc.substring(cc.length() - 2, cc.length());
}

unsigned int inline DetScreen::calcol(const String& s) {
  return constrain((cols - s.length()) / 2, 0, cols - 1);
}

// TODO if string leght > columns bad thing happen
void DetScreen::smartprint(const String& s, uint row) {
  setCursor(calcol(s), row);
  print(s);
}

void DetScreen::start_system(const String& version) {
  String ver = version;
  ver.trim();
  smartprint("OpenDet " + ver, 0);
  delay(1000);
  smartprint("TechLab  Community", 1);
  delay(1000);
  smartprint("Starting  system", 3);

  delay(2000);
}

void DetScreen::slideprint(const String& line, uint row) {
  String textSlide1toPrint = line.substring(0, cols);
  if (millis() - timer >= 800) {
    timer = millis();
    slider++;
    textSlide1toPrint = line.substring(slider, slider + cols);
    if (slider + cols <= line.length())
      smartprint(textSlide1toPrint, 1);
    else {
      slider = 0;
    }
  }
}

void DetScreen::blinkprint(const String& line, uint row) {
  if (millis() - timer >= 800) {
    timer = millis();
    slider = !slider;
    if (slider)
      smartprint(line, row);
    else {
      // TODO why not use lcd.clear()?
      setCursor(0, row);
      for (uint i = 0; i < cols; i++) {
        print(" ");
      }
    }
  }
}

void DetScreen::main_screen(uint status, uint coin) {
  switch (status) {
    case 0:
      smartprint("Sistema Pronto", 0);
      break;
    case 1:
      smartprint("ERROR", 0);
      break;
  }
  setCursor(0, 3);
  if (coin > 0) {
    smartprint("SELEZIONARE PRODOTTO", 1);
    print("Credito : ");
    // TODO add eur symbol if possible
    String coins_string = cents_to_string(coin);
    setCursor(cols - coins_string.length(), 3);
    print(coins_string);
  } else {
    slideprint("  Inserire monete o selezionare prodotto", 1);
    for (uint i = 0; i < cols; i++) print(" ");
  }
}

void DetScreen::dispense_screen(const String& detName) {
  smartprint("EROGAZIONE", 0);
  smartprint(detName, 1);
  blinkprint("ATTENDERE  PREGO", 2);
}

void DetScreen::dispenseEnd_screen() { smartprint("FINE EROGAZIONE", 1); }
void DetScreen::creditLow_screen(uint credit, uint detPrice) {
  smartprint("Credito", 0);
  smartprint("Insufficiente", 1);
  smartprint("Prezzo: " + (String)detPrice, 2);
  smartprint("Credito: " + (String)credit, 3);
}
