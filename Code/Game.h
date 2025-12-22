#pragma once

#include <iostream>
#include <stdexcept>
#include "Board.h"
#include "Player.h"
#include "Card.h"



class Game {
private:
    Board board;
    int   round;                  
    Side  currentTurn;            
    Player* players[4];           

    const Card* previousCard;
    const Card* currentCard;

    // Pour mode expert
    bool expertDisplay = false;  //Affichage expert

    bool forceRepeat   = false;  // Crabe
    bool skipNext      = false;  // Tortue

    bool bannedSet     = false;  // walrus
    Letter bannedL;
    Number bannedN;

    void printExpertBoard(std::ostream& os) const; // Afficher la board expert

public:
    /////////////////////////// Fonction Demandée ///////////////////////////////
    Game();

    int getRound() const { return round; }

    void addPlayer(const Player& p);
    Player& getPlayer(Side s) const;

    const Card* getPreviousCard() const { return previousCard; }
    const Card* getCurrentCard() const  { return currentCard; }

    void setCurrentCard(const Card* card);

    Card* getCard(const Letter& L, const Number& N);
    void setCard(const Letter& L, const Number& N, Card* card);

    const Board& getBoard() const { return board; }

    Side getCurrentTurn() const { return currentTurn; }
    void setCurrentTurn(Side s) { currentTurn = s; } //Optionnal
    void nextTurn();       // avance au prochain côté
    void nextRound() { ++round; }

    //////////////////////////// Pour l'affichage/////////////////////////////
    friend std::ostream& operator<<(std::ostream& os, const Game& g);

    ////////////////////////////////////////Optionnel/////////////////////////////////
    void threeCardsDisplay(Side s);

    //Pour mode Expert
    void setExpertDisplay(bool b) { expertDisplay = b; }
    bool getExpertDisplay() const { return expertDisplay; }

    // ----- Crabe / Tortue helpers -----
    void setForceRepeat(bool b) { forceRepeat = b; }
    bool getForceRepeat() const { return forceRepeat; }

    void setSkipNext(bool b) { skipNext = b; }
    bool getSkipNext() const { return skipNext; }

    // ----- Walrus helpers -----
    void setBannedNext(Letter L, Number N) {
        bannedSet = true;
        bannedL   = L;
        bannedN   = N;
    }
    void clearBannedNext() { bannedSet = false; }
    bool hasBannedNext() const { return bannedSet; }
    Letter getBannedLetter() const { return bannedL; }
    Number getBannedNumber() const { return bannedN; }



    // ----- Extra Wrappers de Board-----
    bool isFaceUp(const Letter& L, const Number& N) const {
        return board.isFaceUp(L, N);
    }

    void faceDown(const Letter& L, const Number& N) {
        board.turnFaceDown(L, N);
    }

    void faceUp(const Letter& L, const Number& N) {
        board.turnFaceUp(L, N);
    }

    void allFacesDown() {
        board.allFacesDown();
    }

    void resetPairState() {
        previousCard = nullptr;
        currentCard = nullptr;
    }

    bool getExpertDisplayFlag() const { return expertDisplay; } // optional


    
};

