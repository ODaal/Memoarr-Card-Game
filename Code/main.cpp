#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <limits>
#include <cctype>
#include <ctime>
#include <cstdlib>
#include <string>
#include <memory>   // std::unique_ptr

#include "Game.h"
#include "Rules.h"
#include "ExpertRules.h"
#include "Player.h"
#include "Board.h"
#include "Card.h"
#include "CardDeck.h"
#include "RubisDeck.h"
#include "Rubis.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////// Helpers ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Trim un string
static string trim(const string& s) {
    size_t start = 0;
    while (start < s.size() && isspace((unsigned char)s[start])) ++start;
    size_t end = s.size();
    while (end > start && isspace((unsigned char)s[end - 1])) --end;
    return s.substr(start, end - start);
}


// Demande Oui ou Non
static bool askYesNo(const string& prompt) {
    while (true) {
        cout << prompt << " (o/n): ";
        string s;
        if (!getline(cin, s)) return false;
        s = trim(s);
        if (s == "o" || s == "O" || s == "y" || s == "Y") return true;
        if (s == "n" || s == "N") return false;
        cout << "Veuillez répondre par o ou n.\n";
    }
}

// Check qu'une valeur est dans une intervalle propre
static int askIntInRange(const string& prompt, int minVal, int maxVal) {
    while (true) {
        cout << prompt;
        string s;
        if (!getline(cin, s)) return minVal;
        s = trim(s);
        try {
            int v = stoi(s);
            if (v >= minVal && v <= maxVal) return v;
        } catch (...) {}
        cout << "Entrée invalide. Veuillez entrer un nombre entre "
             << minVal << " et " << maxVal << ".\n";
    }
}

// Meme logique utilisé dans Plqyer
static string sideToString(Side s) {
    switch (s) {
        case Side::top:    return "top";
        case Side::bottom: return "bottom";
        case Side::left:   return "left";
        case Side::right:  return "right";
    }
    return "?";
}

// Demande le Coté du Joueur
static Side askSideForPlayer(const string& playerName,
                             const map<string, Side>& str2side,
                             vector<Side>& chosen) {
    while (true) {
        cout << playerName << " Côté (top/bottom/left/right): ";
        string s;
        if (!getline(cin, s)) return Side::top;
        s = trim(s);
        for (auto& ch : s) ch = (char)tolower((unsigned char)ch);

        auto it = str2side.find(s);
        if (it == str2side.end()) {
            cout << "Entrée invalide.\n";
            continue;
        }
        Side pick = it->second;
        if (find(chosen.begin(), chosen.end(), pick) != chosen.end()) {
            cout << "Ce côté est déjà pris!\n";
            continue;
        }
        chosen.push_back(pick);
        return pick;
    }
}

// Check les Coordonnés (Aussi utilisé dans ExpertRules)
static bool checkParsing(const string& rawInput,
                         Letter& L,
                         Number& N,
                         Board& board) {
    string s = rawInput;
    // remove whitespace
    s.erase(remove_if(s.begin(), s.end(),
                      [](unsigned char ch){ return isspace(ch); }),
            s.end());

    if (s.size() < 2) return false; // Minimu 2 char

    // Conversion
    char rowChar = (char)toupper((unsigned char)s[0]);
    string numPart = s.substr(1);
    int colNum = 0;

    try {
        colNum = stoi(numPart);
    } catch (...) {
        return false;
    }

    // Intervalle valide
    if (rowChar < 'A' || rowChar > 'E') return false;
    if (colNum < 1 || colNum > 5)      return false;
    if (rowChar == 'C' && colNum == 3) return false; // centre interdit

    L = (Letter)(rowChar - 'A');
    N = (Number)(colNum - 1);

    // Laisser Board valider via l'exception OutOfRange
    try {
        board.isFaceUp(L, N);
    } catch (...) {
        return false;
    }
    return true;
}

// Demande les Coordonéé et vérifie pourLe Morse
static void askCoord(const string& prompt,
                     Board& board,
                     Game& game,
                     Letter& L,
                     Number& N) {
    while (true) {
        cout << prompt;
        string input;
        if (!getline(cin, input)) return;

        if (!checkParsing(input, L, N, board)) {
            cout << "Coordonnée invalide.\n";
            continue;
        }

        // Case interdite par le morse ?
        if (game.hasBannedNext() &&
            L == game.getBannedLetter() &&
            N == game.getBannedNumber()) {
            cout << "Case interdite par le morse. Choisissez-en une autre.\n";
            continue;
        }

        if (board.isFaceUp(L, N)) {
            cout << "Cette carte est déjà visible. Choisissez-en une autre.\n";
            continue;
        }

        // Choix valide -> on consomme l'interdiction
        if (game.hasBannedNext()) {
            game.clearBannedNext();
        }
        break;
    }
}

// Prochain Joueur prêt pour la ronde (EN ROTATION)
static Side nextActiveSide(const Game& game,
                           const vector<Side>& chosen,
                           Side cur) {
    if (chosen.empty()) return cur;

    int idx = 0;
    for (int i = 0; i < (int)chosen.size(); ++i) {
        if (chosen[i] == cur) { idx = i; break; }
    }
    for (int offset = 1; offset <= (int)chosen.size(); ++offset) {
        Side cand = chosen[(idx + offset) % chosen.size()];
        try {
            const Player& p = game.getPlayer(cand);
            if (p.isActive()) return cand;
        } catch (...) {
            // Ignore
        }
    }
    return cur; // fallback
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////// Main ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main() {
    cin.tie(nullptr);
    srand((unsigned)time(nullptr));

    cout << "=== Memoarr ===\n";

    // Demande le Mode
    bool useExpertDisplay = askYesNo("Mode affichage expert ?");
    bool useExpertRules   = askYesNo("Mode règles expert ?");

    // Nombre de joueurs
    int nPlayers = askIntInRange("Nombre de joueurs (2–4): ", 2, 4);

    // Noms des Joueurs
    vector<string> names(nPlayers);
    cout << "Entrez les noms des joueurs:\n";
    for (int i = 0; i < nPlayers; ++i) {
        cout << "Nom du joueur " << (i + 1) << ": ";
        getline(cin, names[i]);
        if (names[i].empty())
            names[i] = string("Joueur ") + to_string(i + 1);
    }

    // Choix des côtés
    map<string, Side> str2side{
        {"top",    Side::top},
        {"bottom", Side::bottom},
        {"left",   Side::left},
        {"right",  Side::right}
    };

    vector<Side> chosen;
    chosen.reserve(nPlayers);
    vector<Side> playerSides(nPlayers);

    cout << "Assignez un côté à chacun (top/bottom/left/right):\n";
    for (int i = 0; i < nPlayers; ++i) {
        playerSides[i] = askSideForPlayer(names[i], str2side, chosen);
    }

    int startIndex = 0;   // INDEX DU JOUEUR QUI VA JOUER

    // Decks

    // Board construit CardDeck
    RubisDeck& rd = RubisDeck::make_RubisDeck();

    Game  game;
    game.setExpertDisplay(useExpertDisplay); // Mode Expert ?

    // Rules (base ou expert)
    std::unique_ptr<Rules> rules;
    ExpertRules* expertRules = nullptr;

    if (useExpertRules) {
        auto er = std::make_unique<ExpertRules>(); //Unique
        expertRules = er.get();
        rules = std::move(er);
    } else {
        rules = std::make_unique<Rules>(); //Unique
    }

    // PLace les joueurs
    vector<Player> players;
    players.reserve(nPlayers);
    for (int i = 0; i < nPlayers; ++i) {
        players.emplace_back(names[i], playerSides[i], 0);
        game.addPlayer(players.back());
    }

    cout << "\nConfiguration terminée. Plateau initial :\n\n";
    cout << game << "\n";

    // Fini de Configurer

    // 5) Boucle des manches
    while (!rules->gameOver(game)) {

        Board& board = const_cast<Board&>(game.getBoard());

        cout << "--- Manche " << (game.getRound() + 1) << " ---\n";

        game.allFacesDown();         // reset visibilité
        game.resetPairState();       // reset previous/current cards

        for (Side s : chosen) {
            game.getPlayer(s).setActive(true);
        }

        // Aperçu de 3 cartes devant chaque joueur
        cout << "(Aperçu de 3 cartes devant chaque joueur)\n";
        for (Side s : chosen) {
            Player& P = game.getPlayer(s);
            cout << "Aperçu pour " << P.getName()
                 << " (" << sideToString(P.getSide())
                 << ") — appuyez Entrée pour continuer.\n";
            game.threeCardsDisplay(P.getSide());
        }

        // Le tour du Joueur
        Side currentPlayer = chosen[startIndex];
        bool boardExhausted = false;

        // Boucle de la manche
        while (!rules->roundOver(game)) {
            Player& P = game.getPlayer(currentPlayer);
            if (!P.isActive()) {
                currentPlayer = nextActiveSide(game, chosen, currentPlayer);
                continue;
            }

            cout << "Au tour de " << P.getName()
                 << " - choisissez une carte à retourner.\n";

            Letter L;
            Number N;
            askCoord("Carte (ex: A1): ", board, game, L, N);

            board.turnFaceUp(L, N);
            Card* c = game.getCard(L, N);
            game.setCurrentCard(c); // gère previous / current

            cout << game << "\n";

            // Vérifier la paire à partir de la 2e carte globale
            if (game.getPreviousCard() != nullptr) {
                if (!rules->isValid(game)) {
                    cout << "Pas de paire — " << P.getName()
                         << " est éliminé pour cette manche.\n";
                    P.setActive(false);
                } else {
                    cout << "C'est une Paire! " << P.getName()
                         << " reste dans la manche.\n";
                }
            } else {
                cout << "Première carte du tour global — aucune vérification de paire.\n";
            }

            // Appliquer les pouvoirs d'animaux si mode expert Sauf si fin de manche ou joueur perd
            if (expertRules) {
                if (!rules->roundOver(game) && P.isActive()) {
                    expertRules->afterReveal(game, currentPlayer, L, N);
                }
            }

            if (board.allCardsFaceUp()) {
                cout << "Toutes les cartes sont visibles. Fin de la manche.\n";
                boardExhausted = true;
                break;
            }

            // Gestion crabe et turtle (repeat / skip)
            bool repeat = useExpertRules && game.getForceRepeat();
            bool skip   = useExpertRules && game.getSkipNext();

            if (repeat) {
                // le même joueur rejoue, on consomme le flag
                game.setForceRepeat(false);
            } else {
                if (skip) {
                    // sauter le prochain joueur actif
                    game.setSkipNext(false);
                    Side s1 = nextActiveSide(game, chosen, currentPlayer);
                    currentPlayer = nextActiveSide(game, chosen, s1);
                } else {
                    currentPlayer = nextActiveSide(game, chosen, currentPlayer);
                }
            }
        }

        // Fin de manche: trouver le gagnant et donner un rubis
        Side  winnerSide = chosen[0];
        bool  found      = false;
        if (!boardExhausted) {
            for (Side s : chosen) {
                if (game.getPlayer(s).isActive()) {
                    winnerSide = s;
                    found = true;
                    break;
                }
            }
        }

        if (boardExhausted) {
            cout << "Aucun gagnant: plateau épuisé.\n";
        } else if (found) {
            Player& W = game.getPlayer(winnerSide);
            Rubis* r = rd.getNext();
            if (!r) {
                cout << "Le paquet de rubis est vide. Aucun rubis distribué.\n";
            } else {
                W.addRubis(*r);
                cout << "Manche remportée par " << W.getName()
                     << ". Récompense: " << *r << "\n";
            }
        } else {
            cout << "Aucun gagnant pour cette manche.\n";
        }

        // Classement partiel
        vector<Player*> ranking;
        for (Side s : chosen) {
            ranking.push_back(&game.getPlayer(s));
        }
        sort(ranking.begin(), ranking.end(),
             [](Player* a, Player* b) {
                 return a->getNRubies() < b->getNRubies();
             });

        cout << "Classement partiel (Croissant):\n";
        for (auto* p : ranking) {
            p->setDisplayMode(true);
            cout << *p << "\n";
            p->setDisplayMode(false);
        }

        game.nextRound();
        cout << "\n";

        // Prochain round commence par le joueur suivant
        startIndex = (startIndex + 1) % chosen.size();
    }

    
    ////// Fin de la partie
    cout << "=== Fin de partie ===\n";

    vector<Player*> ranking;
    for (Side s : chosen) {
        ranking.push_back(&game.getPlayer(s));
    }
    sort(ranking.begin(), ranking.end(),
         [](Player* a, Player* b) {
             return a->getNRubies() > b->getNRubies();
         });

    int best = ranking.front()->getNRubies();

    cout << "Gagnant(s):\n";
    for (auto* p : ranking) {
        if (p->getNRubies() == best) {
            p->setDisplayMode(true);
            cout << *p << "\n";
            p->setDisplayMode(false);
        }
    }

    return 0;
}
