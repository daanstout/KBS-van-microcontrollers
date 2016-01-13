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



void Game::game(MI0283QT9 lcd, Menu M, Opmaak O, Jump J) {

  lcd.fillScreen(RGB(255, 255, 255)); // scherm leeg
  lcd.drawText(10, 210, "Score:", RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.drawInteger(105, 210, M.score, DEC, RGB(0, 0, 0), RGB(255, 255, 255), 2);
  O.tekenLijn(lcd);
  nunchuck_setpowerpins();
  nunchuck_init();
  O.speler(lcd);
  O.drawMoeilijkheid(lcd);

  //J.positionY = 160;
  O.obstakelLocatie1 = 0;
  O.obstakelLocatie2 = 0;
  O.aantalObstakels = 0;



  //  while (gameIsLive) {
  //    nunchuck_get_data();
  //    J.zbutton = nunchuck_zbutton();
  //
  //
  //
  //    O.randomLevel();
  //
  //      //yPosition = J.positionY;
  //      //yVel = J.velocityY;
  //
  //    hitbox(O, J);
  //    J.tekenJump(lcd);
  //
  //  //J.in_air = false;
  //
  //  while (gameIsLive) {
  //    nunchuck_get_data();
  //    J.zbutton = nunchuck_zbutton();
  //  //J.in_air = false;

  while (gameIsLive) {
    nunchuck_get_data();
    J.zbutton = nunchuck_zbutton();
    
    //Serial.print(J.zbutton);

    O.randomLevel();
    vormObstakel1 = O.obstakelVorm1;
    locatieObstakel1 = O.obstakelLocatie1;
    O.sidescroll(lcd, M);
 
    O.teken(lcd);

    hitbox(O, J);
    J.checkJump();
    if (death) {
      gameIsLive = false;
    }
    if (!geland) {
      J.updateJump();
    }
    J.tekenJump(lcd);

    //    Serial.println(death);
    //    Serial.println(J.positionY);
    //    Serial.println(O.obstakelLocatie1);
    //    Serial.print('\n');
    //    _delay_ms(100);




  }

}
//void Game::hitbox(Opmaak P, Jump J) {
//  if (P.obstakelVorm1 == 2) {
//    if (47 > P.obstakelLocatie1) {
//
//    }
//
//
//  if (P.obstakelVorm1 == 1) {
//    if (47 > P.obstakelLocatie1) {
//
//
//
//}
void Game::hitbox(Opmaak P, Jump U) {

  if (vormObstakel1 == 2) { //als eerste figuur == 4kant

    if (47 > locatieObstakel1) {
      if (U.positionY > 129) {
        death = true;
        geland = false;
      }
      if (U.positionY > 127 && U.positionY < 129) {
        U.velocityY = 0.0;
        U.in_air = false;
        U.positionY = 128;
        geland = true;
//        punten = false;
//        P.dubbelPunten = false;
//        Serial.println(P.dubbelPunten);

        U.updateJump();
      }
    }
    if (32 < locatieObstakel1 && geland) {
      U.in_air = true;
      geland = false;
    }
    if (U.positionY > 160) {
      U.in_air = false;
      
    }
  }
  if (P.obstakelVorm1 == 1) {
    if (47 > P.obstakelLocatie1) {
      if (U.positionY > currentY) {
        death = true;
      }
      if (P.obstakelLocatie1 < 31) {
        currentY += 2;
      } else {
        currentY -= 2;
      }
    }
  }
  if (currentY == 128) {
    currentY = 160;
  }
//  Serial.println(geland);
//  Serial.println(P.dubbelPunten);
}

