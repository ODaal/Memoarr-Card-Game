/*Player.cpp; Project; CSI2772A*/

#include <iostream>
using namespace std;
#include "Rubis.h"
#pragma once   //Garantir que la classe est incluse une fois

//---------------------------------
//  Enumerations
//---------------------------------
enum Side{top, bottom, left, right};

//---------------------------------
//  Class
//---------------------------------
class Player {
    string name;         // Nom du joueur
    bool state = true;   // Etat du joueur
    int nrubis = 0;      // Nombre de rubis
    Side side;           // Coté
    bool isEnd = false;  // Fin du jeu ?

//---------------------------------
//  Methodes publique
//---------------------------------  

public:
    Player() = default;
    Player(string player_name, Side player_side, int player_rubis = 0): name(std::move(player_name)), nrubis(player_rubis), side(player_side){}  //constructor

    string getName() const { return name; }
    void setActive(bool b) { state = b; }
    bool isActive() const { return state; }
    int getNRubies() const { return nrubis; }
    void addRubis(const Rubis& r) { nrubis += r; }
    void setDisplayMode(bool endOfGame) { isEnd = endOfGame; }
    Side getSide() const { return side; }
    void setSide(Side s) { side = s; }


    friend ostream& operator<<(ostream& os, const Player& p);
};



