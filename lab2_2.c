#include <REGX52.H>

unsigned char code koder[] = { 0x11, 0x13, 0x15, 0x17, 0x19 };

void main(void){
	unsigned char zmiennaAA = 0;
	
	while(1){
			if(zmiennaAA == 0){
				P2 = 0xFF;
			} else if (zmiennaAA > 0 && zmiennaAA % 2 == 1){
				P2 = koder[(zmiennaAA % 10)/2];
			}
			
			while(P3_2 == 1){
				;
			}				
			while(P3_2 == 0){
				;
			}
			
			if(zmiennaAA < 16){
				zmiennaAA++;
			} else {
				zmiennaAA = 0;
			}
	}
}