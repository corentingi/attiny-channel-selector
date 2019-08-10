#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t selectedChannel = 0;     // channel selection (counter)
volatile uint8_t previousState = 0;

// Intialize TIMER1
static inline void initTimer1(void)
{
    TCCR1 |= (1 << CTC1);                             // clear timer on compare match
    TCCR1 |= (1 << CS12) | (1 << CS11) | (1 << CS10); //clock prescaler 4
    OCR1C = 50;                                       // compare match value to trigger interrupt every 200us ([1 / (1E6 / 4)] * 50 = 200us)
    TIMSK |= (1 << OCIE1A);                           // enable output compare match interrupt
    sei();                                            // enable interrupts
}

// Debounce function
unsigned char debounce(unsigned char sample)
{
    static unsigned char state, cnt0, cnt1;
    unsigned char delta;

    delta = sample ^ state;
    cnt1 = (cnt1 ^ cnt0) & (delta & sample);
    cnt0 = ~cnt0 & (delta & sample);
    state ^= (delta & ~(cnt0 | cnt1));

    return state;
}

// Interrupt function for TIMER1
ISR(TIMER1_COMPA_vect)
{
    // Debounced pin state
    uint8_t state = debounce(PINB);

    // If PB3 was just released (rising edge with INPUT_PULLUP)
    // PB3 increment selected channel
    if ( (state & (1 << PB3)) && !(previousState & (1 << PB3)) )
    {
        if (selectedChannel < 7)
            selectedChannel++;
    }

    // If PB4 was just released (rising edge with INPUT_PULLUP)
    // PB4 decrement selected channel
    if ( (state & (1 << PB4)) && !(previousState & (1 << PB4)) )
    {
        if (selectedChannel > 0)
            selectedChannel--;
    }

    if (state != previousState)
        PORTB = (PORTB & 0b11111000) | (selectedChannel & 0b00000111); // Set PORTB to i with a mask

    // Save current state for future comparision
    previousState = state;
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
