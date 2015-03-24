//
// Created by dadou on 20/03/15.
//

#include "generique_pointer.hpp"

long generique_pointer::long_auto_inc = 0;

long generique_pointer::inc_compteur() {
    return ++(long_auto_inc);
}
