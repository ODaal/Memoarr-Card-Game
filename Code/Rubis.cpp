#include "Rubis.h"
#include <iostream>
using namespace std;

Rubis::Rubis(int v) {
    if (v < 1 || v > 4)
        value = 1;    // valeur par défaut
    else
        value = v;
}

ostream& operator<<(ostream& os, const Rubis& r) { // Affiche le nombre de rubis
    os << r.value << " rubi" << (r.value>1 ? "s" : "");
    return os;
}