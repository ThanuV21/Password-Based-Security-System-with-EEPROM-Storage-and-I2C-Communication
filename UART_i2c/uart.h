#include<lpc21xx.h>
#include<stdio.h>
#define lcd_d 0xf<<20
#define rs 1 << 17
#define rw 1<<18
#define e 1 << 19
void delay_ms(unsigned int ms)
{
	T0PR=15000-1;
	T0TCR=0x01;
	while(T0TC<ms);
	T0TCR=0x03;
	T0TCR=0x00;
}


void LCD_command(unsigned char cmd)
{
	IOCLR1=lcd_d;
	IOSET1=(cmd&0XF0)<<16;
	IOCLR1=rs;
	IOSET1=e;
	delay_ms(2);
	IOCLR1=e;

	IOCLR1=lcd_d;
	IOSET1=(cmd&0X0f)<<20;
	IOCLR1=rs;
	IOSET1=e;
	delay_ms(2);
	IOCLR1=e;
}
void LCD_data(unsigned char d)
{
	IOCLR1=lcd_d;
	IOSET1=(d&0XF0)<<16;
	IOSET1=rs;
	IOSET1=e;
	delay_ms(2);
	IOCLR1=e;

	IOCLR1=lcd_d;
	IOSET1=(d&0X0F)<<20;
	IOSET1=rs;
	IOSET1=e;
	delay_ms(2);
	IOCLR1=e;
}
void LCD_init(void)
{
	IODIR1|= lcd_d|rs|e|rw;
	IOCLR1=rw;
	LCD_command(0x01);
	LCD_command(0x02);
	LCD_command(0x0c);
	LCD_command(0x28);
	LCD_command(0x80);
}
void LCD_int(int n)
{
	unsigned char arr[5];
	signed char i=0;
	if(n==0)
		LCD_data('0');
	else
		{
			if(n<0)
			{
				LCD_data('-');
				n=-n;
			}
			while(n>0)
			{
				arr[i++]=n%10;
				n=n/10;
			}
			for(i=i-1;i>=0;i--)
				LCD_data(arr[i]+48);
		}
}
void LCD_str(unsigned char *s)
{
	int i=0;
	for(i=0;s[i];i++)
		LCD_data(s[i]);
		
}

void LCD_scroll(unsigned char *s)
{
	unsigned int i=0x80;
	while(1)
	{
		LCD_command(0x01);
		LCD_command(i++);
		LCD_str(s);
		delay_ms(300);
		if(i==0x91)
		{
			break;
		}
	}
}
void uart0_config(void)
{
	PINSEL0|=0x00000005;
	U0LCR = 0x83;
	U0DLL = 97;
	U0DLM = 0;
	U0LCR = 0x03;
}

void uart0_tx(unsigned char ch)
{	
	U0THR = ch;
	while((U0LSR>>5&1)==0); 
}

unsigned char uart0_rx(void)
{
	while((U0LSR&1)==0);
	return U0RBR;
}

void uart0_tx_str(unsigned char *s)
{
	while(*s)
	uart0_tx(*s++);
}

int uart0_rx_ready() {
    return (U0LSR & 0x01); // Checks if the Receive Data Ready (RDR) bit is set
}

// Function to clear any residual data in UART buffer
void uart_clear_buffer() {
    while (uart0_rx_ready()) uart0_rx();
}

// Function to receive a string from UART
void uart0_rx_str(unsigned char *buffer, int length) {
    int i = 0;
    while (i < length - 1) {
        char c = uart0_rx(); // Receive a single character
        if (c == '\r' || c == '\n') { // Handle end of line
            break;
        }
        buffer[i++] = c;
    }
    buffer[i] = '\0'; // Null-terminate the string
}




