#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU/16/BAUD) - 1)
#define BUFF_SIZE 32

#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>

volatile char rx_buffer[BUFF_SIZE];
volatile uint8_t head = 0;
volatile uint8_t tail = 0;

char buff[32];
uint8_t len = 0;

/* Function Prototypes */
void uart_init(void);
void uart_tx(char k);
int uart_read(char *c);
void uart_commands(const char *str);
void compare(void);

void uart_init(void) {
    UBRR0H = (UBRR_VALUE >> 8);
    UBRR0L = UBRR_VALUE;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

ISR(USART_RX_vect)
{
    char c = UDR0;

    uart_tx(c);  // echo

    uint8_t next = (head + 1) % BUFF_SIZE;

    if (next != tail)   // if buffer not full
    {
        rx_buffer[head] = c;
        head = next;
    }
}

int uart_read(char *c)
{
    if(head == tail) return 0;

    *c = rx_buffer[tail];
    tail = (tail + 1) % BUFF_SIZE;
    return 1;
}

void uart_tx(char k)
{
    while(!(UCSR0A & (1 << UDRE0)));
    UDR0 = k;
}

void uart_commands(const char *str)
{
    while(*str){
        uart_tx(*str++);
    }
}
#include <util/delay.h>

void blink_led(void)
{
    for (uint8_t i = 0; i < 3; i++)
    {
        PORTB |= (1 << PB5);
        _delay_ms(200);
        PORTB &= ~(1 << PB5);
        _delay_ms(200);
    }
}

void compare(void)
{
    if (strcmp(buff, "led on") == 0) {
        PORTB |= (1 << PB5);
    }

    if (strcmp(buff, "led off") == 0) {
        PORTB &= ~(1 << PB5);
    }

    if (strcmp(buff, "status") == 0) {
        if(PINB & (1 << PB5)){
            uart_commands("led is on right now\r\n");
        } else {
            uart_commands("led is off right now\r\n");
        }
    }

   if (strcmp(buff, "blink") == 0) {
    blink_led();
   }
}

int main(void)
{
    uart_init();
    sei();

    DDRB |= (1 << PB5);

    uart_commands(" AVAIABLE COMMANDS \r\n");
    uart_commands("led on/off  -controlls the led (pin 13)\r\n");
    uart_commands("status      -get the current status of led \r\n");
    uart_commands("help        -lorem ipsum \r\n");
    uart_commands("blink       -led blinks 3 times\r\n");
    while (1)
    {
        char c;

        while(uart_read(&c))
        {
            if(c == '\r' || c == '\n')
            {
                buff[len] = '\0';
                compare();
                len = 0;
            }
            else if(len < sizeof(buff) - 1)
            {
                buff[len++] = c;
            }
        }
    }

    return 0;
}
