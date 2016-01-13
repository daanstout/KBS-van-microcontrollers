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
#include "Jump.h"
#include "Opmaak.h"



void Jump::checkJump() {
  //Serial.print("check");
  if (zbutton == 1) startJump();
  if (zbutton == 0) endJump();

  
  
}
void Jump::startJump() {
  if (!in_air) {
    velocityY = -2.5;
    in_air = true;
    //Serial.print("sjump");
  }
}

void Jump::endJump() {

  if (in_air) {
    if (velocityY < -1.5) {
      velocityY = -1.5;
      //in_air = false;
      //Serial.print("ejump");
    }
  }
}

void Jump::updateJump() {

  last_y = positionY;
  velocityY += gravity;
  positionY += velocityY;

  Opmaak O;

  if (positionY >= 160.0) {
    positionY = 160.0;
    velocityY = 0.0;
    in_air = false;
  }
  //  if(O.obstakelLocatie1 < 47){
  //    Serial.print(positionY);
  //    Serial.println(" JUMP");
  //    if(positionY <= 160 && positionY > 127){ //linkerzijde
  //        if(!geland){
  //
  //        }
  //      }
  //  if(positionY >= 127 && positionY <= 129 && in_air && (O.obstakelLocatie1 <= 32 && O.obstakelLocatie1 >= 47)){
  //
  //      geland = true;
  //      in_air = false;
  //
  //      positionY = 128;
  //      velocityY = 0.0;
  //   }
  //
  //  if (O.obstakelLocatie1 < 32 && geland) {
  //
  //      in_air = true;
  //      geland = false;
  //      velocityY = 1.5;
  //      //endJump();
  //    }
// }
}
void Jump::tekenJump(MI0283QT9 lcd) {

  if (in_air) {
    if (velocityY <= 0) {
      lcd.fillRect(32, positionY - 15, 15, 15 , RGB(0, 0, 0));
      lcd.fillRect(32, positionY, 15, last_y - positionY + 1, RGB(255, 255, 255));
    } else if (velocityY > 0 || positionY == 160) {
      lcd.fillRect(32, positionY - 15, 15, 15, RGB(0, 0, 0));
      lcd.fillRect(32, last_y - 15 - 1, 15, (positionY - 15) - (last_y - 15) + 1, RGB(255, 255, 255));
    } else if (positionY > 128) {
      lcd.fillRect(32, positionY - 15, 15, 15, RGB(0, 0, 0));
      lcd.fillRect(32, last_y - 15 - 1, 15, (positionY - 15) - (last_y - 15) + 1, RGB(255, 255, 255));
    }
  }
  //      Serial.println("teken:");
  //      Serial.println(velocityY);
  //
}