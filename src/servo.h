#include <avr/io.h>

void start_servo() {
    TCCR1B |= (1 << CS11);
}

void stop_servo() {
	TCCR1B &= ~(1 << CS11);
}

void servo_init() {
    DDRB |= (1 << DDB1);  // Pin D9 is PB1 (OC1A)

    TCCR1A |= (1 << COM1A1) | (1 << WGM11); // Fast PWM
    TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11); // Prescaler de 8
    // TCCR1B |= ((1 << WGM12) | (1 << WGM13)); // Prescaler de 8

    // Frecventa 50 Hz
    ICR1 = 40000;
    OCR1A = 800;
}