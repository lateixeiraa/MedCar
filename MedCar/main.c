/*
 * MedCar.c
 *
 * Created: 23/11/2020 10:22:43
 * Author : Larissa e Marina - PC
 */ 

#define F_CPU 16000000UL //Frequência de trabalho da CPU
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "nokia5110.h"

float dutyCycle;
char Status = ' ', Velocidade = ' ';

void Menu()
{
	nokia_lcd_clear();
	nokia_lcd_set_cursor(0,0);
	nokia_lcd_write_string("MedCar",2);
	nokia_lcd_set_cursor(0,20);
	nokia_lcd_write_string("Motor: ",1);
	nokia_lcd_write_char(Velocidade,1);
	nokia_lcd_set_cursor(0,30);
	nokia_lcd_write_string("Modo: ",1);
	nokia_lcd_write_char(Status,1);
	nokia_lcd_set_cursor(0,40);
	nokia_lcd_write_string("Sensor: ",1);
	nokia_lcd_set_cursor(50,40);
	nokia_lcd_write_string("OFF ",1);
	nokia_lcd_render();
	
	
}
ISR(INT0_vect) // Sensor
{

	Menu();
	nokia_lcd_set_cursor(50,40);
	nokia_lcd_write_string("ON ",1);
	nokia_lcd_render();
	_delay_ms(3000);
	
}

ISR(USART_RX_vect)
{
	char recebido;
	recebido = UDR0;

	
	// Modo do MedCar
	if(recebido=='l')
	{
		PORTC = 0b00000001; // Ativado
		Status = 'L';
		
		PORTD = 0xFF;
	}
	if(recebido=='d')
	{
		PORTC = 0b00000000;	// Desativado
		Status = 'D';	
		
		PORTD = 0x00;
	}

	
	//Motor
	if (recebido == '1' )// Local 1 á frente
	{
//		dutyCycle = 25;
		Velocidade = '1';
		
		PORTD = 0b00001100;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b01000100;
		_delay_ms(50);

	}
	
	if (recebido == '2' )// Local 2 á frente
	{
//		dutyCycle = 50;
		Velocidade = '2';
		
		PORTD = 0b00001100;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b01000100;
		_delay_ms(50);
		PORTD = 0b00001100;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b01000100;
		_delay_ms(50);
		
	}
	
	if (recebido == '3' )// Local 3 á frente
	{
//		dutyCycle = 75;
		Velocidade = '3';
		
		PORTD = 0b00001100;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b01000100;
		_delay_ms(50);
		PORTD = 0b00001100;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b01000100;
		_delay_ms(50);
		PORTD = 0b00001100;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b01000100;
		_delay_ms(50);
		
	}
	
	if (recebido == '4' )// Voltar
	{
//		dutyCycle = 100;
		Velocidade = '4';
		
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00001100;
		_delay_ms(50);
		PORTD = 0b00000100;
		_delay_ms(50);
		
	}
	
	USART_Transmit(recebido);
}

// Função para inicialização da USART
void USART_Init(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8); //Ajusta a taxa de transmissão
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0); //Habilita o transmissor e o receptor
	UCSR0C = (1<<USBS0)|(3<<UCSZ00); //Ajusta o formato do frame: 8 bits de dados e 2 de parada
	
}

// Função para envio de um frame de 5 a 8bits
void USART_Transmit(unsigned char data)
{
	while(!( UCSR0A & (1<<UDRE0)));//Espera a limpeza do registr. de transmissão
	UDR0 = data; //Coloca o dado no registrador e o envia
}

// Função para recepção de um frame de 5 a 8bits
unsigned char USART_Receive(void)
{
	while(!(UCSR0A & (1<<RXC0))); //Espera o dado ser recebido
	return UDR0; //Lê o dado recebido e retorna
}
void main(void)
{
	//GPIO
	DDRC = 0xFF; //Define a porta C como saída
	DDRB  = 0xFF; //Define a porta B como saída
	DDRD =	0b01111000; //PD6 saída
	PORTD = 0b00000100; //Todos os pull-ups da porta D habilitados

	//Configuração das interrupções
	EICRA = 0b00001010;//interrupção externa INT0 e INT1 na borda de descida
	EIMSK = 0b00000011;//habilita a interrupção externa INT0 e INT1	

	TCCR0A = 0b10000011; //PWM não invertido nos pinos OC0A 
	TCCR0B = 0b00000101; //frequencia em 61Hz
	OCR0A = 0; //controle do ciclo ativo do PWM
	sei();
	
	USART_Init(MYUBRR);
	nokia_lcd_init(); //Inicia o LCD
	
	while(1)
	{
	
	Menu();	
	OCR0A = (dutyCycle*255)/100.0;
	
	}
}
