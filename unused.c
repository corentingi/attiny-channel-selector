// main.c
//
// A channel selector to use with fatshark modules and 2 push buttons
// Connect red LED at pin 2 (PB3)
//

#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
// #include <util/delay.h>

volatile uint8_t i = 0;

static inline void initTimer1(void)
{
    TCCR1 |= (1 << CTC1);                             // clear timer on compare match
    TCCR1 |= (1 << CS13) | (1 << CS12) | (1 << CS11); // clock prescaler 8192
    OCR1C = 122;                                      // compare match value
    TIMSK |= (1 << OCIE1A);                           // enable compare match interrupt
}

ISR(TIMER1_COMPA_vect)
{
    if (i == 15)
        i = 0;
    else
        i++;

    PORTB = i; // write updated i to PORTB
}

int main(void)
{
    // initializations
    DDRB = 0b00000111;   // enable PB0-PB2 as outputs
    PORTB |= (1 << PB4); // enable pullup on pushbutton output
    initTimer1();        // initialize timer registers
    sei();               // enable interrupts

    while (1)
    {
    }

    return 0;
}

// int main(void)
// {
//     // Set pins 5,6 and 7 as outputs (PB0,PB1,PB2)
//     DDRB = 0b00000111;
//     while (1)
//     {
//         // Alternate outputs
//         PORTB = 0b00000001;
//         _delay_ms(1000);
//         PORTB = 0b00000010;
//         _delay_ms(1000);
//         PORTB = 0b00000100;
//         _delay_ms(1000);
//     }

//     return 1;
// }


// unsigned char debounce(unsigned char sample)
// {
//     static unsigned char state, cnt0, cnt1;
//     unsigned char delta;

//     delta = sample ^ state;
//     cnt1 = (cnt1 ^ cnt0) & (delta & sample);
//     cnt0 = ~cnt0 & (delta & sample);
//     state ^= (delta & ~(cnt0 | cnt1));

//     return state;
// }
