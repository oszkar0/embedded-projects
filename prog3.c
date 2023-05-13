#include <REGX52.H>

void delay(void)
{
   unsigned char j;
   unsigned char i;
   for(i=0;i<100;i++)
   {
      for(j=0;j<255;j++)
      { ; }
   }
} 

unsigned char patternsRtoL[] = { 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00 };
unsigned char patternsLtoR[] = { 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00 };

void main(void)
{
	unsigned char iterator;
	P2 = 0xFF;
	while(1) 
	{
		if( P3_2 == 1 )
		{
			for(iterator = 0; iterator < 8; iterator++)
			{
				P2 = patternsRtoL[iterator];
				delay();
				if(P3_2 == 0)
				{
					break;
				}
			}
			P2 = 0xFF;
			delay();
		}
		else
		{
			for(iterator = 0; iterator < 8; iterator++)
			{
				P2 = patternsLtoR[iterator];
				delay();
				if(P3_2 == 1)
				{
					break;
				}
			}
			P2 = 0xFF;
			delay();
		}
	}

}