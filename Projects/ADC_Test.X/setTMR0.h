/*
  File:         setTMR0.h
  Author:       Ilija Vorontsov
  Discription:  Sets the Prescaler automaticly and fills Timer0 counter registers according
                to the given time-value.

  Created on 16. May 2017, 15:25
 */


// Calculate for your own Oszillator:   TimeConst = FreqOsz / 4000
#define TimeConst 2765


long setPreScaler(long timeval);
void setTMR0_ms(long timeval);


void setTMR0_ms(long timeval) {
  long PreScaler = 0 , StartingValue = 0;

  // Calculating and setting the prescaler.
  PreScaler = setPreScaler(timeval);

  // Calculating and setting startingvalues for both Timer 0 registers.
  StartingValue = 0x10000 - timeval * TimeConst / PreScaler;
  TMR0L = StartingValue % 256;
  TMR0H = StartingValue / 256;
}

long setPreScaler(long t){
 long PreScaler = 0;
 char HighBitCounter = 0;

 // Calculating ideal prescaler.
 PreScaler = (t * TimeConst) / 0x10000;

 //Calculating real prescaler.
 while(PreScaler > 1){
    HighBitCounter++;
    PreScaler = PreScaler >> 1;
  }
  PreScaler = PreScaler << (HighBitCounter + 1);

  //Setting prescaler
  if(PreScaler > 1){
      T0CON = HighBitCounter;
  }else{
      T0CON = 0b00001000;
    }
  return(PreScaler);
  }
