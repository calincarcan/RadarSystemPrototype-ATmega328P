#include "twi.h"
#include <util/delay.h>
#define LCD_ADDR 0x27

// Trimitere comanda spre LCD
void lcd_write_cmd(unsigned char address);

// Trimitere date catre slave
void lcd_write_data(unsigned char data);

void module_write(unsigned char data);

void lcd_4bit_send(unsigned char data);

// Scriere in registrul de comanda al LCD-ului
void twi_lcd_cmd(unsigned char x);

void twi_lcd_data(unsigned char x);

void lcd_print(char *c);

void twi_lcd_init();

void lcd_clear();

void lcd_first_line();

void lcd_second_line();

void lcd_print_info(int distance_cm, int time_ms);