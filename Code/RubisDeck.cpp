#include "RubisDeck.h"

RubisDeck::RubisDeck() {  // Ajoutes tous les rubis et shuffle
    items.push_back(Rubis(1));
    items.push_back(Rubis(1));
    items.push_back(Rubis(1));
    items.push_back(Rubis(2));
    items.push_back(Rubis(2));
    items.push_back(Rubis(3));
    items.push_back(Rubis(4));
    shuffle();
}

RubisDeck& RubisDeck::make_RubisDeck() {
    static RubisDeck instance;   // Créé une fois, réutilisé
    return instance;
}
