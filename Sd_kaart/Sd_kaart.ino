#include <EEPROM.h>

int eeAdress = 0;

struct Score{
  uint16_t score;
  char naam[10];
};

Score nummer1 = {30,"Piet"};
Score nummer2 = {20, "DONO"};

void saveScore(){
  //Serial.println("voor de eeprom");
  EEPROM.put(0, nummer1);
  //Serial.println("STruct is succesvol geschreven");
}
void printScore(){
  EEPROM.get(0, nummer2);
}

int main(){
  init();
  Serial.begin(9600);
//  saveScore();
//  printScore();
  nummer1 = nummer2;
  eeAdress += sizeof(Score);
  Serial.print(eeAdress);
//  eeAdress += sizeof(Score);
//  Serial.print(eeAdress);
//  Serial.print(nummer1.score);
//  Serial.print(nummer1.naam);
  return 0;
}
