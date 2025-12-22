#include "Board.h"

// ------------------ méthodes privée ------------------
void Board::checkInRange(const Letter& L, const Number& N) const {
    int r = static_cast<int>(L);
    int c = static_cast<int>(N);

    if (r < 0 || r >= 5 || c < 0 || c >= 5) {
        throw std::out_of_range("Position Invalide");
    }

    // Interdire la case centrale (C, III) = (2,2)
    if (r == 2 && c == 2) {
        throw std::out_of_range("Position Invalide");
    }
}


// ------------------ constructeur ------------------

Board::Board() {
    CardDeck& deck = CardDeck::make_CardDeck();

    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {

            // Case centrale : pas de carte, reste vide
            if (r == 2 && c == 2) {
                tableau[r][c] = nullptr;
                faceUp[r][c]  = false;
                continue;
            }

            Card* card = deck.getNext();
            if (card == nullptr) {
                throw NoMoreCards(); // pas assez de cartes
            }
            tableau[r][c] = card;
            faceUp[r][c]  = false;
        }
    }
}


// ------------------ méthodes publiques ------------------


bool Board::isFaceUp(const Letter& L, const Number& N) const {
    checkInRange(L, N);
    return faceUp[static_cast<int>(L)][static_cast<int>(N)];
}

bool Board::turnFaceUp(const Letter& L, const Number& N) {
    checkInRange(L, N);
    int r = static_cast<int>(L);
    int c = static_cast<int>(N);
    if (faceUp[r][c]) {
        return false; // déjà visible
    }
    faceUp[r][c] = true;
    return true;
}

bool Board::turnFaceDown(const Letter& L, const Number& N) {
    checkInRange(L, N);
    int r = static_cast<int>(L);
    int c = static_cast<int>(N);
    if (!faceUp[r][c]) {
        return false; // déjà cachée
    }
    faceUp[r][c] = false;
    return true;
}

Card* Board::getCard(const Letter& L, const Number& N) {
    checkInRange(L, N);
    return tableau[static_cast<int>(L)][static_cast<int>(N)];
}

void Board::setCard(const Letter& L, const Number& N, Card* card) {
    checkInRange(L, N);
    tableau[static_cast<int>(L)][static_cast<int>(N)] = card;
}

void Board::allFacesDown() {
    for (int r = 0; r < 5; ++r)
        for (int c = 0; c < 5; ++c)
            faceUp[r][c] = false;
}

// ------------------ Personel ------------------
// Check que toutes les cartes sont visibles
bool Board::allCardsFaceUp() const {
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            // skip centre (C,3) = (2,2)
            if (r == 2 && c == 2) continue;

            Letter L = static_cast<Letter>(r);
            Number N = static_cast<Number>(c);

            if (!isFaceUp(L, N)) {
                return false;
            }
        }
    }
    return true;
}

// ------------------ opérateur d'affichage ------------------

std::ostream& operator<<(std::ostream& os, const Board& b) {
    // Pour chaque rangée de lettres (A..E)
    for (int r = 0; r < 5; ++r) {
        char rowLetter = 'A' + r;

        // Chaque carte a 3 lignes: 0 (haut), 1 (milieu), 2 (bas)
        for (int sub = 0; sub < 3; ++sub) {
            // Afficher la lettre de rangée seulement sur la ligne du milieu
            if (sub == 1)
                os << rowLetter << ' ';
            else
                os << "  ";

            // Parcourir les 5 colonnes
            for (int c = 0; c < 5; ++c) {

                // Case centrale : toujours vide à l'affichage
                if (r == 2 && c == 2) {
                    os << "    ";   // même largeur qu'une carte (" zzz" ou " XXX")
                    continue;
                }

                if (!b.faceUp[r][c]) {
                    // Carte cachée → "zzz"
                    os << " zzz";
                } else {
                    // Carte visible → utiliser Card::operator()(sub)
                    os << ' ' << (*(b.tableau[r][c]))(sub);
                }
            }
            os << '\n';
        }
        os << '\n';
    }

    // Ligne des numéros de colonnes au bas
    os << "    1   2   3   4   5\n";
    return os;
}
