#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU/16/BAUD) - 1)

#include <avr/io.h>
#include <string.h>

char c;
char buff[32];
int len = 0;

void uart_init(void) {
    UBRR0H = (UBRR_VALUE >> 8);
    UBRR0L = UBRR_VALUE;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_rx(){


while(1){

while(!(UCSR0A & (1 << RXC0)));
c = UDR0;


if(c == '\r' || c == '\n')
break;

if(len < 31)
buff[len++] = c;

}
buff[len] = '\0';
len = 0;
}

void uart_tx(char k){

while(!(UCSR0A & (1 << UDRE0)));

UDR0 = k;

}

void uart_commands(const char *str){
  while(*str){
    uart_tx(*str++);
   
    }
  
  }


void compare(void) {
    if (strcmp(buff, "led") == 0) {
        PORTB ^= (1 << PB5);
    }
}



int main(void) {
    uart_init();

DDRB |= (1 << PB5);
uart_commands("enter just led to toggle it on or off fucker \r\n");

while (1)
{
uart_rx();
compare();

}

    return 0;
}
