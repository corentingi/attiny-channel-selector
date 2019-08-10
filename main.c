#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t i = 0;                   // counter variable
uint8_t previousReadingPB3 = 1;  // holds last 8 pin reads from pushbutton
uint8_t buttonWasPressedPB3 = 1; // keeps track of debounced button state, 0 false, 1 true
uint8_t previousReadingPB4 = 1;  // holds last 8 pin reads from pushbutton
uint8_t buttonWasPressedPB4 = 1; // keeps track of debounced button state, 0 false, 1 true

static inline void initTimer1(void)
{
    TCCR1 |= (1 << CTC1);                             // clear timer on compare match
    TCCR1 |= (1 << CS12) | (1 << CS11) | (1 << CS10); //clock prescaler 4
    OCR1C = 50;                                       // compare match value to trigger interrupt every 200us ([1 / (1E6 / 4)] * 50 = 200us)
    TIMSK |= (1 << OCIE1A);                           // enable output compare match interrupt
    sei();                                            // enable interrupts
}

ISR(TIMER1_COMPA_vect)
{
    // PB3
    if ((PINB & (1 << PB3)) != previousReadingPB3) // if current button pin reading doesn't equal previousReadingPB3 200us ago,
    {
        if (!buttonWasPressedPB3) // and button wasn't pressed last time
        {
            if (i < 7) // increment i from 0 to 7
                i++;
            else
                i = 0;
            PORTB = (PORTB & 0b11111000) | (i & 0b00000111); // Set PORTB to i with a mask
            buttonWasPressedPB3 = 1;                         // set debounced button press state to 1
        }
        else
            buttonWasPressedPB3 = 0; // button has been let go, reset buttonWasPressed
    }
    previousReadingPB3 = (PINB & (1 << PB3)); // set previous reading to current reading for next time

    // PB4
    if ((PINB & (1 << PB4)) != previousReadingPB4) // if current button pin reading doesn't equal previousReadingPB4 200us ago,
    {
        if (!buttonWasPressedPB4) // and button wasn't pressed last time
        {
            if (i > 0) // decrement i from 7 to 0
                i--;
            else
                i = 7;
            PORTB = (PORTB & 0b11111000) | (i & 0b00000111); // Set PORTB to i with a mask
            buttonWasPressedPB4 = 1;                         // set debounced button press state to 1
        }
        else
            buttonWasPressedPB4 = 0; // button has been let go, reset buttonWasPressed
    }
    previousReadingPB4 = (PINB & (1 << PB4)); // set previous reading to current reading for next time
}

int main(void)
{
    // initializations
    DDRB = 0b00000111;   // enable PB0-PB2 as outputs
    PORTB |= (1 << PB3); // enable pullup on pushbutton output hooked up to PB3
    PORTB |= (1 << PB4); // enable pullup on pushbutton output hooked up to PB4
    initTimer1();        // initialize timer and interrupt

    while (1)
    {
    }
    return 0;
}
