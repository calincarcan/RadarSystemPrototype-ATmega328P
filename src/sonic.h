#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

#define TRIG_PIN PB0
#define ECHO_PIN PB2

struct sonic_data {
    int time_ms;
    int distance_cm;
};

void init_ultrasonic_sensor() {
    // Set TRIG_PIN as output
    DDRB |= (1 << TRIG_PIN);
    
    // Set ECHO_PIN as input
    DDRB &= ~(1 << ECHO_PIN);
    
    // Ensure the TRIG_PIN is low
    PORTB &= ~(1 << TRIG_PIN);
}

void trigger_ultrasonic_sensor() {
    // Send a 10us high pulse to the TRIG_PIN
    PORTB |= (1 << TRIG_PIN);
    _delay_us(10);
    PORTB &= ~(1 << TRIG_PIN);
}

// Reads echo time in microseconds
struct sonic_data read_echo_time() {
    int count = 0;

    // Wait for the ECHO_PIN to go high
    while (!(PINB & (1 << ECHO_PIN)));
    
    // Start counting once the ECHO_PIN goes high
    while (PINB & (1 << ECHO_PIN)) {
        _delay_us(1);
        count++;
    }
    
    struct sonic_data data;
    data.time_ms = count;
    data.distance_cm = count / 58;
    return data;
}