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
#include <util/delay.h> // Incluir Delay
#include <avr/interrupt.h> // Incluir Interrupções
#include "nokia5110.h" // Biblioteca do display nokia
#include <avr/eeprom.h> // Incluir EEPROM

// Variáveis Globais
#define MIN 0.5
#define MAX 99.9

float dutyCycle;
char Status = ' ', Velocidade = ' ',Brilho = ' ', recebido;
int cont;


// Menu apresentado no display
void Menu()
{
	nokia_lcd_clear(); // Limpar o display
	nokia_lcd_set_cursor(0,0);
	nokia_lcd_write_string("MedCar",2);
	nokia_lcd_set_cursor(0,20);
	nokia_lcd_write_string("Local: ",1);
	nokia_lcd_write_char(Velocidade,1);
	nokia_lcd_set_cursor(0,30);
	nokia_lcd_write_string("Modo: ",1);
	nokia_lcd_write_char(Status,1);
	nokia_lcd_set_cursor(0,40);
	nokia_lcd_write_string("Sensor: ",1);
	nokia_lcd_set_cursor(50,40);
	nokia_lcd_write_string("OFF ",1);
	nokia_lcd_render(); // Atualiza o display
	
}

ISR(INT0_vect) // Sensor ultrassônico
{
	Menu();
	nokia_lcd_set_cursor(50,40);
	nokia_lcd_write_string("ON ",1);
	nokia_lcd_render();
	_delay_ms(5000);
	
	PORTD = 0b00000100;	// Interrompe os motor no tempo do delay
}

ISR(USART_RX_vect)
{
	recebido = UDR0;
	
	// Modo do MedCar (Ativado ou Desativado)
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
	
	// Local que o MedCar foi solicitado
	
	if (recebido == '1' )// Local 1
	{
		Velocidade = '1';
		
		PORTD = 0b00001100;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b10000100;
		_delay_ms(50);

	}
	
	if (recebido == '2' )// Local 2
	{

		Velocidade = '2';
		
		PORTD = 0b00001100;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b10000100;
		_delay_ms(50);
		PORTD = 0b00001100;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b10000100;
		_delay_ms(50);
		
	}
	
	if (recebido == '3' )// Local 3
	{

		Velocidade = '3';
		
		PORTD = 0b00001100;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b10000100;
		_delay_ms(50);
		PORTD = 0b00001100;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b10000100;
		_delay_ms(50);
		PORTD = 0b00001100;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b10000100;
		_delay_ms(50);
		
	}
	
	if (recebido == '4' )// Retorne
	{

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
	
	// Iluminação Corredores
	
	if (recebido == 'm' )
	{
		dutyCycle = MIN;
		Brilho = '0';
	}
	
	if (recebido == 't' )
	{
		dutyCycle = 50;
		Brilho = '1';
	}
	
	if (recebido == 'n' )
	{
		dutyCycle = MAX;
		Brilho = '2';
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
	while(!( UCSR0A & (1<<UDRE0)));//Espera a limpeza do registrador de transmissão
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
	DDRB  = 0xFF; //Define a porta B como saída
	DDRC  = 0xFF;
	DDRD =	0b11111000; //PD saídas
	PORTD = 0b00000100; //Habilitação do pull-up

	//Configuração das interrupções
	EICRA = 0b00000010;//interrupção externa INT0 na borda de descida
	EIMSK = 0b00000001;//habilita a interrupção externa INT0

	TCCR0A = 0b10000011; //PWM não invertido nos pinos OC0A
	TCCR0B = 0b00000101; //frequencia em 61Hz
	OCR0A = 0; //controle do ciclo ativo do PWM
	
	
	USART_Init(MYUBRR);
	nokia_lcd_init(); //Inicia o LCD
	
	//EEPROM
	char R_array[15],W_array[15] = "DADOS";
	
	eeprom_write_block(W_array,0,strlen(W_array)); // Escrever no endereço 0 do EEPROM

	sei();
	
	while(1)
	{
	
		Menu();	
		OCR0A = (dutyCycle*255)/100.0;
	
		// Registro de dados de solicitação no EEPROM
		if(recebido = '1')
		{
			cont ++;
			_delay_ms(50);
			eeprom_write_block(W_array,0,strlen(W_array));
		}
		if(recebido = '2')
		{
			cont ++;;
			_delay_ms(50);
			eeprom_write_block(W_array,0,strlen(W_array));
		}
		if(recebido = '3')
		{
			cont ++;
			_delay_ms(50);
			eeprom_write_block(W_array,0,strlen(W_array));
		}
		if(recebido = '4')
		{
			cont ++;
			_delay_ms(50);
			eeprom_write_block(W_array,0,strlen(W_array));
		}
	
	}
}
