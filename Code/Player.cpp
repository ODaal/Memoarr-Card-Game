#include "Player.h"
#include "rubis.h"

//---------------------------------
//  Methode d'affichage
//---------------------------------

ostream& operator<<(ostream& os, const Player& p) {
    if(!p.isEnd){
        string sideName;
        switch (p.side) {              //Affiche le coté du joueur
            case 0: sideName = "top"; break;
            case 1: sideName = "bottom"; break;
            case 2: sideName = "left"; break;
            case 3: sideName = "right"; break;
        }
        os << p.name << ": " << sideName << " " << "(" << (p.state ? "active" : "not active") << ")" << endl; //Affiche coté et l'état
    }else{
        os << p.name << ": " << p.nrubis << " rubis" << endl; //Si fin affiche nombre de rubis
    }
    return os;
}