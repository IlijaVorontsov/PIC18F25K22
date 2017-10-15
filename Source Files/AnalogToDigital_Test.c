/*
File:         Tryout.c
Author:       Ilija Vorontsov
Discription:  The 8Bit value of a potentiometer is determined by an ADC and
              displayed on PORTC.
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
    //Checking if Timer0 interrupt was triggerd.
    if(TMR0IE && TMR0IF){

        //Disabling other interrupts stopping timer0 and reseting flagBit.
        TMR0ON = 0;
        TMR0IF=0;
        GIE=0;

        //Setting up time for new interrupt.
        setTMR0_ms(20);

        // ADC routine and setting LED´s on PORTC to the ADC result.
        if (GODONE == 0){
            LATC = ADRESH;
            GODONE = 1;
        }

        //Enableing other interrupts and starting timer0
        GIE=1;
        TMR0ON=1;
   }
}
