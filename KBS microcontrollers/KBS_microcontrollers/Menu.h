#ifndef MENU_H_
#define MENU_H_

struct Score {
  uint16_t punten;
  char letter1;
  char letter2;
  char letter3;
};

class Menu {
  private:
    //namen:
    //beide voor de score
    char eerste = 'A', tweede = 'B', derde = 'C';
    String eerste2, tweede2, derde2;
    uint8_t rank = 0;

  public:
    uint8_t buttonPressed = 0, multiplayerWinner;      //menu variabelen
    //game variabelen
    bool firstTime = true, toCheckButton = true, howToContinue = false;        //menu booleans
    bool scoresBack = false, scoreSubmit = false, charVerandering = false, gameStart = false;     //menu booleans
    uint16_t score2, score;
    //highscores
    uint8_t eeAdress = 0, scherm = 0;
    Score nummer1, nummer2, nummer3, nummer4, nummer5, kleinste, nummer;
    char buf[4];
    bool veranderd = true;

    void incScore();
    void drawScores(MI0283QT9 lcd);
    void tekenVak1(MI0283QT9 lcd);
    void tekenVak2(MI0283QT9 lcd);
    void tekenVak3(MI0283QT9 lcd);
    void inputScore(MI0283QT9 lcd);
    void drawMenu(MI0283QT9 lcd);
    void checkButtonPress(MI0283QT9 lcd);
    void howToNext(MI0283QT9 lcd);
    void howTo(MI0283QT9 lcd);
    void compare();
    uint16_t getterScore();
    void setterScore(uint16_t x);
    //EEPROM
    void emptyEEPROM();
    void saveScore();
    void getScore();
    void sortScore();
    void printScore(MI0283QT9 lcd);
};



#endif /* MENU_H_ */
