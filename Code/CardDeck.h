// CardDeck.h
#pragma once   // Garantir que la classe est incluse une fois

#include "DeckFactory.h"
#include "Card.h"

class CardDeck : public DeckFactory<Card> {

private:
    // Constructeur privé
    CardDeck();

    // Prévenir le copiage
    CardDeck(const CardDeck&) = delete;
    CardDeck& operator=(const CardDeck&) = delete;

public:
    // Accès à l’unique instance (singleton)
    static CardDeck& make_CardDeck();
};

