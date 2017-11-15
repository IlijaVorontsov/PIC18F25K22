/*
File:         Tryout.c
Author:       Ilija Vorontsov
Discription:  The 8Bit value of a potentiometer is determined by an ADC and
              displayed on PORTC.
Created on 10. Juli 2017, 12:48
*/

//including needed header files
#include "xc8.h"


//initializing functions
void Setup(void);
void interrupt isr(void);

//assigning variables
unsigned char i;

//Starting setup and endlessloop
void main(void) {
    Setup();

    while(1){
    }
}

//Setting up all used Special Funciton Registers.
void Setup(void){
    LATC =      0b00000000;
    ANSELC =    0b00000000;
    TRISC =     0b00000000;
    TRISB =     0b00010000;
    IOCB4 = 1;
    IOCB5 = 0;
    IOCB6 = 0;
    IOCB7 = 0;
    RBIP = 1;
    INTCON = 0b11001000;
}


//Interrupt Service Routine
void interrupt isr(void){
    //Checking if Timer0 interrupt was triggerd.
    if(RBIF && RBIE){
        
        //Disabling other interrupts stopping timer0 and reseting flagBit.
        GIE=0;
        PEIE = 0;
        RBIF = 0;
        RBIE = 0;
        
        LATCbits.LC1 = ~LATCbits.LC1;
        
        while(PORTBbits.RB4 == 1){}

        //Enableing other interrupts and starting timer0
        RBIE = 1;
        GIE=1;
        PEIE = 1;
   }
}
