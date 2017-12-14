#define FRQ_PIN 2
//Counter max value, if counter reach this value it will be reset
#define COUNTER_MAX 200

//comment this line to disable debug
//#define DEBUG1

int frqPinState = LOW;
int prevFrqPinState = LOW;
int frqCounter = 0;


void setup()
{
//#ifdef DEBUG1
  Serial.begin(9600);
//#endif
  pinMode(FRQ_PIN, INPUT);

  Serial.println(millis());
}

void loop()
{
  frqPinState = digitalRead(FRQ_PIN);

  if (frqPinState == LOW) {
    prevFrqPinState = LOW;
  }

  //pin change from LOW to HIGH
  if (frqPinState == HIGH && prevFrqPinState == LOW)
  {
    prevFrqPinState = frqPinState;
    frqCounter++;

#ifdef DEBUG1
    Serial.println(frqCounter);
#endif
  }

  if (frqCounter == COUNTER_MAX) {
    Serial.println(millis());
    frqCounter = 0;
    /*
       CODE TO EXECUTE WHEN COUNTER REACH ITS MAX
    */
#ifdef DEBUG1
    Serial.println("STOP");
#endif
  }
}
