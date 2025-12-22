// CardDeck.cpp
#include "CardDeck.h"

CardDeck::CardDeck() {
    for (auto a : {Crab, Penguin, Octopus, Turtle, Walrus})
        for (auto b : {Red, Green, Purple, Blue, Yellow})
            items.push_back(Card(a, b));   // Crée 25 cartes pour tout le deck

    shuffle();
}

CardDeck& CardDeck::make_CardDeck() {
    static CardDeck instance;   // Créé une fois, réutilisé
    return instance;
}