#include <avr/io.h> 

void ADC_init()
{
  ADMUX = (1 << REFS0) | (1 << ADLAR);  // For Aref=AVcc;
  ADCSRA = (1<<ADEN)|(5<<ADPS0); //Prescalar div factor = 32
}

uint16_t ReadADC(uint8_t ch)
{
  ch = ch & 0x07;
  ADMUX |= ch;

  //Start Single conversion
  ADCSRA |= (1<<ADSC);

  //Wait for conversion to complete
  while(!(ADCSRA & (1<<ADIF)));

  //Clear ADIF by writing one to it
  ADCSRA|=(1<<ADIF);

  return(ADCH);
}

void UART_init()
{
  UBRRL = 0x67;

  /* Enable receiver and transmitter */
  UCSRB = (1<<RXEN)|(1<<TXEN);

  /* Set frame format: 8data, 1stop bit */
  UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

void UART_TX( unsigned char data )
{

  /* Wait for empty transmit buffer */
  while ( !( UCSRA & (1<<UDRE)) );

  /* Put data into buffer, sends the data */
  UDR = data;
}

int main()
{	
	UART_init();
	ADC_init();

	while(1){
		unsigned char read = ReadADC(0);
		unsigned char  i=100;
		while(i != 0){
			UART_TX((read / i) + 48);
			read = read %i;
			i = i/10;
		}
		UART_TX('\n');
		UART_TX('\r');
	}
}
