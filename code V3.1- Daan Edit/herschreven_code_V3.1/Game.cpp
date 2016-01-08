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
    J.zbutton = nunchuck_zbutton();

    J.checkJump();
    O.sidescroll(lcd);
    O.randomLevel();
    hitbox(O, J);
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
void Game::hitbox(Opmaak P, Jump U) {
//  Menu M;
//  Opmaak O;
//  Jump J;
  
  if (P.obstakelVorm1 == 2) {
    if (47 > P.obstakelLocatie1) {
      if (U.positionY > 121 && U.positionY < 129) {
        U.velocityY = 0;
        U.in_air = false;
        U.positionY = 128;
        geland = true;
      }
      if (U.positionY > 128) {
        death = true;
      }
    }
    if (32 < P.obstakelLocatie1 && geland) {
      U.in_air = true;
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
}
