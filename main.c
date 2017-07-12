/*
 * main.c
 *
 *  Created on: Jul 12, 2017
 *      Author: leenovoz510
 */

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

 int adValue ;
#define  LCD_RS_Pin 5
#define  LCD_RW_Pin 6
#define  LCD_Enable_Pin 7

void send_command(char command);
void send_data(char data);
void clear_screan();
void Go_to(char x ,char y);
void Print_On_Screen(char *str);
void LCD_init();
void ADC_Init();

void main(void){
	DDRA = 0;
		DDRC |= 0xff;
		DDRD |= 0xff;
		_delay_ms(10);
		LCD_init();
		_delay_ms(10);
		clear_screan();
	 	char  x[20] ;
	 	ADC_Init();
		while(1){
			 ADCSRA |= (1 << ADSC);		// Start Conversion
			 while( (1<<ADIF)==1);		// check if data has been sent
			char string[16];
			 itoa(ADCH, string, 10); // 10 is radix
		    Go_to(1, 1);
		    Print_On_Screen("Temp is ");
		    Go_to(10, 1);
		    Print_On_Screen(string);
		}
}

void ADC_Init(){
	ADMUX |=(1<<REFS0)|(1<<REFS1);   // Refernce Voltage is 2.5
	ADCSRA |= (1 << ADEN);			// Enable ADC
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1<<ADPS2);	// 128 Divition
	ADMUX |= (1 << ADLAR);			//Adjust ADC Results

}
void send_command(char command){
	PORTD = command;
	PORTC &= ~(1<<LCD_RS_Pin);
	PORTC &= ~(1<<LCD_RW_Pin);

	PORTC |= (1<<LCD_Enable_Pin);
	_delay_ms(10);
	PORTC &= ~(1<<LCD_Enable_Pin);
}
void send_data(char data){
	PORTD = data;

	PORTC |= (1<<LCD_RS_Pin);
	PORTC &= ~(1<<LCD_RW_Pin);

	PORTC |= (1<<LCD_Enable_Pin);
	_delay_ms(10);
	PORTC &= ~(1<<LCD_Enable_Pin);

}
void clear_screan(){
	send_command(0x01);
	_delay_ms(10);
}
void Print_On_Screen(char *str){
	int i=0;
	while(str[i] != 0){
		send_data(str[i]);
		i++;
	}
}
void Go_to(char x ,char y){
	char firstAddress[] =

{0x80,0xC0,0x94,0xD4};
	send_command(firstAddress[y-1] + x-1);
		_delay_ms(10);
}
void LCD_init(){
	send_command(0x38);  // 8-bit mode
	send_command(0x0C);
	send_command(0x06);
	send_command(0x80);
		_delay_ms(10);
}
