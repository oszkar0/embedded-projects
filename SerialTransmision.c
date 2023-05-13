/*Oskar Szysiak WCY21IY4S1 Lab2 IKC*/
#include <REGX52.H>

sbit clockM = P2^0; //referencja na nozke zegarowa w mikrokontrolerze nadajacym
sbit dataM = P2^1; //referencja na nozke danych w mikrokontrolerze nadajacym
sbit clockS = P2^2; //referencja na nozke zegarowa w mikrokontrolerze odbierajacym
sbit dataS = P2^3; //referencja na nozke danych w mikrokontrolerze odbierajacym

//flaga zmieniajaca wartosc podczas przerwania wywolanego wcisnieciem
//przycisku, ktora przerywa czekanie mikrokontrolera i sprawia, ze rozpoczyna
//sie wysylanie bajtu + 3 bitow: startu parzystosci oraz stopu 
bit flag = 1;
//zmienna przekazywana do funkcji wait, ktora okresla ilosc 
//wykonanych przez petle iteracji (wieksza - dluzszy czas)
unsigned char waitTimeLong = 100;
//zmienna przekazywana do funkcji wait, ktora okresla ilosc 
//wykonanych przez petle iteracji (krotsza- krotszy czas)
unsigned char waitTimeShort = 10;
//zmienne przechowujace liczbe dla nadajnika i odbiornika
unsigned char numberM = 0;
unsigned char numberS = 0;


//funkcja zwracajaca bit parzystosci w naszym bajcie
//jesli jest parzysta liczba jedynek zwraca 0
//jesli nieprarzysta zwraca 1 
bit getParity(unsigned char byte)
 {
	bit parityBit = 0;
	while(byte)
	{
			parityBit = !parityBit;
			byte = byte & (byte - 1);
	}
	return parityBit;
}

//funkcja sprawiajaca ze wystepuje opoznienie
//wykonuja sie operacje ktore dla zadania nie maja znaczenia
//ale sprawiajaza ze mija pewien czas, tak zeby moc symolowac dzialanie 
//zegara 
void wait(unsigned char waitTime)
 {
	unsigned char k, l, m;
	for (l = 0; l < waitTime; l++)
	{
		k = 120;
		m = 255;
		k = m * l;
	}
 }

//funkcja obslugujaca przerwanie INT 0 (wcisniecie przycisku)
void buttonClickedInterrupt() interrupt 0
 {
 	EX0 = 0; //na chwile wylaczamy przerwanie;
	if(numberM == 255) { numberM = 0;} //jesli liczba jest rowna 255 to przy nastepnym przycisnieciu przekraczamy zakres,   
	else{ numberM++; }				   //jaki mozemy przechowac w zmiennej wiec musimy wyzerowac, a jak nie to mozemy inkrementowac
	EX0 = 1; //wlaczamy spowrotem przerwania 
	flag = 0; //zmieniamy flage na 0 zeby master przeszedl do wysylania 
 }

 void sendBytePlusControlBits(unsigned char byte)
 {
	unsigned char bitesOfNumberToSend = 8; //zmienna pomocnicza zebysmy wiedzieli ile potem zostalo bitow do wyslania	
	bit parityBit = getParity(byte); //sprawdzamy bit parzystosci, ktory wyslemy pozniej
	
	//wysylamy bit startu 	
	dataM = 0; //zmieniamy wartosc na nozce data
	wait(waitTimeLong);
	clockM = 0;	//zmieniamy wartosc na clock
	wait(waitTimeLong); //czekamy na odczyt
	clockM = 1; //zmieniamy wartosc na clock 
	wait(waitTimeLong);	 
	 
	 do{
			dataM = byte & 0x01; //robimy taka maske zeby odczytac najmlodszy bit
			wait(waitTimeLong);
			clockM = 0;			 //zmieniamy wartosc na clock
			wait(waitTimeLong);	 //czekamy na odczyt
			clockM = 1;			 //zmieniamy wartosc na clock 
			wait(waitTimeLong);	 
			byte = byte >> 1;	 //przygotowujemy do nastepnego zastosowania maski
	 }while(--bitesOfNumberToSend > 0);
	 
	//wysylamy bit parzystosci 
	dataM = parityBit;
	wait(waitTimeLong);
	clockM = 0;	//zmieniamy wartosc na clock
	wait(waitTimeLong); //czekamy na odczyt
	clockM = 1;	 //zmieniamy wartosc na clock
	wait(waitTimeLong);	 
	
	//wysylamy bit stopu 
	dataM = 1;
	wait(waitTimeLong);
	clockM = 0;	//zmieniamy wartosc na clock
	wait(waitTimeLong); //czekamy na odczyt
	clockM = 1;	//zmieniamy wartosc na clock
	wait(waitTimeLong);	 
	
	//zmieniamy wartosc na obu liniach sygnalizujac ze nic nie wysylamy  
	clockM = 1;
	dataM = 1;
 }

 unsigned char readBytePlusControlBits()
 {
	unsigned char bitesOfNumberToSend = 8; 	
	unsigned char result = 0;
	
	//czekamy na zmiane na clock na 0 - czyli sygnal ze dane sa tak jakby wazne
	while(clockS == 1)
	{
		wait(waitTimeShort);
	}
	//odbieramy bit startu, ale nic z nim w tym cwieniu nie robimy
	while(clockS == 0)
	{
			wait(waitTimeShort);
	}
	 
	 
	 do
	 {
		//czekamy na 0 co znaczy ze na lini danych dane sa wazne
	 	while(clockS == 1)
		{
			wait(waitTimeShort);
		}
		//przesuwamy nasz wynik zeby zrobic miejsce na nasz odczytany bit
		result = result >> 1;
		//jesli na lini jest jedynka to w odpowiedni miejsce wpisujemy jedynke
		//a jak nie to moze pozostac tam 0
		if(dataS == 1)
		{
			result = result | 0x80;
		}		
		
		while(clockS == 0)
		{
			wait(waitTimeShort);
		}
	}while(--bitesOfNumberToSend > 0);
	
	//tak jak wyzej opisane, odbieramy bit parzystosci
	while(clockS == 1)
	{
			wait(waitTimeShort);
	}
	
	if(dataS != getParity(result))
	{
		P1 = 0xFF ;
	}
	
	while(clockS == 0)
	{
		wait(waitTimeShort);
	}
		
	//czekamy na zmiane na clock na 0 - czyli sygnal ze dane sa tak jakby wazne
	while(clockS == 1)
	{
		wait(waitTimeShort);
	}
	//odbieramy bit konca, ale w tym cwiczneiu nic z nim nie robimy
	while(clockS == 0)
	{
		wait(waitTimeShort);
	}
	
	return result;
 }

 //funkcja ustawiajaca startowe wartosci rejestrow i zmiennych
 void init()
 {
 	numberM = 0; // zerujemy zmienna number
	IT0 = 1;	//INT0 aktywne zero
	EX0 = 1;	//Wlaczenia INT0
	EA = 1;		//Wlaczenie wszstkich przerwan
	P1 = 0;		//zerujemy port P1
	clockM = 1;	//ustawiamy na lini clock mastera 0
	dataM = 1;	//data tez ustawiamy na 0 u mastera 
 }

 void main(void)
 {
 	init(); //wywolanie funkcji init ustawiajacej nasze wartosci startowe
	//MASTER (NADAJNIK) sygnalizuje o tym PO_0 ustawione na 1
	while (P0_0 == 1)		
	{
		while(flag)	 //dopoki flaga nie zostanie zmieniona - nie wystwiapi przerwanie - nie wystapi nacisniecie przycisku
		{						   //to nadajnik czeka 
			wait(waitTimeShort);
		}
		P1 = numberM;	  //guzik nacisniety zinkrementowana zmienna numberM, wyswietlamy ja na wyswietlaczy
		sendBytePlusControlBits(numberM);  //wysylamy bajt
		flag = 1;	 		  //znowu ustawiamy flage na 1 i czekamy na kolejny bajt do wyslania
	}
	//SLAVE (ODBIORNIKA) sygnalizuje o tym PO_0 ustawione na 0
	while (P0_0 == 0)
	{
		while(clockS == 1)		   //dopoki na lini clock jest 1 
		{						   //to odbiornik czeka na sygnal ze cos jest wysylane
			wait(waitTimeShort); //czyli zmiane na lini clock na 0
		}
		numberS = readBytePlusControlBits();    //jak juz wartosci sie zmieni to ja odczytuje
		P1 = numberS;			   //i wyswietla na wyswietlaczu
	}
 }