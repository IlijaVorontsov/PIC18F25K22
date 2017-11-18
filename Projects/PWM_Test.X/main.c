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
void ADC_Setup(void);
void TMR0_Setup(void);
void INT_Setup(void);

//assigning variables
unsigned char i;

//Starting setup and endless loop
void main(void) {
    
    Setup();
    PWM_Setup();
    ADC_Setup();
    INT_Setup();
    TMR0_Setup();
    
    while(1){
    
    
    }
}

//Setting up all used Special Function Registers.
void Setup(void){
    TRISC = 0x00;
    LATC    = 0b00000000;
    ANSELC  = 0x00;
    TRISAbits.RA2 = 0;
    
    INTCON =  0b11100000;
}

void INT_Setup(void){
    INTCONbits.GIE      =   1;          // Global Interrupt Enable bit
    INTCONbits.PEIE     =   1;          // Peripheral Interrupt Enable bit
    INTCONbits.TMR0IE   =   1;          // TMR0 Overflow Interrupt Enable bit
}

void TMR0_Setup(void){
    T0CONbits.TMR0ON    =   1;          
    INTCONbits.TMR0IF   =   1;    
}

void ADC_Setup(void){
    // Setting up RA0 to be a analog input
    TRISAbits.RA0       =   1;          // Data Direction bit
    ANSELAbits.ANSA0    =   1;          // Analog Select bit
    
    // Configuring A/D control registers 
    ADCON0bits.CHS      =   0b00000;    // Analog Channel Select bits
    ADCON2bits.ACQT     =   0b111;      // Acquisition time select bit
    ADCON2bits.ADCS     =   0b111;      // A/D Conversion Clock Select
    PIE1bits.ADIE       =   1;          // A/D Converter Interrupt Enable bit
    
    // Starting A/D Conversion
    ADCON0bits.ADON     =   1;          // ADC Enable bit
    ADCON0bits.GODONE   =   1;          // A/D Conversion Status bit
}


void PWM_Setup(void){
    
    //Setting RC2 to CCP1 mode but disabling output
    TRISAbits.RA4 = 1;   
    
    
    
    CCPTMRS1bits.C5TSEL = 0b00;     //selecting Timer2 as Timer source for CCP1
    CCP5CONbits.CCP5M = 0b1100;     //selecting PWM Mode for CPP1
    PR2 = 0b11111111;               //setting the Period Register of Timer2
    T2CONbits.T2CKPS = 0b00;        //setting PreScaler of Timer2 to 16
    
    
    //loading Duty-Cycle value
    CCPR5 = 0b00011111;         //Bits 10-2
    CCP5CONbits.DC5B = 0b00;    //Bits 1-0
    
    PIR1bits.TMR2IF = 0;
    T2CONbits.TMR2ON = 1;
    TRISAbits.RA4 = 0;  
}

//Interrupt Service Routine
void interrupt isr(void){
    //Checking if Timer0 interrupt was triggered.
    if(TMR0IE && TMR0IF){

        //Disabling other interrupts stopping timer0 and reseting flagBit.
        TMR0ON = 0;
        TMR0IF=0;
        GIE=0;

        //Setting up time for new interrupt.
        setTMR0_ms(500);
        LATAbits.LA2 = ~LATAbits.LA2;
        
        //enabling other interrupts and starting timer0
        GIE=1;
        TMR0ON=1;
   }
    if (PIR1bits.ADIF && PIE1bits.ADIE){
        GIE = 0;
        PIR1bits.ADIF = 0;
        PIE1bits.ADIE = 0;
        T2CONbits.TMR2ON = 0;
        CCPR5 = ADRESH;
        T2CONbits.TMR2ON = 1;

        LATC = ADRESH;
        PIE1bits.ADIE = 1;
        PIR1bits.ADIF =0;
        ADCON0bits.GODONE = 1;
        GIE = 1;

    }
}
