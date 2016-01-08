#ifndef GAME_H_
#define GAME_H_
#include "Jump.h"
#include "Opmaak.h"

class Game{
  private:
  public:
  bool gameIsLive = false, death = false, geland, lucht;     //game booleans
  uint8_t currentY = 160;     //draw variabelen
  uint8_t moeilijkheid = 255, vormObstakel1;    //game variabelen
  uint16_t locatieObstakel1;
  double yPosition;
  double yVel;
  
  
  void game(MI0283QT9 lcd);
  void hitbox(Opmaak P, Jump U);
  
};
#endif
