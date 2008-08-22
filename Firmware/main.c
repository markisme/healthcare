#include <iom128.h>
#define sbi(PORTX,BitX) PORTX |= (1 << BitX)
#define cbi(PORTX,BitX) PORTX &= ~(1 << BitX)
void Delay_us(unsigned int dly);
void Delay_ms(unsigned int dly);
unsigned char Uart_Getch(void);
void Uart_Putch(unsigned char PutData);
void ADC_init(void);
void Uart_init(void);
unsigned int Read_ADC(unsigned char ADC_input);
void  Uart_U16Bit_PutNum(unsigned int NumData);
void Timer_init(void);

unsigned int cnt=0, temp=0;

#pragma vector=TIMER0_OVF_vect
__interrupt void TIMER0_OVF_Interrupt(void)
{
  cnt++;
  if (cnt == 10) // 5ms
  {
    temp = Read_ADC(1);
    Uart_U16Bit_PutNum(temp);
    Uart_Putch(' ');
    cnt = 0;
  }
  TCNT0=6;
}
//TCNT0 = 131~255 반복


void main(void)
{
  ADC_init();
  Uart_init();
  Timer_init();
  while(1){
    //temp = Read_ADC(Uart_Getch()-48);
    //temp = Read_ADC(1);
    //Uart_U16Bit_PutNum(temp);
    //Uart_Putch(' ');
    //Delay_us(250);
    //Delay_us(250);
    //Delay_us(250);
  }
}


void Timer_init()
{
  SREG &= ~(1<<7);
  TCCR0 |= 0x03; // 32 분주  16000000/32 = 500000
  TIMSK |= 0x01; // Timer0 overflow interrupt mode
  TCNT0 = 6;
  SREG |= 0x80;  
}

void Uart_init()
{
  SREG &= ~(1<<7);
  UBRR0H = 0;
  UBRR0L = 0x67;
  UCSR0A = 0x20;
  UCSR0B = 0x18;
  UCSR0C = 0x06;
  DDRE &= ~(1<<0);
  DDRE |= (1<<1);
  SREG |= (1<<7);
}

void ADC_init()
{
  SREG &= ~(1<<7);
  ADMUX = 0x00; // 
  ADCSRA |= ((1<<7) | (1<<5)); // ADC활성화,ADC프리런닝모드
  //ADCSRA |= (1<<7); // ADC활성화
  //ADCSRA |= 0x07; // 분주비 128
  ADCSRA |= 0x04;   //분주비 64
  DDRF &= ~(1<<1);  
  SREG |= (1<<7);
}

unsigned int Read_ADC(unsigned char ADC_input)
{
  ADMUX = ADC_input;
  ADCSRA |= (1<<6); //ADC start
  while((ADCSRA & 0x10) == 0);//조건을 만족하면 돌겟지
  return ADC;  
}
unsigned char Uart_Getch(void)
{
  while(!(UCSR0A & (1<<RXC0)) );
  return UDR0;  
}
void Uart_Putch(unsigned char PutData)
{
  SREG &= ~(1<<7);
  while(!(UCSR0A & (1<<UDRE0)));
  UDR0=PutData;
  SREG |= (1<<7);
}

void  Uart_U16Bit_PutNum(unsigned int NumData)
{
  SREG &= ~(1<<7);
  unsigned int TempData;
  TempData = NumData/10000;
  Uart_Putch(TempData+48);
  TempData = (NumData%10000)/1000;
  Uart_Putch(TempData+48);
  TempData = (NumData%1000)/100;
  Uart_Putch(TempData+48);
  TempData = (NumData%100)/10;
  Uart_Putch(TempData+48);
  TempData = NumData%10;
  Uart_Putch(TempData+48);
  SREG |= (1<<7);  
}

void Delay_us(unsigned int dly)
{
     unsigned int i;

     for(i = 0; i < dly; i++)			// 4 cycle +
     {
          asm(" PUSH  R0 ");	// 2 cycle +
          asm(" POP   R0 ");	// 2 cycle +
          asm(" PUSH  R0 ");	// 2 cycle +
          asm(" POP   R0 ");	// 2 cycle +
          asm(" PUSH  R0 ");	// 2 cycle +
          asm(" POP   R0 ");	// 2 cycle = 16 cycle = 1 us for 16MHz
     }
}

void Delay_ms(unsigned int dly)
{
    unsigned int i;

    for(i = 0; i < dly; i++)
    {
          Delay_us(250);
          Delay_us(250);
          Delay_us(250);
          Delay_us(250);
    }
}
