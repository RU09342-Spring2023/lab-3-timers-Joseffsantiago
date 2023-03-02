/*
 * Part3.c
 *
 *  Created on: Feb 12, 2023
 *      Author: Joseff Santiago
 *
 *      YOU NEED TO FILL IN THIS AUTHOR BLOCK
 */

#include <msp430.h>
unsigned int t = 8192;
void gpioInit();
void timerInit();

void main(){

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    gpioInit();                             //calls initialization functions and sets a variable to track the button duration and frequency
    timerInit();

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
    P2IFG &=~ BIT3;
    P4IFG &=~ BIT1;
    __bis_SR_register(GIE);                 // Enter LPM3 w/interrupt
}

void timerInit(){
        TB1CCTL0 = CCIE;                          // TBCCR0 interrupt enabled
        TB1CCR0 = t;                              // triggers on t
        TB1CTL = TBSSEL_1 | MC_2;                 // ACLK, continuous mode

        TB1CCTL1 = CCIE;
        TB1CCR1 = 8192;                          // Initialized with a random value
        TB1CCR1 = TBSSEL_1 | ID_3 | MC_2;        // ACLK divided by 8
}

void gpioInit(){
        // Configure RED LED on P1.0 as Output
                P1OUT &= ~BIT0;                         // Clear P1.0 output latch for a defined power-on state
                P1DIR |= BIT0;                          // Set P1.0 to output direction

        // Configure Green LED on P6.6 as Output
                P6OUT &= ~BIT6;                         // Clear P6.6 output latch for a defined power-on state
                P6DIR |= BIT6;                          // Set P6.6 to output direction

        // Configure Button on P2.3 as input with pullup resistor
                P2OUT |= BIT3;                          // Configure P2.3 as pulled-up
                P2REN |= BIT3;                          // P2.3 pull-up register enable
                P2IES &= ~BIT3;                         // P2.3 Low --> High edge
                P2IE |= BIT3;                           // P2.3 interrupt enabled
        //Configure Button on P4.1 as an input with pullup resistor
                P4OUT |= BIT1;                          // Configure P2.3 as pulled-up
                P4REN |= BIT1;                          // P4.1 pull-up register enable
                P4IES &= ~BIT1;                         // P4.1 Low --> High edge
                P4IE  |= BIT1;                          // P4.1 interrupt enabled
}

#pragma vector = TIMER1_B0_VECTOR                       //some interrupt stuff; copy and pasted from previous part, still not sure how it works
__interrupt void Timer1_B0_ISR(void)
{
    P1OUT ^= BIT0;                      //toggles LED
    TB1CCR0 += t;                       //offsets CCR0 by t
}
