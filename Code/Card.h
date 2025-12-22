#include <iostream>
using namespace std;
#pragma once   //Garantir que la classe est incluse une fois

//---------------------------------
//  Enumerations
//---------------------------------
enum FaceAnimal { Crab, Penguin, Octopus, Turtle, Walrus };
enum FaceBackground { Red, Green, Purple, Blue, Yellow };


//---------------------------------
//  Class
//---------------------------------
class Card {
private:
    FaceAnimal animal;
    FaceBackground color;

    Card(FaceAnimal a, FaceBackground c) : animal(a), color(c) {}

//---------------------------------
//  Methodes publics
//---------------------------------
public:
    friend class CardDeck; // seul CardDeck peut créer une carte

    // opérateurs de conversion
    operator FaceAnimal() const { return animal; }
    operator FaceBackground() const { return color; }

    // nombre de lignes
    static int getNRows() { return 3; }

    // opérateur() pour afficher une rangée
    string operator()(int row) const {
        char c = getColorChar();
        char a = getAnimalChar();

        string line(3, c); // 3 charactére qui se suivent
        if (row == 1)
            line[1] = a;

        return line;   
    }

    //---------------------------------
    //  Methodes privés
    //---------------------------------
    private:
    // Getters() simples
    char getColorChar() const {
        switch (color) {
            case Red: return 'r';
            case Green: return 'g';
            case Purple: return 'p';
            case Blue: return 'b';
            case Yellow: return 'y';
        }
        return '?';
    }

    char getAnimalChar() const {
        switch (animal) {
            case Crab:  return 'C';
            case Penguin: return 'P';
            case Octopus: return 'O';
            case Turtle:  return 'T';
            case Walrus:  return 'W';
        }
        return '?';
    }
};
