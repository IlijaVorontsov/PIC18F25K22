/*
 * File:   Tryout.c
 * Author: Ilija
 *
 * Created on 10. Juli 2017, 12:48
 */

//including needed header files
#include "xc8.h"
#include "setTMR0.h"

void init(void);
void interrupt isr(void);

unsigned char i;
long AnalogRead ;

void main(void) {

    init();
 
    while(1){
    
    }
    
}


void init(void){
    TRISC = 0;
    ANSELC = 0;
    LATC = 0;
    ANSELA = 0b00000101;
    TRISA = 0b00000001;
    LATA=0;
    TMR0ON=0;       
    T08BIT=0;          
    T0CS=0;              
    T0SE=0;              
    GIE=1;
    PEIE=1;
    TMR0IE=1; 
    PSA=0;
    T0PS0=0;
    T0PS1=1;
    T0PS2=1;
    TMR0L=0xFF;       
    TMR0H=0xFF;
    TMR0ON=1;
    VREFCON1=224;
    VREFCON2=0;
    CHS4 = 0;    //Analog Chanel Select
    CHS3 = 0;
    CHS2 = 0;
    CHS1 = 0;
    CHS0 = 0;
    ADON = 1;            //ADC enable
    ACQT2 = 1;     //Acquisition Time
    ACQT1 = 1;
    ACQT0 = 1;
    ADCS2 = 1;     //A/D Conversion Clock
    ADCS1 = 1;
    ADCS0 = 1;
    VREFCON1 = 0b11100000;
}

void interrupt isr(void){
    if(TMR0IE && TMR0IF){

        TMR0ON = 0;     
        TMR0IF=0;
        
        GIE=0;
        setTMR0_ms(20);
        LATAbits.LA0=~LATAbits.LA0;
        if (GODONE == 0){
            AnalogRead = ADRESH;
            GODONE = 1;        //Starts conversion
        }
        LATC = AnalogRead;
        GIE=1;
        TMR0ON=1;
   } 
}


/* 
 * Interupt on Change (IoC):
 * RBIE = 1 ;               //enables Interupt on Change on Port B
 * IOCB<7:4> = 1 ;      //makes the Pins<7:4> IoC anble
 * RBIP = 1 ;           //Sets the priority of the IoC on Port B 
*/

/*
 * Digital to Analog Converter (DAC):
 * VREFCON1 = 0b11100000 ;      //Setup to get the output to RA2
 * DACR<4:0> = value ;          //VOUT = ((VSRC+) - (VSRC-))*(DACR<4:0>/(2^5)) + VSRC
*/

/*
 * Analog to Digital Converter (ADC):
 * Setup:
 * TRISA1 = 1;
 * ANSELA1 = 1;
 * CHS<4:0> = 00000;    //Analog Chanel Select
 * ADON = 1;            //ADC enable
 * ACQT<2:0> = 111;     //Acquisition Time
 * ADCS<2:0> = 111;     //A/D Conversion Clock
 * Routine:
 * if (ADCON0<1>=0){
 * Upper8Bits = ADRESH; //Stores results
 * Lower8Bits = ADRESL;
 * ADCON<1> = 1;        //Starts conversion
 * }
 * 
 */