// Game.cpp

#include "Game.h"
#include <stdexcept>
#include <limits>


//////////////////////////////////////////// Fonction demandée /////////////////////////////////////////////
Game::Game() {
    for (int i = 0; i < 4; ++i) {
        players[i] = nullptr;
    }
    round        = 0;
    currentTurn  = top;       // on commence par top
    previousCard = nullptr;
    currentCard  = nullptr;
}

void Game::addPlayer(const Player& p) {
    int idx = static_cast<int>(p.getSide());
    if (idx < 0 || idx >= 4) {
        throw std::out_of_range("Invalid side for player");
    }
    players[idx] = const_cast<Player*>(&p);
}

Player& Game::getPlayer(Side s) const {
    int idx = static_cast<int>(s);
    if (idx < 0 || idx >= 4 || players[idx] == nullptr) {
        throw std::out_of_range("No player on this side");
    }
    return *players[idx];
}

void Game::setCurrentCard(const Card* card) {
    previousCard = currentCard;
    currentCard  = card;
}

Card* Game::getCard(const Letter& L, const Number& N) {
    return board.getCard(L, N);
}

void Game::setCard(const Letter& L, const Number& N, Card* card) {
    board.setCard(L, N, card);
}

void Game::nextTurn() {
    int idx = static_cast<int>(currentTurn);
    idx = (idx + 1) % 4;                 // 0→1→2→3→0
    currentTurn = static_cast<Side>(idx);
}

/////////////////////////////////////////////// Affichage Normal ///////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const Game& g) {
    // ----- Board + Carte -----
    if (g.expertDisplay) {
        g.printExpertBoard(os);
        os << "\n";
    } else {
        os << g.board << "\n";
    }

    // ----- Players -----
    for (int i = 0; i < 4; ++i) {
        if (g.players[i]) {            // pointer non nul
            os << *(g.players[i]) << "\n";
        }
    }

    return os;
}





void Game::printExpertBoard(std::ostream& os) const {
    struct Visible {
        Letter L;
        Number N;
        const Card* card;
    };

    std::vector<Visible> vis;
    Board& boardRef = const_cast<Board&>(board); // Car getCard est non-const

    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            // skip centre (C,3) = (2,2)
            if (r == 2 && c == 2) continue;

            Letter L = static_cast<Letter>(r);
            Number N = static_cast<Number>(c);

            if (boardRef.isFaceUp(L, N)) {
                Card* card = boardRef.getCard(L, N);
                if (card) {
                    vis.push_back({ L, N, card });
                }
            }
        }
    }

    if (vis.empty()) {
        os << "(Aucune carte révélée pour l'instant)\n";
        return;
    }

    // Affiche la carte
    for (int sub = 0; sub < 3; ++sub) {
        for (const auto& v : vis) {
            os << v.card->operator()(sub) << " ";
        }
        os << "\n";
    }

    // Coordonnés: A1 D1 B4 D3
    for (const auto& v : vis) {
        char rowChar = 'A' + static_cast<int>(v.L);
        int colNum   = static_cast<int>(v.N) + 1;
        os << rowChar << colNum << "  ";
    }
    os << "\n";
}



////////////////////////////////////// Affiche les 3 cartes devant le joueur + mode Expert //////////////////////////////////////////////

void Game::threeCardsDisplay(Side s) {
    Board& b = const_cast<Board&>(getBoard());

    std::vector<std::pair<Letter, Number>> peek;

    if (s == top || s == bottom) {
        int row = (s == top ? 0 : 4);
        for (int c = 1; c <= 3; ++c) {          // indices 1,2,3
            Letter L = static_cast<Letter>(row);
            Number N = static_cast<Number>(c);
            peek.emplace_back(L, N);
        }
    } else { // left / right
        int col = (s == Side::left ? 0 : 4);
        for (int r = 1; r <= 3; ++r) {          // indices 1,2,3
            Letter L = static_cast<Letter>(r);
            Number N = static_cast<Number>(col);
            peek.emplace_back(L, N);
        }
    }

    // Face UP
    for (auto& p : peek) {
        b.turnFaceUp(p.first, p.second);
    }

    if (!expertDisplay) {
        // AAffichage Normale
        std::cout << *this << "\n";
    } else {
        // Affichage expert juste les 3 cartes
        std::vector<Card*> cards;
        for (auto& p : peek) {
            cards.push_back(getCard(p.first, p.second));
        }

        // 3 rows pqr carte
        for (int sub = 0; sub < 3; ++sub) {
            for (std::size_t i = 0; i < cards.size(); ++i) {
                std::cout << (*cards[i])(sub) << " ";
            }
            std::cout << "\n";
        }

        // Coordonées
        for (auto& p : peek) {
            char rowChar = 'A' + static_cast<int>(p.first);
            int colNum   = static_cast<int>(p.second) + 1;
            std::cout << rowChar << colNum << "  ";
        }
        std::cout << "\n";
    }

    std::cout << "(Appuyez Entrée pour cacher à nouveau)\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Face Down
    for (auto& p : peek) {
        b.turnFaceDown(p.first, p.second);
    }
}



