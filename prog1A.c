#include <REGX52.H>

void delay( void )
{
   unsigned char j;
   unsigned char i;
   for(i=0;i<100;i++)
   {
      for(j=0;j<255;j++)
      { ; } 
   }
}

unsigned char patterns[] = { 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F}; 

void main( void )
{	
	unsigned char iterator;
	P2=0xFF;
	while( 1 )
	{
		for( iterator = 0; iterator < 8; iterator++ )
		{
			P2 = patterns[iterator];
			delay();
		}
	}
}