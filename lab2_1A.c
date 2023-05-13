#include <REGX52.H>

void delay(void){
	unsigned char j;
	unsigned char i;
	for(i = 0; i < 100; i++){
		for(j = 0; j < 100; j++){
			;
		}
	}
}

unsigned char code koder[] = {
							0x01, 0x03, 0x05, 0x07, 
							0x09, 0x11, 0x13, 0x15};

void main(void){
	unsigned char zmiennaAA = 0;

	while(1){
		if(zmiennaAA == 0){
			P2 = 0xFF;
		} else if(zmiennaAA % 2 == 1 && zmiennaAA > 0){
			P2 = koder[zmiennaAA/2];
		}
		
		if(zmiennaAA < 16){
			zmiennaAA++;
		} else {
			zmiennaAA = 0;
		}
		delay();
	}
}