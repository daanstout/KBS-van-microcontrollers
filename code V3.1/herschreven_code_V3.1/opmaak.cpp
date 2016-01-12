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

void Opmaak::sidescroll(MI0283QT9 lcd) {
  Menu M;
  Jump J;
  Game G;
  if (aantalObstakels > 0) {
    vorigeObstakel1 = obstakelLocatie1;
    vorigeObstakel2 = obstakelLocatie2;

    J.checkJump();

    if (obstakelLocatie1 == -32) {
     // Serial.print("sidescroll obstakelvorm 1: ");
      //Serial.println(obstakelVorm1);
      if (G.geland) {
        M.score++;
      } else if (!G.geland) {
        M.score += 2;
      }

      lcd.fillRect(105, 210, 20, 20, RGB(255, 255, 255));
      lcd.drawInteger(105, 210, M.score, DEC, RGB(0, 0, 0), RGB(255, 255, 255), 2);

      aantalObstakels--;

      if (aantalObstakels == 1) {
        Serial.print("sidescroll obstakelvorm 2: ");
        Serial.println(obstakelVorm1);
        obstakelLocatie1 = obstakelLocatie2;
        obstakelVorm1 = obstakelVorm2;
        obstakelVorm2 = 0;
      } else if (aantalObstakels == 0) {
        obstakelVorm1 = 0;
      }
    }
    obstakelLocatie1--;
    obstakelLocatie2--;
  }
}

void Opmaak::randomLevel() {
  Game G;
  if (aantalObstakels == 0) {
    randomObstakelVorm = (random(0, G.moeilijkheid)) + 1;
    aantalObstakels++;
    if (randomObstakelVorm != 1) {
      obstakelVorm1 = 2;
    } else {
      obstakelVorm1 = 1;
    }
    obstakelLocatie1 = 320;
  }
  if (obstakelLocatie1 < 160 && aantalObstakels < 2) {
    nieuwObstakel = (random(0, 3)) + 1;
    if (nieuwObstakel == 1) {
      randomObstakelVorm = (random(0, G.moeilijkheid)) + 1;
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
void Opmaak::teken(MI0283QT9 lcd) {
  Jump J;
  if (obstakelLocatie1 != vorigeObstakel1) {
    if (obstakelVorm1 == 2) {
      drawVierkant(obstakelLocatie1, lcd);
      resetVierkant(vorigeObstakel1, lcd);
    }
    if (obstakelVorm1 == 1) {
      drawDriehoek(obstakelLocatie1, lcd);
      resetDriehoek(vorigeObstakel1, lcd);
    }
  }
  if (aantalObstakels == 2 && obstakelLocatie2 != vorigeObstakel2) {
    if (obstakelVorm2 == 2) {
      drawVierkant(obstakelLocatie2, lcd);
      resetVierkant(vorigeObstakel2, lcd);
    }
    if (obstakelVorm2 == 1) {
      drawDriehoek(obstakelLocatie2, lcd);
      resetDriehoek(vorigeObstakel2, lcd);
    }
  }
  _delay_ms(2);
}
void Opmaak::drawVierkant(int x, MI0283QT9 lcd) {
  lcd.drawLine(x, 160, x, 128, RGB(255, 0, 0)); //eerste rij genereren
}

void Opmaak::resetVierkant(int x, MI0283QT9 lcd) {
  lcd.drawLine(x + 32, 160, x + 32, 128, RGB(255, 255, 255)); //laatste rij van obstakel resetten
}

//driehoek:
void Opmaak::drawDriehoek(int x, MI0283QT9 lcd) {
  lcd.drawLine(x, 160, x + 16, 128, RGB(200, 0, 0));
}
void Opmaak::resetDriehoek(int x, MI0283QT9 lcd) {
  lcd.drawLine(x + 16, 128, x + 32, 160, RGB(255, 255, 255));
}
void Opmaak::tekenLijn(MI0283QT9 lcd) {
  lcd.fillRect(0, 160, 320, 32, RGB(0, 100, 0));
}
void Opmaak::speler(MI0283QT9 lcd) {
  Jump J;
  lcd.fillRect(32, J.positionY - 15, 15, 15, RGB(0, 0, 0));
}
void Opmaak::drawMoeilijkheid(MI0283QT9 lcd){
  Game G;
  if(G.moeilijkheid == 255){
    for(uint16_t c = 195; c < 300; c += 25){
      lcd.drawCircle(c, 15, 10, RGB(0, 0, 0));
    }
  }else{
    if(G.moeilijkheid == 130 || G.moeilijkheid == 129){
      lcd.fillCircle(295, 15, 10, RGB(0, 0, 0));
    }else if(G.moeilijkheid == 160 || G.moeilijkheid == 159){
      lcd.fillCircle(270, 15, 10, RGB(0, 0, 0));
    }else if(G.moeilijkheid == 190 || G.moeilijkheid == 189){
      lcd.fillCircle(245, 15, 10, RGB(0, 0, 0));
    }else if(G.moeilijkheid == 220 || G.moeilijkheid == 219){
      lcd.fillCircle(220, 15, 10, RGB(0, 0, 0));
    }else if(G.moeilijkheid == 250 || G.moeilijkheid == 249){
      lcd.fillCircle(195, 15, 10, RGB(0, 0, 0));
    }
  }
}
