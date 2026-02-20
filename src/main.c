#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU/16/BAUD) - 1)
#define BUFF_SIZE 32

#include <avr/io.h>
#include <string.h>

char rx_buffer[BUFF_SIZE];
volatile int head = 0;
volatile int tail = 0;

char buff[32];
int len = 0;

void uart_init(void) {
    UBRR0H = (UBRR_VALUE >> 8);
    UBRR0L = UBRR_VALUE;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
void uart_rx(){

if(UCSR0A & (1 << RXC0)){

char c = UDR0;

uart_tx(c);

uint8_t next = (head + 1) % BUFF_SIZE;

if(next != tail){

rx_buffer[head] = c;
head = next;
}

}

}

int uart_read(char *c){

if(head == tail) return 0;

*c = rx_buffer[tail];
tail = (tail + 1) % BUFF_SIZE;
return 1;

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
    if (strcmp(buff, "led on") == 0) {
        PORTB |= (1 << PB5);
    }
     if (strcmp(buff, "led off") == 0) {
        PORTB &= ~(1 << PB5);
    }
    if (strcmp(buff, "status") == 0) {
        if(PINB & (1 << PB5)){
          uart_commands("led is on right now\r\n");
          }
          else
          uart_commands("led is off right now\r\n");
    }
    
}



int main(void) {
    uart_init();

DDRB |= (1 << PB5);
uart_commands(" AVAIABLE COMMANDS \r\n");
uart_commands("led on/off  -controlls the led (pin 13)\r\n");
uart_commands("status      -get the current status of led \r\n");
uart_commands("help        -lorem ipsum \r\n");
while (1)
{
uart_rx();

char c;

while(uart_read(&c)){

if(c == '\r' || c == '\n'){

buff[len] = '\0';

compare();
len = 0;

}
else if(len < 31){

buff[len++] = c;

}




}
}

    return 0;
}
