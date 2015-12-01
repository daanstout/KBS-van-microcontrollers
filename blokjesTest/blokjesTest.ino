#include <avr/io.h>
#include <Wire.h>
#include <SPI.h>
#include <digitalWriteFast.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <Arduino.h>
#include "nunchuck_funcs.h"

MI0283QT9 lcd;
int up = 35;
int zbutton = 0;
int in_air = 0;

void pixel(int x, int y, String kleur) {
  lcd.drawRect(x, y, 16, 16, RGB(255, 255, 255)); //wit randje

  if (kleur == "groen") {             //onderste rij is groen
    lcd.fillRect(x, y, 15, 15, RGB(0, 100, 0));
  }
  else {
    lcd.fillRect(x, y, 15, 15, RGB(0, 0, 0));
  }

}

void vulScherm() {
  //for(int p = 0; p <= 240; p++){            //pixels verplaatsen
  for (int j = 0; j < 320; j += 16) {     //pixels op alle plekken op de x-as zetten
    for (int k = 160; k < 240; k += 16) {   //pixels op onderste rij y-as
      pixel(j, k, "groen");
    }
  }
  //if(p == 240){
  //  p = 0;
  //}
  //_delay_ms(500);
  //}
}

void tekenLijn() {
  lcd.fillRect(0, 160, 320, 32, RGB(0, 100, 0));
}

void obstakel(int x) {
  lcd.fillRect(x, 160 - (32), 32, 32, RGB(255, 0, 0)); //blokje genereren met de opgegeven hoogte
}
void resetObstakel(int x) {
  lcd.fillRect(x + 32, 160 - (32), 1, 32, RGB(255, 255, 255)); //laatste rij van obstakel resetten
}



void sidescroll() {
  int last_x;
  int x;
  

  while (1) {
    
    for (x = 320; x >= -32 ; x--) {
      resetObstakel(last_x);
      obstakel(x);
      lcd.drawInteger(50, 10, last_x , DEC, RGB(0, 0, 0), RGB(255, 255, 255), 1);
      last_x = x;
      lcd.drawInteger(10, 10, x , DEC, RGB(0, 0, 0), RGB(255, 255, 255), 1);


      zbutton = nunchuck_zbutton();

      speler();
      if (zbutton == 1 && in_air == 0) {
        Serial.print("test speler");
        jump();
        zbutton = 0;
      }

      _delay_ms(10);
      if (x == -32) {

        x = 320;
      }
    }
  }
}
void speler() {
  lcd.fillCircle(32, 160 - 16, 16, RGB(0, 0, 255)); //bolletje
  while (zbutton == 1) {

  }
}

void jump() {
  in_air = 1;
  int current = 159;
  for (int i = 0; i <= up; i++) {


    lcd.fillCircle(32, (current + 1) - 16, 16, RGB(255, 255, 255)); //verwijder vorige ball
    lcd.fillCircle(32, current - 16, 16, RGB(0, 0, 255)); //ball omhoog
    _delay_ms(1);
    current--;  //speler tekent bolletje steeds opnieuw vind oplossing
  }
  for (int i = 0; i <= up; i++) {



    lcd.fillCircle(32, (current - 1) - 16, 16, RGB(255, 255, 255)); //verwijder vorige ball
    lcd.fillCircle(32, current - 16, 16, RGB(0, 0, 255)); //ball omlaag
    _delay_ms(1);
    current++;
  }

  //omgekeerd naar benede
  in_air = 0;

}

int main() {
  init();
  lcd.begin();
  lcd.fillScreen(RGB(255, 255, 255)); // scherm leeg
  tekenLijn();




  sidescroll();
  nunchuck_get_data();




  return 0;
}
