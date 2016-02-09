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
#include <EEPROM.h>
#include "Game.h"
#include "Menu.h"
#include "Jump.h"
#include "Opmaak.h"


//onze game, dit is wat er gebeurt als de game draait
void Game::game(MI0283QT9 lcd, Menu *M, Opmaak *O, Jump *J) {
  tellert = 0;
  lcd.fillScreen(RGB(255, 255, 255));                                                       //scherm leeg
  lcd.drawText(10, 210, "Score:", RGB(0, 0, 0), RGB(255, 255, 255), 2);                     //tekent de tekst score op het scherm
  lcd.drawInteger(105, 210, M->getterScore() , DEC, RGB(0, 0, 0), RGB(255, 255, 255), 2);   //tekent het integer score op het scherm
  O->tekenLijn(lcd);                                                                        //tekent de vloer waar de speler op loopt
  nunchuck_setpowerpins();                                                                  //set de power pins van de nunchuck zodat de waardes kunnen worden aangepast
  nunchuck_init();                                                                          //initialiseert de nunchuck
  O->speler(lcd);                                                                           //tekent de speler op het scherm
  O->drawMoeilijkheid(lcd, moeilijkheid);                                                   //tekent de moeilijkheid op het scherm

  O->obstakelLocatie1 = 320;                                                                  //set de obstakelLocatie1 op 0
  O->obstakelLocatie2 = 0;                                                                  //set de obstakelLocatie2 op 0
  O->aantalObstakels = 0;                                                                   //set het aantalObstakels op 0
  while (gameIsLive) {                                                                      //loopt zolang de speler leeft
//    Serial.print("gameIsLive: ");
//    Serial.println(gameIsLive);
//    Serial.print("death: ");
//    Serial.println(death);
    if(veranderd){
      tellert++;
      nunchuck_get_data();                                                                    //haalt de data van de nunchuck op
      J->zbutton = nunchuck_zbutton();                                                         //haalt de waarde van de zbutton van de nunchuck op
      J->checkJump(); 
      O->randomLevel();                                                                       //kijkt of er een obstakel moet worden gegenereerd, en zo ja, kijkt of er 1 komt.
      if(tellert > 5){
        hitbox(J, M);                                                                           //kijkt of de speler af is
      }
      J->tekenJump(lcd);
      O->teken(lcd);                                                                          //tekent de jump
      
      
      vormObstakel1 = O->obstakelVorm1;                                                       //slaat de vorm van het eerste obstakel op onder een lokale variabele
      locatieObstakel1 = O->obstakelLocatie1;                                                 //slaat de locatie van het eerste obstakel op onder een lokale variabele
      
      if(locatieObstakel1 == -32){
        if(moeilijkheid > 100){
          moeilijkheid = 255 - M->score;
        }
      }
      if (death) {                                                                            //kijkt of de speler dood is, en zo ja stopt de game
        gameIsLive = false;
      }
      
      veranderd = false;
    }
//    Serial.print("gameIsLive: ");
//    Serial.println(gameIsLive);
//    Serial.print("death: ");
//    Serial.println(death);
  }
}

//onze hitbox
void Game::hitbox(Jump *U, Menu *E) {
  if (vormObstakel1 == 2) {                                                                 //als eerste figuur == 4kant
    if (47 > locatieObstakel1) {                                                            //kijkt of het eerste obstakel dichtbij genoeg is
      if (U->positionY > 129) {                                                              //kijkt of de speler laag genoeg is om dood te gaan aan het obstakel, en zo ja, zegt dat de speler dood is
        death = true;
        Serial.println("2");

        geland = false;
      }
      if (U->positionY > 127 && U->positionY < 129) {                                         //kijkt of de speler is geland op het obstakel, en zo ja, zorgt ervoor dat de speler niet meer valt en continue op de obstakel blijft
        U->velocityY = 0.0;
        U->in_air = false;
        U->positionY = 128;
        geland = true;
        geraakt = true;

        U->updateJump();
      }
    }
    if (locatieObstakel1 == 32) {                                                           //kijkt of de eerste obstakel bijna voorbij de speler is en zo ja, geeft de speler punten
      if (eersteKeer) {
        if (vormObstakel1 == 2) {
          E->incScore();
        }

        eersteKeer = false;
      }
    }
    if (locatieObstakel1 > 31 && geland) {                                                  //als de speler is geland kijkt hij of het obstakel voorbij is, en zo ja, laat de speler weer vallen
      U->in_air = true;
      geland = false;
    }


    if (locatieObstakel1 == 28) {
      eersteKeer = true;
    }

    if (U->positionY >= 160) {                                                               //zorgt ervoor dat de speler niet meer valt zodra hij op de vloer staat
      U->in_air = false;
    }
  }

//  if (vormObstakel1 == 1) {                                                                 //kijkt of het eerste obstakel een driehoek is
//    if (47 > locatieObstakel1) {                                                            //kijkt of het eerste obstakel dichtbij genoeg is
//      if (U.positionY > 128) {                                                              //kijkt of de speler tegen het driehoek aan is gekomen en zo ja, maakt de speler dood
//        death = true;
//      }
//    }
//  }

 if (vormObstakel1 == 1) {
    if (47 > locatieObstakel1 && locatieObstakel1 > 31) {
      if (U->positionY > currentY) {
        death = true;
        Serial.println("1");
      }
      if (locatieObstakel1 < 31) {
        currentY += 2;
      } else {
        currentY -= 2;
      }
    }
  }
  if (locatieObstakel1 == 32) {
    if (eersteKeer) {
      E->incScore();
      E->incScore();
      eersteKeer = false;

    }
  }

  if (locatieObstakel1 == 28) {
    eersteKeer = true;
  }
  if(locatieObstakel1 == 0){
    currentY = 160;
  }
}

