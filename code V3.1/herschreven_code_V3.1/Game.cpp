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
  Menu M;
  Opmaak O;
  Jump J;
  
  lcd.fillScreen(RGB(255, 255, 255)); // scherm leeg
  lcd.drawText(10, 210, "Score:", RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.drawInteger(105, 210, M.score, DEC, RGB(0, 0, 0), RGB(255, 255, 255), 2);
  O.tekenLijn(lcd);
  nunchuck_setpowerpins();
  nunchuck_init();
  O.speler(lcd);

  while (gameIsLive) {
    nunchuck_get_data();
    J.zbutton = nunchuck_get_data();
    
    J.checkJump();
    O.sidescroll(lcd);
    O.randomLevel();
    hitbox();
    O.teken(lcd);

    if (death) {
      gameIsLive = false;
    }
  }

  J.positionY = 160;
  O.obstakelLocatie1 = 0;
  O.obstakelLocatie2 = 0;
  O.aantalObstakels = 0;
  J.in_air = false;
}
void Game::hitbox() {
  Menu M;
  Opmaak O;
  Jump J;
  if (O.obstakelVorm1 == 2) {
    if (47 > O.obstakelLocatie1) {
      if (J.positionY > 121 && J.positionY < 129) {
        J.velocityY = 0;
        J.in_air = false;
        J.positionY = 128;
        geland = true;
      }
      if (J.positionY > 128) {
        death = true;
      }
    }
    if (32 < O.obstakelLocatie1 && geland) {
     J.in_air = true;
    }
    if(J.positionY > 160){
      J.in_air = false;
    }
  }
  if (O.obstakelVorm1 == 1) {
    if (47 > O.obstakelLocatie1) {
      if (J.positionY > currentY) {
        death = true;
      }
      if (O.obstakelLocatie1 < 31) {
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
