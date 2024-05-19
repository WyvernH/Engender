/*
 * analyse.cpp
 *
 *  Created on: Oct 9, 2023
 *      Author: Matthew Hinton
 */

#include "analyse.h"

// gcc automatically inlines member functions of classes, I think
long double layout_t::ngramTimeRecurse(char* ngram, long double greatestTime, long double timeSum, int length, int remainder) const {
    long double time;
    for (int i{1}; i < remainder; ++i) {
        time = timeSum + digraphScores[(int)key[(int)ngram[length - remainder]]][(int)key[(int)ngram[length - remainder + i]]];
        if (i == remainder - 1) {
            //printf("End: time = %Lf\n", time); // <-- verbose output not needed in complete program
            if (time > greatestTime) {
                greatestTime = time;
            }
        } else {
            greatestTime = ngramTimeRecurse(ngram, greatestTime, time, length, remainder - i);
        }
    }
    return greatestTime;
}
// put in header file with inline if inline is not automatically enabled
long double layout_t::ngramScore(char* ngram) const {
    /*
    { // verbose output not needed in complete program
        long double gtime = ngramTimeRecurse(ngram, 0, 0, ngramLength, ngramLength);
        printf("Greatest Time: %Lf\n", gtime);

        long double gnm1time = ngramTimeRecurse(ngram, 0, 0, ngramLength - 1, ngramLength - 1);
        printf("Greatest n-1 Time: %Lf\n", gnm1time);

        printf("Time Difference: %Lf\n", gtime - gnm1time);
    }
    */
    return ngramTimeRecurse(ngram, 0, 0, ngramLength, ngramLength) - ngramTimeRecurse(ngram, 0, 0, ngramLength - 1, ngramLength - 1);
}

long double layout_t::score() const {
    if (ngramData == nullptr) throw runtime_error("Corpus not loaded in program\n");

    long double score{ };
    for (long long unsigned i{ }; i < ngram_t::ngramCount; i++) {
        score += ngramData[i].frequency * ngramScore(ngramData[i].ngram);
    }

    return score / ngram_t::ngramCount;
}


layout_t alphabetical_layout() {
    layout_t alpha_layout;
    for (unsigned i{ }; i < LayoutSize; i++)
        alpha_layout.key[i] = i;
    return alpha_layout;
}





// note: this function takes a random set of 'degree_of_permutation' keys to randomly shuffle. It does not guarantee
// that any number of keys may be shuffled (i.e. the resulting layout may even be unchanged from the original).
// also note: the zeroth key of the layout (typically a space) is off-limits to this repermutation function.
layout_t layout_t::evolve(unsigned degree_of_permutation) const {

    layout_t repermuted(key);

    if (degree_of_permutation >= LayoutSize) cout << "Layout evolution called with illogical degree of permutation (" << degree_of_permutation << "), layout not repermuted"<< endl;
    else if (degree_of_permutation > 1) {

        int from[LayoutSize];
        for (unsigned i{ }; i < LayoutSize; i++) {
            from[i] = i;
        }

        // get a random set of 'degree' elements of a layout
        int choose[degree_of_permutation] = {0};

        unsigned index;
        for (unsigned i{ }; i < degree_of_permutation; i++) {
            srand(time(NULL));
            index = rand() % (LayoutSize - i - 1) + 1; // index cannot equal zero (exclude space key)
            choose[i] = from[index];
            for (unsigned j{index}; j < LayoutSize - 1; j++)
                from[j] = from[j + 1];
        }
        // randomise permutation
        srand(time(NULL));
        for (unsigned i = degree_of_permutation - 1; i > 1; i--) {
            srand(time(NULL));
            if (repermuted.key[choose[i]] == key[choose[i]]) {
                swap(repermuted.key[choose[i]], repermuted.key[choose[rand() % i]]);
            } else swap(repermuted.key[choose[i]], repermuted.key[choose[rand() % (i+1)]]);
        }
        if (repermuted.key[choose[1]] == key[choose[1]]) {
            swap(repermuted.key[choose[1]], repermuted.key[choose[0]]);
        } else if (repermuted.key[choose[0]] == key[choose[0]]) {
            swap(repermuted.key[choose[1]], repermuted.key[choose[0]]);
        } else {
            srand(time(NULL));
            swap(repermuted.key[choose[1]], repermuted.key[choose[rand() % 2]]);
        }
    }

    return repermuted;
}







/*
layout_t layout_t::evolve(int degree_of_permutation) {

	int permutationKeyIndexes[degree_of_permutation];
	int permutationLetterIndexes[degree_of_permutation];	// The letters on the keys in permutationKeyIndexes, but to be scrambled
	int randIndex{ };

	// Pick the keys to permute and store their indexes in permutationKeyIndexes. Store the indexes of the letters of those keys in permutationLetterIndexes.
	initializeKeyIndexVector(LayoutSize);
	for (int i{ }; i < degree_of_permutation; i++) {
		randIndex = seedRandomIndex();
		permutationKeyIndexes[i] = randIndex;
		keyIndexVector.erase(keyIndexVector.begin() + randIndex - 1);		// Find some more efficient way of storing the index info ???
	}

	// Scramble the letters of permutationKeyIndexes and put them in permutationLetterIndexes
	initializeKeyIndexVector(degree_of_permutation);
	for (int c{ }; c < degree_of_permutation; c++) {
		randIndex = seedRandomIndex();
		permutationLetterIndexes[randIndex] = letter[permutationKeyIndexes[c]];
		keyIndexVector.erase(keyIndexVector.begin() + randIndex - 1);		// Find some more efficient way of storing the index info ???
	}

	for (int c{ }; c < degree_of_permutation; c++) {
		letter[permutationKeyIndexes[c]] = permutationLetterIndexes[c];
	}
	verify_key();

}
 */
