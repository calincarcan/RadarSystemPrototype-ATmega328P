#include "twi.h"

void twi_init(void) {
    DDRC |= (1 << PC5);  // SCL output
    PORTC |= (1 << PC5); // SCL high pe idle

	TWCR = 0;
    TWBR = (uint8_t)TWBR_VAL;
    TWSR &= ~((1 << TWPS0) | (1 << TWPS1));
}

void twi_start(void) {    
	TWCR = 0;
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
	while (!(TWCR & (1 << TWINT)));
}

void twi_stop(void) {   
	TWCR = 0;
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);
}

void twi_write(uint8_t data) {
    TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while(bit_is_clear(TWCR, TWINT));
}

void twi_read_ack(uint8_t *data) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while(bit_is_clear(TWCR, TWINT));
    *data = TWDR;
}

void twi_read_nack(uint8_t *data) {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while(bit_is_clear(TWCR, TWINT));
    *data = TWDR;
}
