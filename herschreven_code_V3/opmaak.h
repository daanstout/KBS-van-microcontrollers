#ifndef OPMAAK_H_
#define OPMAAK_H_

class Opmaak{
    private:
    
    public:
    uint8_t randomObstakels = 0, randomObstakelVorm, aantalObstakels = 0, nieuwObstakel, obstakelVorm1 = 0, obstakelVorm2 = 0;    //level generatie variabelen
    uint16_t obstakelLocatie1, obstakelLocatie2, vorigeObstakel1, vorigeObstakel2;      //obstakel variabelen
    
    void sidescroll(MI0283QT9 lcd);
    void randomLevel();
    void teken(MI0283QT9 lcd);
    void drawVierkant(uint16_t x, MI0283QT9 lcd);
    void resetVierkant(uint16_t x, MI0283QT9 lcd);
    void drawDriehoek(uint16_t x, MI0283QT9 lcd);
    void resetDriehoek(uint16_t x, MI0283QT9 lcd);
    void speler(MI0283QT9 lcd);
    void tekenLijn(MI0283QT9 lcd);
};

#endif
