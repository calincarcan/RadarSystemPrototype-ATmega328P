#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usart.h"
#include "twi.h"
#include "lcd.h"
#include "sonic.h"
#include "buzzer.h"
#include "servo.h"

#define F_CPU 16000000L
#define BAUD_RATE 9600
#define LED_PIN PD3

volatile int sensor_state = 1;

void setup() {
	USART0_init(CALC_USART_UBRR(BAUD_RATE));
	USART0_use_stdio();

	init_ultrasonic_sensor();
	setup_buzzer();
	stop_buzzer();
	twi_init();
	twi_lcd_init();

	DDRD |= _BV(LED_PIN);
}

void setup_ISR() {
   // Set D4 as input pin
	DDRD &= ~(1 << PD4);  // Set PD4 (D4) as input

  // Enable pull-up resistor
	PORTD |= (1 << PORTD4);
	
	PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PORTD4);

	// Enable global interrupts
	sei();
}

ISR(PCINT2_vect) {
	if (sensor_state == 0) {
		sensor_state = 1;
	} else {
		sensor_state = 0;
	}
	_delay_ms(200);
}

int main() {
	setup();
	setup_ISR();
	servo_init();
	_delay_ms(100);

	uint16_t servo_increment = 40;
	int lcd_err = 0;

	while (1) {
		stop_servo();
		if (OCR1A >= 4800)
			servo_increment = -40;
		else if (OCR1A <= 810)
			servo_increment = 40;
		OCR1A += servo_increment;
		start_servo();

		stop_servo();

		if (sensor_state) {
			trigger_ultrasonic_sensor();
			struct sonic_data data = read_echo_time();
			if (data.distance_cm >= 3 && data.distance_cm <= 50) {
				start_buzzer();
				PORTD |= _BV(LED_PIN);

				lcd_err = 0;
				lcd_print_info(data.distance_cm, data.time_ms);

				_delay_ms(200);
				stop_buzzer();
				PORTD &= ~_BV(LED_PIN);
			} else if (lcd_err != 1) {
				lcd_clear();
				lcd_print("Unexpected value");
				lcd_err = 1;
			}
		}
		start_servo();
		_delay_ms(100);
	}

	return 0;
}