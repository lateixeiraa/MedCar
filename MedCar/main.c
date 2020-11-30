/*
 * MedCar.c
 *
 * Created: 23/11/2020 10:22:43
 * Author : Larissa e Marina - PC
 */ 

#define F_CPU 16000000UL //Frequ√™ncia de trabalho da CPU
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#include <avr/io.h>
#include <util/delay.h> // Incluir Delay
#include <avr/interrupt.h> // Incluir Interrup√ß√µes
#include "nokia5110.h" // Biblioteca do display nokia
#include <avr/eeprom.h> // Incluir EEPROM

<<<<<<< HEAD
// Vari·veis Globais
#define MIN 10
=======
// Vari√°veis Globais
#define MIN 0.5
>>>>>>> 6121d803d0598de089a269980508221010f646b6
#define MAX 99.9
#define tam_vetor 4

float dutyCycle;
char Status = ' ', Velocidade = ' ',Brilho = ' ', recebido;
int cont;

// ConfiguraÁ„o ADC 

#define tam_vetor 4
unsigned char leitura_ADC_string[tam_vetor];
uint16_t leitura_ADC = 0;

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

ISR(INT0_vect) // Sensor ultrass√¥nico
{
	Menu();
	nokia_lcd_set_cursor(50,40);
	nokia_lcd_write_string("ON ",1);
	nokia_lcd_render();
	_delay_ms(5000);
	
<<<<<<< HEAD
	PORTD = 0b01000100;	// Interrompe os motor no tempo do delay
=======
	PORTD = 0b00000100;	// Interrompe os motores no tempo do delay
>>>>>>> 6121d803d0598de089a269980508221010f646b6
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
		
		PORTD = 0b00000101;
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
		
		PORTD = 0b00000101;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b10000100;
		_delay_ms(50);
		PORTD = 0b00000101;
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
		
		PORTD = 0b00000101;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b10000100;
		_delay_ms(50);
		PORTD = 0b00000101;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b10000100;
		_delay_ms(50);
		PORTD = 0b00000101;
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
		
		PORTD = 0b10000100;
		_delay_ms(50);
		PORTD = 0b00100100;
		_delay_ms(50);
		PORTD = 0b00010100;
		_delay_ms(50);
		PORTD = 0b00000101;
		_delay_ms(50);
		PORTD = 0b00000100;
		_delay_ms(50);
		
	}
	
	// Ilumina√ß√£o Corredores
	
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

// Fun√ß√£o para inicializa√ß√£o da USART
void USART_Init(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8); //Ajusta a taxa de transmiss√£o
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXCIE0)|(1<<RXEN0)|(1<<TXEN0); //Habilita o transmissor e o receptor
	UCSR0C = (1<<USBS0)|(3<<UCSZ00); //Ajusta o formato do frame: 8 bits de dados e 2 de parada
	
}

// Fun√ß√£o para envio de um frame de 5 a 8bits
void USART_Transmit(unsigned char data)
{
	while(!( UCSR0A & (1<<UDRE0)));//Espera a limpeza do registrador de transmiss√£o
	UDR0 = data; //Coloca o dado no registrador e o envia
}

// Fun√ß√£o para recep√ß√£o de um frame de 5 a 8bits
unsigned char USART_Receive(void)
{
	while(!(UCSR0A & (1<<RXC0))); //Espera o dado ser recebido
	return UDR0; //L√™ o dado recebido e retorna
}
void main(void)
{
	
	//GPIO
<<<<<<< HEAD
	DDRB  = 0xFF; //Define a porta B como saÌda
	DDRC =  0b11011111; // pc5 Entrada
	DDRD =	0b11111010; //PD saÌdas
	PORTD = 0b00000100; //HabilitaÁ„o do pull-up

	//ConfiguraÁ„o das interrupÁıes
	EICRA = 0b00000010;//interrupÁ„o externa INT0 na borda de descida
	EIMSK = 0b00000001;//habilita a interrupÁ„o externa INT0
	
	// PWM
	TCCR0A = 0b10000011; //PWM n„o invertido nos pinos OC0A
=======
	DDRB  = 0xFF; //Define a porta B como sa√≠da
	DDRC  = 0xFF;
	DDRD =	0b11111000; //PD sa√≠das
	PORTD = 0b00000100; //Habilita√ß√£o do pull-up

	//Configura√ß√£o das interrup√ß√µes
	EICRA = 0b00000010;//interrup√ß√£o externa INT0 na borda de descida
	EIMSK = 0b00000001;//habilita a interrup√ß√£o externa INT0

	TCCR0A = 0b10000011; //PWM n√£o invertido nos pinos OC0A
>>>>>>> 6121d803d0598de089a269980508221010f646b6
	TCCR0B = 0b00000101; //frequencia em 61Hz
	
	TCCR2A = 0b10100011;
	TCCR2B = 0b00000011;
	
	OCR0A = 0; //controle do ciclo ativo do PWM
	OCR2B = 0; //controle do ciclo ativo do PWM
	
	//CONFIGURA ADC
	ADMUX  = 0b00000101; // ADC5
	ADCSRA = 0b11101111;
	ADCSRB = 0x00;
	DIDR0  = 0b00111011;
	
	USART_Init(MYUBRR);
	nokia_lcd_init(); //Inicia o LCD
	
	//EEPROM
	char R_array[15],W_array[15] = "DADOS";
<<<<<<< HEAD
	eeprom_write_block(W_array,0,strlen(W_array)); // Escrever no endereÁo 0 do EEPROM
=======
	
	eeprom_write_block(W_array,0,strlen(W_array)); // Escrever no endere√ßo 0 do EEPROM
>>>>>>> 6121d803d0598de089a269980508221010f646b6

	sei();
	
	while(1)
	{
	
		Menu();	
		OCR0A = (dutyCycle*255)/100.0;
	
		// Registro de dados de solicita√ß√£o no EEPROM
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

ISR(ADC_vect)
{
	OCR2B = ADC/4; //PWM
	leitura_ADC = ADC;
}

//CONVERS√O INTEIRO PARA STRING
void int2string(unsigned int valor, unsigned char *disp)
{
	for(uint8_t n = 0; n < tam_vetor; n++)
	{
		disp[n]= 0 + 48;
		//limpa vetor para armazenagem dos digitos
	}
	disp += tam_vetor-1;
	do
	{
		*disp = (valor%10) + 48;
		//pega o resto da divis„o por 10
		valor /= 10; //pega o inteiro da div por 10
		
		disp--;
		
	}while(valor!=0);
}
