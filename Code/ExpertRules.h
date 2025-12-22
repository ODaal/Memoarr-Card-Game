#pragma once

#include "Rules.h"
#include "Game.h"
#include "Board.h"
#include "Card.h"


class ExpertRules : public Rules {
public:
    ExpertRules() = default;

    // Aprés qu'une carte est revélée; active le pouvoir de la carte
    void afterReveal(Game& g, Side /*current*/, Letter L, Number N);
};
