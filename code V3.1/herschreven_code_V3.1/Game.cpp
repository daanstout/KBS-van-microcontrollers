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

  J.positionY = 160;
  O.obstakelLocatie1 = 0;
  O.obstakelLocatie2 = 0;
  O.aantalObstakels = 0;
  //J.in_air = false;

  while (gameIsLive) {
    nunchuck_get_data();
    J.zbutton = nunchuck_zbutton();

    //Serial.print(J.zbutton);

    O.randomLevel();
    vormObstakel1 = O.obstakelVorm1;
    locatieObstakel1 = O.obstakelLocatie1;
    yPosition = J.positionY;
    lucht = J.in_air;
    //    Serial.print(O.obstakelLocatie1);
    //    Serial.print(" - ");

    //    Serial.print(vormObstakel1);
    hitbox();
    O.sidescroll(lcd);

    O.teken(lcd);
    J.checkJump();
    J.tekenJump(lcd);

    //        Serial.println(death);
    //        Serial.println(locatieObstakel1);
    //        Serial.println(yPosition);
    //        Serial.println(death);
    //        Serial.println(" - ");
    //    Serial.println(O.obstakelLocatie1);
    //    Serial.print('\n');
    //    _delay_ms(100);


    if (death) {
      gameIsLive = false;
    }
  }


}
void Game::hitbox() {
  Jump J;
  Opmaak O;

  //  Serial.print(" - ");
  //  Serial.print(O.obstakelVorm1);
  //  Serial.print(" - ");
  //  Serial.println(vormObstakel1);
  if (vormObstakel1 == 2) { //als eerste figuur == 4kant
    //Serial.println("vorm1 == 2");
    if (locatieObstakel1 < 48) {
      Serial.println("O.Obstakel1 < 47");
      Serial.println(geland);

      if (yPosition >= 128 && lucht && !geland) {

        Serial.println("hier1");

        death = false;
        J.velocityY = 0.0;
        J.in_air = false;
        J.positionY = 128;
        geland = true;

      } else if (!geland) {
        if (locatieObstakel1 <= 46 && yPosition > 129) {
          Serial.println("hier2");
          death = true;
        }
      }
      if (J.positionY >= 128.0 && geland) {
        J.positionY = 128.0;
        J.velocityY = 0.0;
        J.in_air = false;
      }

      if (locatieObstakel1 < 32 && geland) {
        Serial.println("hier3");
        J.in_air = true;
        geland = false;
        death = false;
      }
      if (yPosition > 160) {
        J.in_air = false;
      }
    }
  }
  if (O.obstakelVorm1 == 1) {
    if (O.obstakelLocatie1 < 47) {
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
