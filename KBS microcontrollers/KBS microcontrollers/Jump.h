/*
   Jump.h

   Created: 17-12-2015 12:12:14
    Author: Dono
*/


#ifndef JUMP_H_
#define JUMP_H_
class Jump {
  public:

    uint8_t zbutton = 0;
    double velocityY = 0.0, positionY = 160, last_y, gravity = 0.05;      //gravity variabelen
    bool in_air = false;     //jump booleans

    void checkJump();
    void startJump();
    void endJump();
    void updateJump();
    void tekenJump(MI0283QT9 lcd);
};




#endif /* JUMP_H_ */
