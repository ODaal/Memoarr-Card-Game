#include <iostream>
using namespace std;
#pragma once   //Garantir que la classe est incluse une fois

class Rubis {
private:
    int value;

    //Constructeur privé
    Rubis(int v);

public:
    friend class RubisDeck; // RubisDeck peu créer Rubis

    //conversion vers int
    operator int() const { return value; }

    //Methode d'affichage
    friend ostream& operator<<(ostream& os, const Rubis& r);
};