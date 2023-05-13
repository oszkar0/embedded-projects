/* Oskar Szysiak WCY21IY4S1 */
#include <REGX52.H>

//ustawienie lini Strobe oraz ACK w obu 
//mikrokontrolerach (master i slave)
sbit Strobe_Master 	= P0^6;
sbit ACK_Master 		= P0^7;
sbit Strobe_Slave 	= P0^4;
sbit ACK_Slave 			= P0^5;

//deklaracja liczby, która bedzie odbierana
//w slavie, a wysylana i inkrementowana w
//masterze
unsigned char byte 	= 0x00;
//bit w masterze, ktory jest zmieniany w 
//przerwaniu po inkrementacji, ktory
//po ustawieniu na 0 sprawia ze master, zmienia wartosc
//na wyswietlaczu
bit bitMaster = 1;


//funkcja wait, ktora przez wykonywanie pewnych operacji arytemtycznych
//powoduje opoznienie
void wait(int i){
	unsigned int k, l, m;
	
	for(l = 0; l < i; l++){
		k = 500;
		m = 1000;
		k = m * l;
	}
}

//funkcja inicjujaca
//
void initInt0(){
	byte = 0; //zerujemy zmienna byte
	IT0 = 1;	//INT0 aktywne 0
	EX0 = 1;  //Wlaczenia INT0
	EA = 1;		//globalny wlacznik przerwan
}

//funkcja obslugi przerwania wywolanego 
//pod wplywem wcisniecia przycisku
void increment() interrupt 0{
	EX0 = 0; //wylaczamy przerwania
	byte++;		//inkrementujemy nasza zmienna
	EX0 = 1;	//wlaczamy przerwania
	bitMaster = 0;	//zmieniamy bit na 0 zeby przejsc do wyswietlenia zmiennej na ekran
}

//funkcja wysylajaca bajt do mikrokontrolera slave
void sendByte(unsigned char byte){
	//czekamy dopoki jeszcze slave ma ustawione 0 na lini potwierdzenia, poniewaz to
	//dotyczy poprzedniego odebrania bajtu
	while(ACK_Master == 0){ ; }  			
	P2 = byte;	//jak juz mozna wysylac bo slave zmienil ack, to wysylamy portem P2
	Strobe_Master = 1; //zapobiegawczo ustawiamy 1 na lini strobe
	wait(1);	//odczekujemy chwile
	Strobe_Master = 0;	//ustawiamy 0 sygnalizujac ze sa wyslane dane
	while(ACK_Master == 1){ ; } //czekamy az zostanie ustawione potwierdzenie przez slave
	Strobe_Master = 1;	// ustawiamy strobe spowrotem na 1, poniewaz dane juz sa odebrane
	P2 = 0xFF; //usuwamy dane z portu P2
}

//funkcja odbierajaca bajt
void receiveByte(){
	while(Strobe_Slave == 1){ ; } //czekamy na informacje ze sa dane wyslane
	byte = P2; //odbieramy bajt 
	ACK_Slave = 0; //informujemy mastera ze odebralismy bajt
	while(Strobe_Slave == 0){ ; } //czekamy az master zmieni linie Strobe
	ACK_Slave = 1;	//zmieniamy spowrotem linie informacji o odebraniu bajtu
}



void main(void){
	initInt0(); //wywolanie funkcji inicjujacej

	P1 					= 0;	//zerujemy wswietlacz
	ACK_Slave 	= 1;	//ustawiamy wyjscie ack slavea
	Strobe_Master 	= 1; //ustawiamy linie strobe mastera
	
	//master
	while(P0_0 == 1){
		while(bitMaster){ wait(1); }; //czekamy az zmienna zostanie zainkrementowana i w funkcji oblsugi zostanie zmieniony nasz bit
		P1 = byte;	//ustawiamy wartosc na wyswietlacz
		sendByte(byte);	//wywolujemy funkcje wysylania
		bitMaster = 1;	//zmieniamy spowrotem bit zeby mikrokontroler czekal na ponowna inkrementacje
	}
	
	//slave
	while(P0_0 == 0){
		receiveByte(); //wywolanie funkcji obierajacej bajt
		P1 = byte;	//ustawienie odebranej wartosci na wyswietlacz
	}
}



