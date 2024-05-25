#include "lcd.h"
#include <util/delay.h>

unsigned char lcd = 0x00;

void lcd_write_cmd(unsigned char address) {
	TWDR=address;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	while(TW_STATUS != TW_MT_SLA_ACK);
}

void lcd_write_data(unsigned char data) {
	TWDR=data;
	TWCR=(1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));	
	while(TW_STATUS != TW_MT_DATA_ACK);
}

void module_write(unsigned char data) {
    twi_start();
    lcd_write_cmd(LCD_ADDR << 1);
    lcd_write_data(data);
    twi_stop();
}

void lcd_4bit_send(unsigned char data) {
    unsigned char temp = 0x00;
    // Write high nibble
    lcd &= 0x0F; // Clear data
    temp = data & 0xF0; // Get high nibble
    lcd |= temp; // Set high nibble
    lcd |= 0x04; // Set enable
    module_write(lcd); // Write data
    _delay_us(1); // Delay
    lcd &= ~(0x04); // Clear enable
    module_write(lcd); // Write data
    _delay_us(5); // Delay

    // Write low nibble
    temp = ((data & 0x0F) << 4); // Get low nibble
    lcd &= 0x0F; // Clear data
    lcd |= temp; // Set low nibble
    lcd |= 0x04; // Set enable
    module_write(lcd); // Write data
    _delay_us(1); // Delay
    lcd &= ~(0x04); // Clear enable
    module_write(lcd); // Write data
    _delay_us(5);
}

// Scriere in registrul de comanda al LCD-ului
void twi_lcd_cmd(unsigned char data) {
	lcd = 0x08;
	lcd &= ~(0x01);
	module_write(lcd);
	lcd_4bit_send(data);
}

void twi_lcd_data(unsigned char data) {
	lcd |= 0x09;
	module_write(lcd);
	lcd_4bit_send(data);
}

// Trimitere string
void lcd_print(char *c) {
	while (*c != '\0')
	twi_lcd_data(*c++);
}

// Initializare LCD cu 4-Bit Mode, setare cursor și pozitionare pe linie si coloană
void twi_lcd_init() {	
	lcd = 0x04;
	module_write(lcd);
	_delay_us(25);
	
	twi_lcd_cmd(0x03);
	twi_lcd_cmd(0x03);
	twi_lcd_cmd(0x03);
	twi_lcd_cmd(0x02);
	twi_lcd_cmd(0x28); // 4 bit mode, 2 lines, 5x8 font
	twi_lcd_cmd(0x0F); // display on, cursor on, blink on
	twi_lcd_cmd(0x01); // clear display
	twi_lcd_cmd(0x06); // setare cursor cu incrementare
	twi_lcd_cmd(0x80); // setare prima linie
}

void lcd_clear() {
	twi_lcd_cmd(0x01);
    twi_lcd_cmd(0x80);
}

void lcd_first_line() {
    twi_lcd_cmd(0x80);
}

void lcd_second_line() {
    twi_lcd_cmd(0xC0);
}

void lcd_print_info(int distance_cm, int time_ms) {
	char buffer[16] = "";
	lcd_clear();
	lcd_print("Dist: ");
	sprintf(buffer, "%d", distance_cm);
	lcd_print(buffer);
	lcd_print(" cm");
	lcd_second_line();
	lcd_print("Dur: ");
	sprintf(buffer, "%d", time_ms);
	lcd_print(buffer);
	lcd_print(" ms");
}