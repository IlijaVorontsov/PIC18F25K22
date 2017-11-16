/*
File:           PWM_Test.c
Author:         Ilija Vorontsov
Description:    An analog value should be converted to a digital 10 bit value.
 *              The digital value should be displayed by the duty cycle of the 
 *              PWM output on RC2.
Created:        15. November 2017
*/

//including needed header files
#include "xc8.h"
#include "setTMR0.h"

//initializing functions
void Setup(void);
void interrupt isr(void);
void PWM_Setup(void);

//assigning variables
unsigned char i;

//Starting setup and endless loop
void main(void) {
    Setup();
    PWM_Setup();
    while(1){
    
    
    }
}

//Setting up all used Special Function Registers.
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
    INTCON =  0b11100000;
    ADCON0 =  0b00000001;
    ADCON2 =  0b00111111;
}


void PWM_Setup(void){
    
    //Setting RC2 to CCP1 mode but disabling output
    TRISCbits.RC2 = 1;   
    ANSELCbits.ANSC2 = 0;
    
    
    CCPTMRS0bits.C1TSEL = 0b00;     //selecting Timer2 as Timer source for CCP1
    CCP1CONbits.CCP1M = 0b1100;     //selecting PWM Mode for CPP1
    PR2 = 0b11111111;               //setting the Period Register of Timer2
    T2CONbits.T2CKPS = 0b00;        //setting PreScaler of Timer2 to 16
    
    
    //loading Duty-Cycle value
    CCPR1 = 0b11111111;         //Bits 10-2
    CCP1CONbits.DC1B = 0b11;    //Bits 1-0
    
    PIE1bits.TMR2IE = 1;
    PIR1bits.TMR2IF = 0;
    T2CONbits.TMR2ON = 1;
    TRISCbits.RC2 = 0;  
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


        
        //enabling other interrupts and starting timer0
        GIE=1;
        TMR0ON=1;
   }
}
