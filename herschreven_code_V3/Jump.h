/*
 * Jump.h
 *
 * Created: 17-12-2015 12:12:14
 *  Author: Dono
 */ 


#ifndef JUMP_H_
#define JUMP_H_

class Jump {
	public:
	uint8_t zbutton;	// Defines the "gamestate for the while loop. 0 = Main Menu, 1 = Highscores, 2 = How To, 3 = Playing the game
	double velocityY = 0.0, positionY = 160.0, last_y, gravity = 0.05;      //gravity variabelen
	bool in_air = false;     //jump booleans
	
	void checkJump();
	void startJump();
	void endJump();
	void update();
};




#endif /* JUMP_H_ */