#ifndef MENU_H_
#define MENU_H_

class Menu{
	private:
	//namen:
	//beide voor de score
	char eerste = 'A', tweede = 'B', derde = 'C';
	String eerste2, tweede2, derde2;
	uint8_t rank = 0; 
	
	public:
	uint8_t buttonPressed = 0, multiplayerWinner;      //menu variabelen
	uint16_t score, score2;     //game variabelen
	bool firstTime = true, toCheckButton = true, howToContinue = false;        //menu booleans
	bool scoresBack = false, scoreSubmit = false, charVerandering = false, gameStart = false, postGame = false;     //menu booleans

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
};



#endif /* MENU_H_ */
