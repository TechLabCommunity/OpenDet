#include <CH926.h>
#include <DetScreen.h>
#include <Dispenser.h>
#include <Global.h>
#include <config.h>

DetScreen lcd(0x27, 20, 4);

CH926 coin(COIN_SIGPIN, COIN_PWRPIN, NO, ACTIVE_HIGH);
uint32_t CH926::_counter = 0;
uint credit = 0;
uint credit_new = 0;

void setup() {
  DEBUGBEGIN(115200);

  lcd.begin();
  lcd.backlight();
  lcd.start_system(VERSION_FIRMWARE);
  lcd.clear();

  DEBUG("Att\n");
  coin.startInterrupt();
  delay(1000);

  DEBUG("Det\n");
  coin.stopInterrupt();
  delay(1000);
  DEBUG("Counter: " + (String)coin.counter());

  DEBUG("Att\n");
  coin.startInterrupt();

  DEBUG("Counter: " + (String)coin.counter());
}

void loop() {
  // enable coin acceptor

  credit_new = coin.counter() * 10;

  lcd.main_screen(0, credit + credit_new);
}