/*
File:   Tryout.c
Author: Ilija

Created on 10. Juli 2017, 12:48
*/

//including needed header files
#include "xc8.h"
#include "setTMR0.h"

//initializing functions
void init(void);
void interrupt isr(void);

//assigning variables
unsigned char i;
long AnalogRead ;

//Starting setup and endlessloop
void main(void) {
    Setup();

    while(1){}
}

//Setting up all used Special Funciton Registers.
void Setup(void){
    TRISC =   0b00000000;
    ANSELC =  0b00000000;
    LATC =    0b00000000;
    ANSELA =  0b00000101;
    TRISA =   0b00000001;
    T0CON =   0b10000000;
    TMR0L =   0b11111111;
    TMR0H =   0b11111111;
    LATA =    0b00000000;
    INCON =   0b11100000;
    ADCON0 =  0b00000001;
    ADCON2 =  0b00111111;
}


//Interrupt Service Routine
void interrupt isr(void){
    //Chacking if
    if(TMR0IE && TMR0IF){

        TMR0ON = 0;
        TMR0IF=0;

        GIE=0;
        setTMR0_ms(20);
        if (GODONE == 0){
            AnalogRead = ADRESH;
            GODONE = 1;        //Starts conversion
        }
        LATC = AnalogRead;
        GIE=1;
        TMR0ON=1;
   }
}
