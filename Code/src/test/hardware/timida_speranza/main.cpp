#include <Global.h>

#define POMPA   13 //D11 Controllino
#define POMPA2  10
#define BUTTON  12 //D12 Controllino
volatile bool fine_erogazione = false;
bool was_pressed = false;

int fluxWorking = 2;
int fluxState = LOW;
int fluxStatePrev = LOW;
int fluxCounter = 0;
int maxCounter = 200;

void setup()
{
#ifdef DEBUG1 //se è definito DEBUG1 allora fai istruzioni fino ad #endif
  Serial.begin(9600);
#endif

  pinMode(POMPA, OUTPUT);
  pinMode(POMPA2, OUTPUT);
  pinMode(BUTTON, INPUT);
  digitalWrite(POMPA2, LOW);

  //Imposto la funzione che venga chiamata ogni 5 ms (200Hz)
  MsTimer2::set(5, pulseCounter);

}

void loop()
{
  //Se A0 ha in ingresso stato alto, la pompa viene aperta.
  if (digitalRead(BUTTON) && !was_pressed) {
    digitalWrite(POMPA, HIGH);
    Serial.println(millis());
    MsTimer2::start();
    was_pressed = true;
#ifdef DEBUG1
    Serial.println("First if");
#endif
  }

  //Finita l'erogazione spengo pompa e fermo il timer
  if (fine_erogazione) {
    digitalWrite(POMPA, LOW);
    MsTimer2::stop();
    was_pressed = false;
    fine_erogazione = false;
#ifdef DEBUG1
    Serial.println("Second if");
#endif
  }
}

void pulseCounter(){
  fluxState = digitalRead(fluxWorking);

  if (fluxState == LOW) {
    fluxStatePrev = LOW;
  }

  //pin change from LOW to HIGH
  if (fluxState == HIGH && fluxStatePrev == LOW)
  {
    fluxStatePrev = fluxState;
    fluxCounter++;

#ifdef DEBUG1
    Serial.println(fluxCounter);
#endif
  }

  if (fluxCounter == maxCounter) {
    //code to execute when counter has reachet its maximum
    Serial.println(millis());
    fluxCounter = 0;
    fine_erogazione = true;
#ifdef DEBUG1
    Serial.println("Loading Complete");
#endif
  }
}
