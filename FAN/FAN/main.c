/*
 * FAN.c
 *
 * Created: 12/12/2022 5:21:56 PM
 * Author : Mohamed ali
 */ 


#define F_CPU 1600000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#define LCD PORTB
#define EN 7
#define RS 5
#define RW 6
void lcdcmd(unsigned char cmd){
	PORTD &= ~(1<<RS);
	PORTD &= ~(1<<RW);
	LCD=cmd& 0xF0;
	PORTD |=(1<<EN);
	_delay_ms(1);
	PORTD &= ~(1<<EN);
	
	LCD=cmd<<4;
	PORTD |=(1<<EN);
	_delay_ms(1);
	PORTD &= ~(1<<EN);
}
void lcddata(unsigned char data){
	PORTD |= (1<<RS);
	PORTD &= ~(1<<RW);
	LCD=data& 0xF0;
	PORTD |=(1<<EN);
	_delay_ms(1);
	PORTD &= ~(1<<EN);
	
	LCD=data<<4;
	PORTD |=(1<<EN);
	_delay_ms(1);
	PORTD &= ~(1<<EN);
}
void lcd_init(){
	DDRB =0XFF;
	DDRD =0xFF;
	PORTD &= ~(1<<EN);
	lcdcmd(0x33);
	lcdcmd(0x32);
	lcdcmd(0x28);
	lcdcmd(0x0E);
	lcdcmd(0x01);
	_delay_ms(2);
	
}
void lcd_print(char *str){
	unsigned char i=0;
	while(str[i]!=0){
		lcddata(str[i]);
		i++;
	}
}

void convertndisplay(unsigned char value)
{
	unsigned char x,d1=0,d2=0;
	x=(value/10);
	d1=x;
	d2=value%10;
	lcdcmd(0x8D);
	lcddata(d1+0x30);
	lcddata(d2+0x30);
}


int main()
{
	lcd_init();
	lcd_print("Temperature:");
	DDRA |= (1<<0);
	ADCSRA=0x87;
	ADMUX=0xE0;
	DDRC=0xFF;

	while(1)
	
	{
		unsigned char Temp;
		unsigned char i;

		ADCSRA |= (1<<ADSC);
		while((ADCSRA&(1<<ADIF))==0);
		Temp=ADCH;
		convertndisplay(Temp);
		_delay_ms(50);
		//PORTC=0b11101111; // for rotate clock wise
		/*_delay_ms(200);
		PORTC=0xFF; // for anti clockwise
		_delay_ms(200);*/
		i=64;
		OCR0=i;
		TCCR0=0x61;
		GICR=(1<<INT0);
		MCUCR=0x02;
		sei();
		

		if (Temp>0 && Temp<41)
		{
			PORTC=0b00001001;
			
			_delay_ms(10);
			PORTC=0b00000001;
			_delay_ms(30);
		} 
		else if(Temp>=41 && Temp<81)
		{
			PORTC=0b00001010;
			
			_delay_ms(40);
			PORTC=0b00000010;
			_delay_ms(40);

			
		}
		else if(Temp>=81 && Temp<100)
		{
			PORTC=0b00001100;
			
			

		}
		else if(Temp>=100)
		{
			PORTC=0b11101100;
			
		}
		else{
			PORTC=0x00;
		}	
	}
}

