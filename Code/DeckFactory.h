// DeckFactory.h
#pragma once   //Garantir que la classe est incluse une fois
#include <vector>
#include <algorithm>  // contient std::random_shuffle

template <typename C>
class DeckFactory {

protected:  // Pour que les classes derivés aient accées
    std::vector<C> items;
    std::size_t pos = 0; //Position de l'item

public:
    virtual ~DeckFactory() = default; //Destructeur pour la classe et la classe dérivée

    // Mélange le paquet
    void shuffle() {
        std::random_shuffle(items.begin(), items.end()); // Shuffle
        pos = 0;
    }

    // retourne un pointeur vers l’élément suivant, ou nullptr si vide
    C* getNext() {
        if (pos >= items.size()) return nullptr;
        pos++;
        return &items[pos];
    }

    // vrai si plus rien à distribuer
    bool isEmpty() const {
        return pos >= items.size();
    }

    /////////////
    //Optionnel//
    /////////////
    //Getter d'une position donnée
    C* getAt(int pos) const {
        if (pos >= items.size()) return nullptr;
        return &items[pos];
    }
};
