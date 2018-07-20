#include <DetScreen.h>
#include <Dispenser.h>
#include <Global.h>

long xxxx[0];
// first byte = address, last byte = checksum
// cctalk message : destination address | number of data bytes | source address
// | header (function number) | checksum
byte simplePool[] = {3, 0, 1, 254, 254};

byte serialNumber[] = {3, 0, 1, 242, 10};

byte payment[] = {3, 4, 1, 167, 140, 136, 15, 2, 44};

byte enable[] = {3, 1, 1, 164, 0xA5, 178};

int count = 0;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial.println("Setup");
}
void loop() {
  byte c;

  while (count < 6) {
    Serial2.write(enable[count]);
    count++;
  }

  while (Serial2.available()) {
    c = Serial2.read();
    Serial.print(c);
  }
}