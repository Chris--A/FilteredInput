#include <FilteredInput.h>

char colRed[] PROGMEM = "red";
char colGreen[] PROGMEM = "green";
char colBlue[] PROGMEM = "blue";
char colPurple[] PROGMEM = "purple";
char colPink[] PROGMEM = "pink";
char colOrange[] PROGMEM = "orange";
char modCray[] PROGMEM = "cray";
char modProgram[] PROGMEM = "program";
char modRandom[] PROGMEM = "random";
char *cCodes[] PROGMEM = { colRed, colGreen, colBlue, colPurple, colPink, colOrange, modCray, modProgram, modRandom, 0x00 };

FilterStream< 7 > filter( Serial, cCodes, run );

void run( char idx ){
  
   switch( idx ){
     case 0:  case 1: case 2:
     case 3:  case 4: case 5:
       Serial.print(  F("Got COLOUR change: ") );
       break; 
       
     case 6: case 7: case 8:
       Serial.print(  F("Got MODE change: ") );
       break;    
   }
  Serial.println( ( __FlashStringHelper* ) pgm_read_word( &cCodes[ idx ] ) );
}

void setup() {
  Serial.begin( 115200 );
}

void loop() {
  static unsigned long time = millis();

  if( filter.run() ) 
    time = millis();
  else if( millis() > ( time + 100 ) ) 
    filter.flush();
}