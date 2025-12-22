#pragma once   // Garantir que la classe est incluse une fois

#include <iostream>
#include <stdexcept>
#include "Card.h"
#include "CardDeck.h"

enum Letter { A, B, C, D, E };
enum Number { I, II, III, IV, V };

// Exception pour manque de cartes
class NoMoreCards : public std::runtime_error {
public:
    NoMoreCards() : std::runtime_error("Plus de carte pour construire la board") {}
};

class Board {
private:
    Card* tableau[5][5];   // pointeurs vers les cartes
    bool  faceUp[5][5];    // true = visible, false = cachée

    // Vérifie les bornes ET interdit la case centrale (C,III)
    void checkInRange(const Letter& L, const Number& N) const;

public:
    // --------- Constructeur ---------
    Board();

    // --------- Méthodes demandées ---------
    bool isFaceUp(const Letter& L, const Number& N) const;
    bool turnFaceUp(const Letter& L, const Number& N);
    bool turnFaceDown(const Letter& L, const Number& N);
    Card* getCard(const Letter& L, const Number& N);
    void setCard(const Letter& L, const Number& N, Card* card);
    void allFacesDown();

    // --------- Personelle ---------
    bool allCardsFaceUp() const;

    // --------- Affichage simple ---------
    friend std::ostream& operator<<(std::ostream& os, const Board& b);
};
