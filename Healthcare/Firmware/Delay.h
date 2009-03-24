void Delay_us(unsigned int dly);
void Delay_ms(unsigned int dly);

void Delay_us(unsigned int dly)
{
     unsigned int i;

     for(i = 0; i < dly; i++)			// 4 cycle +
     {
          asm volatile(" PUSH  R0 ");	// 2 cycle +
          asm volatile(" POP   R0 ");	// 2 cycle +
          asm volatile(" PUSH  R0 ");	// 2 cycle +
          asm volatile(" POP   R0 ");	// 2 cycle +
          asm volatile(" PUSH  R0 ");	// 2 cycle +
          asm volatile(" POP   R0 ");	// 2 cycle = 16 cycle = 1 us for 16MHz
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
