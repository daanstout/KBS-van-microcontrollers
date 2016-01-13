#ifndef OPMAAK_H_
#define OPMAAK_H_

class Opmaak{
    private:
    uint8_t randomObstakels = 0, randomObstakelVorm, nieuwObstakel, obstakelVorm2 = 0;    //level generatie variabelen
    uint16_t  vorigeObstakel1, vorigeObstakel2;      //obstakel variabelen
    public:
    uint8_t  aantalObstakels = 0, obstakelVorm1;
    uint16_t obstakelLocatie1, obstakelLocatie2;
    bool scoreUpdate = false;
    
    void sidescroll(MI0283QT9 lcd);
    void randomLevel();
    void teken(MI0283QT9 lcd);
    void drawVierkant(int x, MI0283QT9 lcd);
    void resetVierkant(int x, MI0283QT9 lcd);
    void drawDriehoek(int x, MI0283QT9 lcd);
    void resetDriehoek(int x, MI0283QT9 lcd);
    void speler(MI0283QT9 lcd);
    void tekenLijn(MI0283QT9 lcd);
    void drawMoeilijkheid(MI0283QT9 lcd);
};

#endif
