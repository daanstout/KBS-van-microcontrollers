#ifndef GAME_H_
#define GAME_H_

class Game{
  private:
  public:
  bool gameIsLive = false, death = false, geland = false;     //game booleans
  uint8_t currentY = 160;     //draw variabelen
  uint8_t moeilijkheid = 255;    //game variabelen
  
  void game(MI0283QT9 lcd);
  void hitbox();
  
};







#endif
