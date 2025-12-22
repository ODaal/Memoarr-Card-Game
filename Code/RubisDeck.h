#pragma once   // Garantir que la classe est incluse une fois

#include "DeckFactory.h"
#include "Rubis.h"

class RubisDeck : public DeckFactory<Rubis> {

private:
    // Constructeur privé
    RubisDeck();

    // Prévenir le copiage
    RubisDeck(const RubisDeck&) = delete;
    RubisDeck& operator=(const RubisDeck&) = delete;

public:
    // Singleton
    static RubisDeck& make_RubisDeck();
};
