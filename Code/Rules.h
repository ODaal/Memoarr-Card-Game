#pragma once

#include "Game.h"
#include "Player.h"
#include "Card.h"
#include <stdexcept>

class Rules {
public:
    bool isValid(const Game& game) const;
    bool gameOver(const Game& game) const;
    bool roundOver(const Game& game) const;
    const Player& getNextPlayer(const Game& game) const;

};
