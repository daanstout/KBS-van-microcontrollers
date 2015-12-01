#include <avr/io.h>
#include <Wire.h>
#include <SPI.h>
#include <digitalWriteFast.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <Arduino.h>
#include "nunchuck_funcs.h"


MI0283QT9 lcd;
int up = 35; //hoogte van de sprong
int zbutton = 0; //waarde van de Z knop
int in_air = 0; //waarde om aan te geven of de speler al aan het springen is

void pixel(int x, int y, String kleur){
	lcd.drawRect(x,y,16,16,RGB(255,255,255)); //wit randje
	
	if(kleur == "groen"){ //onderste rij is groen
		lcd.fillRect(x,y,15,15,RGB(0,100,0));
		}
		else{
		lcd.fillRect(x,y,15,15,RGB(0,0,0));
	}
	
}

void vulScherm(){
	//for(int p = 0; p <= 240; p++){						//pixels verplaatsen
		for(int j = 0; j < 320; j += 16){				//pixels op alle plekken op de x-as zetten
			for(int k = 160; k < 240; k += 16){			//pixels op onderste rij y-as
				pixel(j,k, "groen");
			}
		}
		//if(p == 240){
		//	p = 0;
		//}
		//_delay_ms(500);
    //}
}

void tekenLijn(){
	lcd.fillRect(0, 160,320,32,RGB(0,100,0));
}

void obstakel(int hoogte){

  lcd.fillRect(64, 160 - (32*hoogte),32,32 * hoogte, RGB(255,0,0));    //blokje genereren met de opgegeven hoogte
  
}

void speler(){
  lcd.fillCircle(32, 160-16, 16, RGB(0,0,255)); //bolletje
  while(zbutton == 1){  //controleren op indrukken van knop Z
  if (zbutton == 1 && in_air == 0) { //bevind de speler zich al in de lucht? -> nee? -> voer uit if statement
    jump();
    zbutton = 0; //na springen knop weer op 0
  }
  }
}

void jump(){
  in_air = 1; //speler bevind zich nu in de lucht
  int current = 159; //begin punt van de speler
  for(int i = 0; i <= up; i++){ //voor de hoogte van int up, de speler omhoog verplaatsen
    
  lcd.fillCircle(32, (current + 1)-16, 16, RGB(255,255,255)); //verwijder vorige ball
  lcd.fillCircle(32, current-16, 16, RGB(0,0,255));  //teken een nieuwe ball
  _delay_ms(1);
  current--; //haal 1 van current af zodat de bal volgende keer door de loop weer 1 omhoog word verplaatst
  }
  for(int i = 0; i <= up; i++){ //omgekeerd process van hierboven
 
   lcd.fillCircle(32, (current - 1)-16, 16, RGB(255,255,255)); 
   lcd.fillCircle(32, current-16, 16, RGB(0,0,255));  
   _delay_ms(1);
   current++;
  }
  in_air = 0; //zet op nul, speler is terug op de grond.
}


int main(){
	init();
	lcd.begin();
        Serial.begin(9600);
	lcd.fillScreen(RGB(255,255,255)); // scherm leeg

	nunchuck_setpowerpins(); 
	nunchuck_init();

	tekenLijn();
	obstakel(1);   
        while(1){
               // Serial.print("main test");
		nunchuck_get_data();
                zbutton = nunchuck_zbutton();
                speler();
        }
        
        return 0;
}
