#include "ExpertRules.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>

// Helper pour checker que les coordonées entrées sont valides

static bool parseCoord(const std::string& raw,
                       Letter& L,
                       Number& N) {
    std::string s = raw;
    // remove whitespace
    s.erase(std::remove_if(s.begin(), s.end(),[](unsigned char ch){ return std::isspace(ch); }),s.end());

    if (s.size() < 2) return false; // Minimum 2 char

    //Convertir l'entrée
    char rowChar = (char)std::toupper((unsigned char)s[0]);
    std::string numPart = s.substr(1);
    int colNum = 0;
    try {
        colNum = std::stoi(numPart);
    } catch (...) {
        return false;
    }

    //Check In Range
    if (rowChar < 'A' || rowChar > 'E') return false;
    if (colNum < 1 || colNum > 5)      return false;
    if (rowChar == 'C' && colNum == 3) return false; // centre interdit

    L = (Letter)(rowChar - 'A');
    N = (Number)(colNum - 1);
    return true;
}


void ExpertRules::afterReveal(Game& g, Side /*current*/, Letter L, Number N) {
    Card* c = g.getCard(L, N);
    FaceAnimal a = *c; 

    // === CRABE : Rejoue ==================================
    if (a == FaceAnimal::Crab) {
        std::cout << "Pouvoir Crabe: vous jouez encore!\n";
        g.setForceRepeat(true);
    }

    // === OCTOPUS : Swap avec carte adjacente =======================
    if (a == FaceAnimal::Octopus) {
        std::cout << "Pouvoir pieuvre: Choisissez une carte adjacente (haut/bas/gauche/droite) pour échanger!\n"
                     "Entrée vide pour ignorer.\n";

        while (true) {
            std::string s;
            std::getline(std::cin, s);

            // Ignorer si entrée
            if (s.empty()) {
                break;
            }

            Letter L2;
            Number N2;

            if (!parseCoord(s, L2, N2)) {
                std::cout << "Coordonnée invalide. Réessayez (ou Entrée pour ignorer).\n";
                continue;
            }

            int r0 = L;
            int c0 = N;
            int r  = L2;
            int c  = N2;

            // Orthogonaux
            if (std::abs(r - r0) + std::abs(c - c0) != 1) {
                std::cout << "Position non adjacente. Réessayez (ou Entrée pour ignorer).\n";
                continue;
            }

            // Get cards
            Card* A = g.getCard(L,  N);
            Card* B = g.getCard(L2, N2);

            // Get Etat des cartes
            bool A_face = g.isFaceUp(L,  N);
            bool B_face = g.isFaceUp(L2, N2);

            // Swap Les Cartes
            g.setCard(L,  N,  B);
            g.setCard(L2, N2, A);

            // Garde les mêmes états de la carte
            if (B_face) g.faceUp(L, N);  else g.faceDown(L, N);
            if (A_face) g.faceUp(L2, N2); else g.faceDown(L2, N2);

            std::cout << "Échange effectué.\n";
            break;
        }
    }

    // === TURTLE : Skip le tourne du prochain joueur=========================
    if (a == FaceAnimal::Turtle) {
        std::cout << "Pouvoir tortue: le prochain joueur saute son tour!\n";
        g.setSkipNext(true);
    }

    // === WALRUS : Interdire une carte =====================
    if (a == FaceAnimal::Walrus) {
        std::cout << "Pouvoir morse: interdisez une case au prochain joueur (ex: B3).\n"
                     "Entrée vide pour ignorer.\n";

        while (true) {
            std::string s;
            std::getline(std::cin, s);

            if (s.empty()) {
                break;
            }

            Letter Lb;
            Number Nb;
            if (!parseCoord(s, Lb, Nb)) {
                std::cout << "Coordonnée invalide. Réessayez (ou Entrée pour ignorer).\n";
                continue;
            }

            g.setBannedNext(Lb, Nb);
            std::cout << "Case (" << char('A' + (int)Lb) << (int)Nb + 1
                      << ") interdite pour le prochain joueur.\n";
            break;
        }
    }

    // === PENGUIN : Flipe une carte visible===========================
    if (a == FaceAnimal::Penguin) {
        std::cout << "Pouvoir pingouin: vous pouvez renverser une carte visible!\n"
                     "Entrée vide pour ignorer.\n";

        while (true) {
            std::string s;
            std::getline(std::cin, s);

            if (s.empty()) {
                break;
            }

            Letter L2;
            Number N2;

            if (!parseCoord(s, L2, N2)) {
                std::cout << "Coordonnée invalide. Réessayez (ou Entrée pour ignorer).\n";
                continue;
            }

            if (!g.isFaceUp(L2, N2)) {
                std::cout << "Cette carte n'est pas visible. Réessayez (ou Entrée pour ignorer).\n";
                continue;
            }

            g.faceDown(L2, N2);
            std::cout << "Carte renversée.\n";
            break;
        }
    }
}
