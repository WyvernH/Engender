/*
 * analyse.h
 *
 *  Created on: Oct 9, 2023
 *      Author: Matthew Hinton
 */

#ifndef ANALYSE_H_
#define ANALYSE_H_

#include "corpus.h"

#include <algorithm>
#include <stdlib.h>
#include <time.h>

class layout_t {
public:
    char* key;						// key[n] returns the key on which the letter with index n resides
    // letter[n] returns the index of the letter assigned to key n

    long double ngramTimeRecurse(char*, long double, long double, int, int) const;
    long double ngramScore(char* ngram) const; // long long?
    long double score() const; // long long?

    layout_t evolve(unsigned) const;	// Re-permute the layout by a certain degree of permutation

    layout_t () { key = new char [LayoutSize]; }
    layout_t (char k [LayoutSize]) {
        key = new char [LayoutSize];
        memcpy(key, k, LayoutSize);
    }
    layout_t (const layout_t& lyt) {
        key = new char [LayoutSize];
        memcpy(key, lyt.key, LayoutSize);
    }
    layout_t& operator= (const layout_t& lyt) {
        key = new char [LayoutSize];
        memcpy(key, lyt.key, LayoutSize);
        return *this;
    }
    layout_t operator% (const layout_t& permutation) {
        layout_t temp;
        for (int i{ }; i < LayoutSize; i++) {
            temp.key[i] = this->key[(int)permutation.key[i]];
        }
        return temp;
    }
};


inline double digraphScores[LayoutSize][LayoutSize];

#endif /* ANALYSE_H_ */
