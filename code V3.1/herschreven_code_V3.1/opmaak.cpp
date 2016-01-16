#include <avr/io.h>
#include <Wire.h>
#include <SPI.h>
#include <digitalWriteFast.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <Arduino.h>
#include "nunchuck_funcs.h"
#include <util/delay.h>
#include <stdlib.h>
#include "Opmaak.h"
#include "Jump.h"
#include "Menu.h"
#include "Game.h"

//onze sidescroll functie
void Opmaak::sidescroll(MI0283QT9 lcd, Menu *E) {

  Jump J;
  Game G;

  //kijkt of er obstakels zijn
  if (aantalObstakels > 0) {
    //slaat de vorige locatie op
    vorigeObstakel1 = obstakelLocatie1;
    vorigeObstakel2 = obstakelLocatie2;

    //kijkt of een obstakel uit het scherm is
    if (obstakelLocatie1 == -32) {

      //redrawed de score
      lcd.fillRect(105, 210, 20, 20, RGB(255, 255, 255));
      lcd.drawInteger(105, 210, E->getterScore() , DEC, RGB(0, 0, 0), RGB(255, 255, 255), 2);

      //past het aantal obstakels aan
      aantalObstakels--;

      //kijkt of er nog een tweede obstakel is, en zo ja, slaat die waardes op onder de eerste
      if (aantalObstakels == 1) {
        obstakelLocatie1 = obstakelLocatie2;
        obstakelVorm1 = obstakelVorm2;
        obstakelVorm2 = 0;
      } else if (aantalObstakels == 0) {
        obstakelVorm1 = 0;
      }
    }

    //haalt 1 van de obstakel locaties af zodat ze kunnen scrollen
    obstakelLocatie1--;
    obstakelLocatie2--;
  }
}

void Opmaak::randomLevel() {
  Game G;
  if (aantalObstakels == 0) {//kijkt of er geen obstakels zijn, en zo ja, maakt er 1 aan
    randomObstakelVorm = (random(0, G.moeilijkheid)) + 1;   //bepaalt de vorm via een random getal
    aantalObstakels++;
    if (randomObstakelVorm != 1) {
      obstakelVorm1 = 2;
    } else {
      obstakelVorm1 = 1;
    }
    obstakelLocatie1 = 320;
  }
  if (obstakelLocatie1 <= 160 && aantalObstakels < 2) {    //kijkt of er minder dan 2 zijn en het eerste obstakel een bepaalde afstand heeft afgelegd
    nieuwObstakel = (random(0, 3)) + 1;       //bepaalt of er een nieuw obstakel komt via een random getal
    if (nieuwObstakel == 1) {
      randomObstakelVorm = (random(0, G.moeilijkheid)) + 1;     //bepaalt de vorm via een random getal
      aantalObstakels++;
      if (randomObstakelVorm == 1) {
        obstakelVorm2 = 1;
      } else {
        obstakelVorm2 = 2;
      }


      obstakelLocatie2 = 320;
    }
  }


}


//tekent de obstakels tijdens het scrollen
void Opmaak::teken(MI0283QT9 lcd) {
  if (obstakelLocatie1 != vorigeObstakel1) {
    if (obstakelVorm1 == 2) {
      drawVierkant(obstakelLocatie1, lcd);
      resetVierkant(vorigeObstakel1, lcd);
    }
    if (obstakelVorm1 == 1) {
      drawDriehoek(obstakelLocatie1, lcd);
      resetDriehoek(vorigeObstakel1 , lcd);
    }
  }
  if (aantalObstakels == 2 && obstakelLocatie2 != vorigeObstakel2) {
    if (obstakelVorm2 == 2) {
      drawVierkant(obstakelLocatie2, lcd);
      resetVierkant(vorigeObstakel2 - 1, lcd);
    }
    if (obstakelVorm2 == 1) {
      drawDriehoek(obstakelLocatie2, lcd);
      resetDriehoek(vorigeObstakel2 - 1, lcd);
      resetVierkant(vorigeObstakel1 -2, lcd);
    }
  }
  _delay_ms(2);
}

//tekent een lijn van de vierkant
void Opmaak::drawVierkant(int x, MI0283QT9 lcd) {
  lcd.drawLine(x, 160, x, 128, RGB(255, 0, 0)); //eerste rij genereren
}

// haalt een lijn weg van de vierkant
void Opmaak::resetVierkant(int x, MI0283QT9 lcd) {
  lcd.drawLine(x + 32, 160, x + 32, 128, RGB(255, 255, 255)); //laatste rij van obstakel resetten
}

//tekent een lijn van de driehoek
void Opmaak::drawDriehoek(int x, MI0283QT9 lcd) {
  lcd.drawLine(x, 160, x + 16, 128, RGB(200, 0, 0));
}

//haalt een lijn wev van de driehoek
void Opmaak::resetDriehoek(int x, MI0283QT9 lcd) {
  lcd.drawLine(x + 16, 128, x + 32, 160, RGB(255, 255, 255));
}

//tekent de vloer
void Opmaak::tekenLijn(MI0283QT9 lcd) {
  lcd.fillRect(0, 160, 320, 32, RGB(0, 100, 0));
}

//tekent de speler
void Opmaak::speler(MI0283QT9 lcd) {
  Jump J;
  lcd.fillRect(32, J.positionY - 15, 15, 15, RGB(0, 0, 0));
}

//tekent de moeilijkheid bovenin het scherm en kleurt hem in als de moeilijkheid hoger wordt
void Opmaak::drawMoeilijkheid(MI0283QT9 lcd) {
  Game G;
  if (G.moeilijkheid == 255) {
    for (uint16_t c = 195; c < 300; c += 25) {
      lcd.drawCircle(c, 15, 10, RGB(0, 0, 0));
    }
  } else {
    if (G.moeilijkheid == 130 || G.moeilijkheid == 129) {
      lcd.fillCircle(295, 15, 10, RGB(0, 0, 0));
    } else if (G.moeilijkheid == 160 || G.moeilijkheid == 159) {
      lcd.fillCircle(270, 15, 10, RGB(0, 0, 0));
    } else if (G.moeilijkheid == 190 || G.moeilijkheid == 189) {
      lcd.fillCircle(245, 15, 10, RGB(0, 0, 0));
    } else if (G.moeilijkheid == 220 || G.moeilijkheid == 219) {
      lcd.fillCircle(220, 15, 10, RGB(0, 0, 0));
    } else if (G.moeilijkheid == 250 || G.moeilijkheid == 249) {
      lcd.fillCircle(195, 15, 10, RGB(0, 0, 0));
    }
  }
}
