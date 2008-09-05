#include <WinAvr\avr\include\avr\io.h>
#include <avr/signal.h>
#include <math.h>
#include <avr/interrupt.h>

#define   SYSTEM_CLOCK  16000000   // CLOCK (X-tal frequency)
#define   MAX_ADC_SUM_COUNTER 64
#define   TMP_HIGH 9
#define   TMP_LOW 10
#define   TMP_POINTF 12
#define   TMP_POINTS 13
#define   Beta 3435


typedef unsigned char BYTE;
//Global Data array for displaying 16x2 lcd.
volatile char NEWRPM=0,RPM_DISPLAY[16]={'F','A','N',32,'S','P','D',':',32,48,48, 48,48,'R','P','M'};
volatile char TMP_DISPLAY[16]={'F','A','N',32,'T','M','P',':',32,0x00,0x00,'.', 0x00,0x00,0xdf,'C'};
volatile unsigned int FAN_T=0,RPM;
volatile unsigned char  ADC_Complete=0,run=0,stop=0 ;
volatile unsigned int ADC_Sum_Counter=0;
volatile unsigned long ADC_Sum=0;

// from AVR ATmega128 ������
void Delay_us(unsigned char time_us)
{
  register unsigned char i;
  for(i = 0; i < time_us; i++)
  {
    asm volatile(" PUSH R0 ");
    asm volatile(" POP R0 ");
    asm volatile(" PUSH R0 ");
    asm volatile(" POP R0 ");
    asm volatile(" PUSH R0 ");
    asm volatile(" POP R0 ");
  }
}

// from AVR ATmega128 ������
void Delay_ms(unsigned int time_ms)
{
  register unsigned int i;
  for(i = 0; i < time_ms; i++)
  {
    Delay_us(250);
    Delay_us(250);
    Delay_us(250);
    Delay_us(250);
  }
}
void port_init(void)
{
	DDRB=0Xff;  //portb 0~2 control line for lcd and 3 control line for motor 6~7 for crystal
        DDRD=0x3f;  //portd 0~5 data line for lcd and 6~7 sensing line for motor
        DDRC=0xfe; //portc 0 for adc, 1~2 data line for lcd
      
        PORTB=0x00;
	PORTD=0x00;
        PORTC=0x00;
}

void lcd_control(char c)
{
	 PORTB = (PORTB&0xf8) | 0x04;      //make high pin E
         PORTD = (c & 0x3f);     //output data
         PORTC = (c & 0xc0)>>5;     //output data
	 Delay_ms(1);    //delay
	 PORTB = (PORTB&0xf8) |0x00;      //make low pin E
}

void lcd_display(char c)
{
	PORTB = (PORTB&0xf8)|0x05;     //make high pin E & RS
	PORTD = (c & 0x3f);     //output data
        PORTC = (c & 0xc0)>>5;     //output data
	 Delay_ms(1);    //delay
	PORTB = (PORTB&0xf8)|0x01;     //make low pin E
}

void lcd_position(unsigned char x, unsigned char y)
{
	if(y>1) y=1; if(x>15) x=15;   //you have a 1*2 line character lcd.  x = 0, y = 0 to 1
	if(y==0) lcd_control(x+0x80); //first line address(0,0) is 0x80
	else lcd_control(x+0xc0);     //second line address(0,1) is 0xc0
}

void lcd_display_string(char x, char y, char string[])
{
	lcd_position(x,y);            //select lcd position
	while(*string) lcd_display(*string++); //for example, if string is "han"
}                                              //h,a,n's address is 0,1,2
                                               //you didn`t have 3 and more addresses
                                               //so while must return "0" => the end

void lcd_init(void)
{
	lcd_control(0x38);
	lcd_control(0x0e);
	lcd_control(0x02);
	lcd_control(0x01);
}

SIGNAL(SIG_INPUT_CAPTURE1)
{ FAN_T = ICR1L;
  FAN_T |= (ICR1H<<8);
  TCNT1H=0X00;
  TCNT1L=0X00;
  // the frequency of sensing signal is (1/FAN_T*64us) because of timer prescaler setting(1024)
  // frequency/3 is the number of turns for single seconds
  // so, RPM = the number of turns * 60 , that is frequency*15
  RPM=(15625/FAN_T)*20;
  NEWRPM=1;
}

SIGNAL(SIG_OVERFLOW1)
{    TCNT1H=0X00;
     TCNT1L=0X00;
     stop=1;
}
void ADC_Init(uint8_t channel)
{
  // REFS1 REFS0 = 00 => VREF�� ���� ���� ����
  // REFS1 REFS0 = 01 => AVCC�� ���� ���� ����
  // REFS1 REFS0 = 11 => ���� 2.56V�� ���� ���� ����
  // �ܺο��� VREF�� ������ �����ϸ� ������ VREF�� ���� ����
  // UST ATmega128 board�� ���� VREF�� ����Ǿ� �ִ�
  // ADLAR = 0 => ADCH:ADCL ������ ����
  // MUX4 - MUX0 = channel => analog input ����
  ADMUX = channel;
  ADMUX |= _BV(REFS1) | _BV(REFS0);
  // ADEN = 1 => ADC enable
  // ADSC = 1 => ADC start conversion
  // ADFR = 0 => single conversion mode => �ѹ��� ADC
  // => interrupt �� �ٽ� A/D ��ȯ�� �Ϸ��� ADSC bit�� 1�� �����ؾ� ��
  // ADIE = 1 => ADC interrupt enable
  // ADPS2 - ADPS0 = 001 => A/D ��ȯ Ŭ���� ���ֺ� = 2
  ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS0);
  //ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADFR) | _BV(ADIE) | _BV(ADPS0);
}

// A/D conversion�� ����Ǹ� �߻��ϴ� interrupt service routine
SIGNAL(SIG_ADC)
{
  unsigned char ADC_L, ADC_H;
  // read ADCL first
  ADC_L = ADCL;
  // read ADCH => ������ ���Ľ������Ƿ� ���� 2��Ʈ�� ��ȿ
  ADC_H = ADCH & 0x03;
  // A/D ��ȯ�� 10��Ʈ���� int type ���� ADD_Sum�� ���Ѵ�
  ADC_Sum += ADC_H*256 + ADC_L;
  // ���� Ƚ���� 1 ����
  ADC_Sum_Counter++;
  // ���� Ƚ���� ������ ������ ���� ��� �ٽ� A/D conversion ����
  if(ADC_Sum_Counter < MAX_ADC_SUM_COUNTER)
    sbi(ADCSRA,ADSC); // A/D conversion start
}


void TC1_Init(void)
{
  // 16��Ʈ timer/counter 1�� ����ؼ� 1Hz ���� ���ͷ�Ʈ �߻���Ų��.
  // timer/counter 1 output compare match interrupt ���� : 1 Hz
  // WGM(Waveform Generation Mode) 4 ��Ʈ => ���� ��� ����
  // WGM13 WGM12 WGM11 WGM10 = 0100 => OCR1A(Output Compare Regiser) ����
  // TOP(timer/counter1�� �ִ밪)�� �Ǵ� CTC ���
  // CTC(Clear Timer on Compare Match) mode������ timer/counter 1�� ��
  // Ȥ�� TCNT1 register�� ���� OCR1A(WGM=0100) Ȥ�� ICR1(WGM=1100) register��
  // ���� �������� output compare match interrupt �߻��Ѵ�.
  //
  // COM(Compare Match Output Mode) 2 ��Ʈ => OC(Output Compare) pin ����
  // COM1A1 COM1A0 = 00 => OC1A pin�� I/O pin PB5 pin���� ����ϵ��� ����
  //
  // CS(Clock Select) 3 ��Ʈ => clock ���ֺ� ����
  // CS12 CS11 CS10 = 001 : 1 ����
  // CS12 CS11 CS10 = 010 : 8 ����
  // CS12 CS11 CS10 = 011 : 64 ����
  // CS12 CS11 CS10 = 100 : 256 ����
  // CS12 CS11 CS10 = 101 : 1024 ����
  //
  // ���ͷ�Ʈ �߻� ���ļ� f = 16MHz / (���ֺ�*(1+N))
  // f = 1Hz, ���ֺ� = 256 => 1 = 16MHz / (256*(1+N))
  // => N = (16MHz / 256) - 1 = 62499
  // => 62499�� OCR1A register�� �����Ѵ�.
  // OCR1A register�� 16��Ʈ register��
  // 16 ��Ʈ register�� ����� ���� �����Ͽ��� �Ѵ�.
  // 16 ��Ʈ register read : low byte�� ���� �о�� �Ѵ�.
  // 16 ��Ʈ register write : high byte�� ���� ��� �Ѵ�.
  // TCCR(Timer/Counter Control Register)�� ���ؼ� ���� ���� ����
  // WGM13 WGM12 WGM11 WGM10 = 0100
  // CS12 CS11 CS10 = 100
  // COM1A1 COM1A0 = 00
  TCCR1A = 0x00;
  TCCR1B |= _BV(ICNC1) | _BV(ICES1) | _BV(CS12) | _BV(CS10); //1024����
  sbi(TIMSK, TICIE1);
  sbi(TIMSK, TOIE1);
  sbi(ACSR,ACIC);
}

int main(void)
{    double Vin,R,Temperature,Current;
     port_init();
     ADC_Init(0);
     TC1_Init();
     lcd_init();

  sbi(ADCSRA,ADSC); // A/D conversion start
  sei();
  int i,tmp_TMP;
  lcd_control(0x01);
      lcd_position(0,0);
      for(i=0;i<16;i++)
      {lcd_display(RPM_DISPLAY[i]);}
  for(;;)
  {
   if(ADC_Sum_Counter == MAX_ADC_SUM_COUNTER)
    { Vin=((double)(ADC_Sum>>6)*2.56)/(1024);
      Current=Vin/2955;
      R=(5-Vin)/Current;
       Temperature=100*((298.15*Beta/(298.15*log(R/10000)+Beta))-(273.15));
      //Temperature=(10*298.15*Beta/(298.15*log(R/10000)+Beta)-273.15+0.5);
      //Temperature=10*(298.15*Beta/(298.15*(R/10000)+Beta)-273.15+0.5);
      tmp_TMP=floor(Temperature);
      TMP_DISPLAY[TMP_HIGH] =(tmp_TMP/1000)+48;
      TMP_DISPLAY[TMP_LOW] =((tmp_TMP/100)%10)+48;
      TMP_DISPLAY[TMP_POINTF] =((tmp_TMP%100)/10)+48;
      TMP_DISPLAY[TMP_POINTS] =((tmp_TMP%100)%10)+48;
      lcd_position(0,1);
      for(i=0;i<16;i++)
      lcd_display(TMP_DISPLAY[i]);
      ADC_Sum_Counter=0;
      ADC_Sum=0;
      if(tmp_TMP>=2500)
       {sbi(PORTB,3);
        run=1;
        Delay_ms(1000);}
     else{
       if((tmp_TMP>=2500&&tmp_TMP<=2800) && run)
        {cbi(PORTB,3);
      Delay_ms(100);}// end of if
     else{  if(stop || !(run)) {
     RPM_DISPLAY[9]= 48;
     RPM_DISPLAY[10]=48;
     RPM_DISPLAY[11]=48;
     RPM_DISPLAY[12]=48;
      lcd_position(0,0);
      for(i=0;i<16;i++)
      {lcd_display(RPM_DISPLAY[i]);}
      NEWRPM=0;
      stop=0;}
       }// end of if
      }// end of else
       sbi(ADCSRA,ADSC); // A/D conversion start
    }//end of else

    if(NEWRPM)
    {
     RPM_DISPLAY[9]= (RPM/1000)+48;
     RPM_DISPLAY[10]=((RPM/100)%10)+48;
     RPM_DISPLAY[11]=((RPM/10)%10)+48;
     RPM_DISPLAY[12]=(RPM%10)+48;
      lcd_position(0,0);
      for(i=0;i<16;i++)
      {lcd_display(RPM_DISPLAY[i]);}
      NEWRPM=0;
     }
    }
   }

