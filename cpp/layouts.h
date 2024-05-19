/*
 * layouts.h
 *
 *  Created on: Oct 11, 2023
 *      Author: Matthew Hinton
 */

#ifndef LAYOUTS_H_
#define LAYOUTS_H_


#include "analyse.h"

#include <filesystem>
#include <iostream>
#include <string>

class MetaLayout {
    char* letters;
public:
    static int numberof;
    const string name;
    const string creator;
    const layout_t layout;
    /*
    string get_name() { return name; }
    string get_creator() { return creator; }
    layout_t get_layout() { return layout; }
    */
    long double score() const { return layout.score(); }
    char key(int i) const { return layout.key[i]; }
    char letter(int i) const { return letters[i]; }

    MetaLayout(layout_t l) : name("Generated Layout"), creator("Engender keyboard layout generator"), layout(l) {
        letters = new char [LayoutSize];
        for (int i{ }; i < LayoutSize; i++) {
            letters[(int)layout.key[i]] = i;
        }
        numberof++;
    }
    MetaLayout(string n, string c, layout_t l) : name(n), creator(c), layout(l) {
        letters = new char [LayoutSize];
        for (int i{ }; i < LayoutSize; i++) {
            letters[(int)layout.key[i]] = i;
        }
        numberof++;
    }
    ~MetaLayout() {
        numberof--;
    }

}; inline int MetaLayout::numberof;


#endif // LAYOUTS_H_
