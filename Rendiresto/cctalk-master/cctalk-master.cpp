#define ALTSOFTCOMM
#ifdef ALTSOFTCOMM
#include <AltSoftSerial.h>
#endif
#include "cctcom.h"
#include "headers.h"
// some garbage , without it my compiler gives a fatal error , you can skip this if the schetch compiles. 
long weqwerqr;long xxxx[1];long sdasdasd[1];long sdasdasa[1];long sdasdase[1];
///

int lastevent = 0;
int loops = 0;

void printASCIIdata(){// prints the ascii data field
    for (int i = 4 ; i<RX_buffer[1]+4;i++){
      Serial.write(RX_buffer[i]);
    }
    Serial.println();
}
void printBINdata(){// prints the bin data field
  for (int i = 4 ; i<RX_buffer[1]+4;i++){
      Serial.println(RX_buffer[i]);
    }

}

void setup() {
  Serial.begin(9600);
  ccTalk_comm_init();
  while ( RX_state != RXidle) {ccTalkReceive();}
  
    cctsend(reset_device,40,0);
  while (RX_state < RXcomplete) {ccTalkReceive();}
  if (RX_state!=RXcomplete){
    Serial.println(RX_msg[RX_state]);// Show the error if any
    while(1);// stop
  }
  delay(3000);

    cctsend(modify_master_inhibit_status,40,1,1);
  while (RX_state < RXcomplete) {ccTalkReceive();}
  if (RX_state!=RXcomplete){
    Serial.println(RX_msg[RX_state]);// Show the error if any
    setup();
    //while(1);// stop
  }
delay (100);
//Serial.println("modify bill operation mode");
    cctsend(modify_bill_operating_mode,40,1,1);
  while (RX_state < RXcomplete) {ccTalkReceive();}
  if (RX_state!=RXcomplete){
    Serial.println(RX_msg[RX_state]);// Show the error if any
    setup();
  }
delay(100);

//Serial.println("modify inhibit status");
    cctsend(modify_inhibit_status,40,2,255,255);
  while (RX_state < RXcomplete) {ccTalkReceive();}
  if (RX_state!=RXcomplete){
    Serial.println(RX_msg[RX_state]);// Show the error if any
    setup();
  }
delay(200);
}
void loop() {
RX_buffer[5] = 0;
RX_buffer[4] = 0;


    cctsend(read_buffered_bill_events,40,0);
  while (RX_state < RXcomplete) {ccTalkReceive();}
  if (RX_state!=RXcomplete){
    //Serial.println("read_buffered_bill_events");
    //Serial.println(RX_msg[RX_state]);// Show the error if any
    }
// printBINdata();
  if (RX_buffer[4] > lastevent)
  {
      if (RX_buffer[5] > 0)
        {
         // Serial.print("Schein kassiert!");
          switch (RX_buffer[5]) {
                               case 0:
                                break;
                               case 1:
                                Serial.println("5");
                                break;
                               case 2:
                                Serial.println("10");
                                break;
                               case 3:
                                Serial.println("20");
                                break;
                               case 4:
                                Serial.println("50");
                                break;
                               case 5:
                                Serial.println("100");
                                break;
                              }
          lastevent = RX_buffer[4] ;
            delay(200);
        }
  }
    else {
    if (RX_buffer[6] != 0) {
      Serial.print("Kein Schein, sondern: ");
      Serial.print(RX_buffer[6]);
                switch (RX_buffer[6]) {
                               case 0:
                                break;
                               case 1:
                                Serial.println("bill held/returned escrow");
                                break;
                               case 2:
                                Serial.println("invalid bill (validation)");
                                break;
                               case 3:
                                Serial.println("invalid bill (transport problem)");
                                break;
                               case 4:
                                Serial.println("inhibited bill (serial)");
                                break;
                               case 5:
                                Serial.println("inhibited bill (dip switch)");
                                break;
                               case 6:
                                Serial.println("bill jammed in transport");
                                break;
                               case 7:
                                Serial.println("bill jammed in stacker");
                                break;
                               case 8:
                                Serial.println("bill pulled backwards");
                                break;
                               case 9:
                                Serial.println("bill tamper");
                                break;
                              }
      RX_buffer[6]=0;
      delay(200);
    }
    }     

delay(200);

//Serial.println(loops);
//loops++;
      

}


