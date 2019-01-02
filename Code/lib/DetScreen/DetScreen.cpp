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
}

void DetScreen::slideprint(const String& line, uint row) {
  String textSlide1toPrint = line.substring(0, cols);
  if (millis() - timer >= 300) {
    timer = millis();
    slider++;
    textSlide1toPrint = line.substring(slider, slider + cols);
    if (slider + cols <= line.length())
      smartprint(textSlide1toPrint, row);
    else {
      slider = 0;
    }
  }
}

// TODO not working for multi line print
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
    slideprint("   Inserire monete o selezionare prodotto", 1);
  }
}

void DetScreen::bottlePosition_screen() {
  // warning message for bottle positioning
  smartprint("POSIZIONARE", 0);
  smartprint("BOTTIGLIA", 1);
  smartprint("Premere  per", 2);
  smartprint("iniziare", 3);
}

void DetScreen::dispense_screen(const String& detName) {
  // TODO if possible insert a progress bar

  // info message for dispensing
  smartprint("EROGAZIONE", 0);
  smartprint(detName, 1);
  blinkprint("ATTENDERE  PREGO", 2);
}

void DetScreen::dispenseEnd_screen() { smartprint("FINE EROGAZIONE", 1); }

void DetScreen::creditLow_screen(uint credit, uint detPrice) {
  /*
  smartprint("Credito", 0);
  smartprint("Insufficiente", 1);
  smartprint("Prezzo: " + (String)detPrice, 2);
  smartprint("Credito: " + (String)credit, 3);
  */

  // TODO remove this shit ;)
  smartprint("Credito", 0);
  smartprint("Insufficiente", 1);
}

void DetScreen::bottleRemove_screen() {
  smartprint("RIMUOVERE", 1);
  smartprint("BOTTIGLIA", 2);
}

void DetScreen::unavailableProduct_screen() {
  smartprint("PRODOTTO", 1);
  smartprint("ESAURITO", 2);
}

void DetScreen::dispensingErr_screen() {
  smartprint("ERRORE DURANTE", 0);
  smartprint("L'EROGAZIONE", 1);
}

void DetScreen::dispenserErr_screen() {
  smartprint("EROGATORE", 1);
  smartprint("NON FUNZIONANTE", 2);
}

void DetScreen::maintMode_screen() {
  smartprint("MODALITA", 1);
  smartprint("MANUTENZIONE", 2);
}

void DetScreen::refill_screen() {
  smartprint("PREMERE NUOVAMENTE", 1);
  smartprint("PER RICARICARE", 2);
}

void DetScreen::refillEnd_screen() {
  smartprint("RICARICA  EFFETTUATA", 1);
  smartprint("CORRETTAMENTE", 2);
}

void DetScreen::maintInstr_screen() {
  smartprint("Premere un pulsante", 1);
  smartprint("per ricaricare", 2);
}

void DetScreen::calibr_screen() {
  smartprint("CALIBRAZIONE", 1);
  smartprint("POMPE", 2);
}

void DetScreen::calibrInfo_screen() {
  smartprint("Premere il pulsante", 0);
  smartprint("fino ad erogare", 1);
  smartprint("1 litro esatto", 2);
}

void DetScreen::calibrEnd_screen(uint index) {
  smartprint("POMPA " + (String)index, 1);
  smartprint("CALIBRATA", 2);
}

void DetScreen::refillErr_screen() {
  smartprint("DISPENSER GUASTO", 0);
  smartprint("Correggere l'errore", 2);
  smartprint("prima di ricaricare", 3);
}

void DetScreen::pumpNoErr_screen() {
  smartprint("POMPA", 1);
  smartprint("FUNZIONANTE", 2);
}

void DetScreen::pumpFixed_screen() {
  smartprint("POMPA", 1);
  smartprint("RIPARATA", 2);
}

void DetScreen::calibrModeEnd_screen() {
  smartprint("RITORNO ALLA", 0);
  smartprint("MODALITA", 1);
  smartprint("MANUTENZIONE", 2);
}