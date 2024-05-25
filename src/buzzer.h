#include <avr/io.h>
#include <util/delay.h>

void setup_buzzer() {
	// Set the PWM pin as output
	DDRD |= (1 << DDD6); // PD6 (OC0A) as output

	// Set Fast PWM mode with non-inverted output
	TCCR0A |= (1 << WGM01) | (1 << WGM00); // Fast PWM mode
	TCCR0A |= (1 << COM0A1); // Clear OC0A on Compare Match, set OC0A at BOTTOM (non-inverting mode)

	TCCR0B |= (1 << CS01) | (1 << CS00); // Prescaler = 64

	OCR0A = 250;
}

void start_buzzer() {
     TCCR0B |= (1 << CS01) | (1 << CS00); // Start timer with prescaler = 8
}

void stop_buzzer() {
     TCCR0B &= ~((1 << CS01) | (1 << CS00)); // Stop timer by clearing the prescaler bits
}

void ping_buzzer() {
	start_buzzer();
	_delay_ms(600);
	stop_buzzer();
}