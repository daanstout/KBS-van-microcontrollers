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
#include "Game.h"
#include "Menu.h"
#include "Jump.h"
#include "Opmaak.h"



void Game::game(MI0283QT9 lcd) {
  Jump J;
  Menu M;
  Opmaak O;

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


  while (gameIsLive) {
    nunchuck_get_data();
    J.zbutton = nunchuck_zbutton();



    O.randomLevel();

      //yPosition = J.positionY;
      //yVel = J.velocityY;
   
    hitbox(O, J);
    J.tekenJump(lcd);
    O.sidescroll(lcd);

    O.teken(lcd);
    J.checkJump();


    if (death) {
      gameIsLive = false;
    }
  }
}
void Game::hitbox(Opmaak P, Jump J) {
  if (P.obstakelVorm1 == 2) {
    if (47 > P.obstakelLocatie1) {
      
    }
    
   
  if (P.obstakelVorm1 == 1) {
    if (47 > P.obstakelLocatie1) {
      if (J.positionY > currentY) {
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
}
}
