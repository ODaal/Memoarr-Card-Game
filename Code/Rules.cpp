#include "Rules.h"

// Paire si carte précédente et courante ont même animal ou même couleur
bool Rules::isValid(const Game& game) const {
    const Card* prev = game.getPreviousCard();
    const Card* curr = game.getCurrentCard();

    if (!prev || !curr) {
        return false;
    }

    // Utilisation des opérateurs de conversion de Card:

    FaceAnimal aPrev = static_cast<FaceAnimal>(*prev);
    FaceAnimal aCurr = static_cast<FaceAnimal>(*curr);

    FaceBackground cPrev = static_cast<FaceBackground>(*prev);
    FaceBackground cCurr = static_cast<FaceBackground>(*curr);

    return (aPrev == aCurr) || (cPrev == cCurr);
}

// Vrai si le nombre de manches est rendu à 7
bool Rules::gameOver(const Game& game) const {
    return game.getRound() >= 7;
}

// Vrai s’il ne reste qu’UN seul joueur actif
bool Rules::roundOver(const Game& game) const {
    int activeCount = 0;

    // On parcourt les 4 côtés possibles
    for (int i = 0; i < 4; ++i) {
        Side s = static_cast<Side>(i);
        try {
            Player& p = game.getPlayer(s);
            if (p.isActive()) {
                ++activeCount;
            }
        } catch (const std::out_of_range&) {
            // Aucun joueur sur ce côté → on ignore
        }
    }

    return activeCount <= 1;
}

// Retourne le prochain joueur ACTIF à jouer
const Player& Rules::getNextPlayer(const Game& game) const {
    // On part du currentTurn du jeu
    Side current = game.getCurrentTurn();
    int startIdx = static_cast<int>(current);

    // On cherche le prochain joueur ACTIF en tournant en rond
    for (int offset = 0; offset < 4; ++offset) {
        int idx = (startIdx + offset) % 4;
        Side s = static_cast<Side>(idx);

        try {
            const Player& p = game.getPlayer(s);
            if (p.isActive()) {
                return p;
            }
        } catch (const std::out_of_range&) {
            // Pas de joueur sur ce côté → on continue
        }
    }

    throw std::out_of_range("Plus de joueur Acitfs");
}
