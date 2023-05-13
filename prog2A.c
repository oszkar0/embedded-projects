#include <REGX52.H>

void delay( void )
{
   unsigned char j;
   unsigned char i;
 
   for(i=0;i<100;i++)
   {
      for(j=0;j<255;j++)
      {;}
   }
}


unsigned char patterns[] = { 0x3F, 0x9F, 0xCF, 0xE7, 0xF3, 0xF9, 0xFC };

void main( void )
{
	unsigned char iterator;
	P2 = 0xFF;
	while(1)
	{
		for(iterator = 0; iterator < 7; iterator++)
		{
			P2 = patterns[iterator];
			if( iterator % 2 == 0 ){ delay(); }
			else { delay(); delay(); }
		} 
	}
}