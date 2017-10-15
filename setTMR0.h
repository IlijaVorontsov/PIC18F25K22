/*
 * File:   setTMR0.h
 * Author: Ilija Vorontsov  
 * Programm zur automatischen dimensionierung des Prescalers, sowie setzen der
 * TMR0L und TMR0H SFR.
 * Created on 16. Mai 2017, 15:25
 */

long setPreScaler(long t);
void setTMR0_ms(long t);


void setTMR0_ms(long t) {
    long PreScaler=0, StartingValue=0;              //Variablen initialisiert und deklariert
    
    PreScaler = setPreScaler(t);           //Prescaler einstellen

    StartingValue=0x10000 - t*2765/PreScaler;    //Berechnung der Einstellung für Timer0
    TMR0L=StartingValue%256;    //Timer 0 Low Byte setzen
    TMR0H=StartingValue/256;    //Timer 0 High Byte setzen
    
}   //Programm zum setzen des Timer0 Low und High bytes

long setPreScaler(long t){
 long PreScaler=0;
 char HighBitCounter=0;
 
 PreScaler=(t*2765)/0x10000;     //Berechnen des idealen Prescalers  
 
 while(PreScaler>1){            //Ermittlung der Stelle des höchseten Bits
    HighBitCounter++;    
    PreScaler=PreScaler>>1;      
 }
    
    PreScaler = PreScaler<<(HighBitCounter+1);           //Einstellen des Bits um eine Stelle höher
    if(PreScaler>1){               
        T0CON=HighBitCounter; //Prescaler einstellen
    }else{
        T0CON=8; //PSA High rest Low
    }
    return(PreScaler);
}   //Programm zur Berechnung und setzen des Prescalers

