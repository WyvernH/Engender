/*
 * corpus.h
 *
 *  Created on: Nov 3, 2021
 *      Author: Matthew Hinton
 */

#ifndef CORPUS_H_
#define CORPUS_H_


#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>
#include <cstring>
#include <filesystem>

//#define LayoutSize 31 // not more than 127!
//#define LayoutSize 6

using namespace std;
namespace fs = filesystem;

inline unsigned ngramLength;

class ngram_t {
private:
public:
    static unsigned long long ngramCount;
    long long unsigned frequency;
    char* ngram;
    ngram_t () { ngramCount++; }
    ngram_t (long long unsigned freq, char* ng) {
        frequency = freq;
        ngram = new char [ngramLength];
        memcpy(ngram, ng, ngramLength);
        ngramCount++;
    }
    ngram_t (ngram_t &ng) {
        frequency = ng.frequency;
        ngram = new char [ngramLength];
        memcpy(ngram, ng.ngram, ngramLength);
        ngramCount++;
    }
    ~ngram_t () { ngramCount--; }
    ngram_t& operator= (const ngram_t& ng) {
        frequency = ng.frequency;
        ngram = new char [ngramLength];
        memcpy(ngram, ng.ngram, ngramLength);
        return *this;
    }
}; inline long long unsigned ngram_t::ngramCount;

inline constexpr char LayoutSize = 31; // not more than 127!

inline ngram_t* ngramData;


// Settings and variables for parsing words file and calculating scoring function
inline constexpr string defaultCorpusFile = "corpus.txt";
inline constexpr string defaultCorpusDataFile = "corpus.csv";
inline constexpr string defaultWeightsFile = "weights.csv";

inline constexpr char alphabet[] = {' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',',','.','/','\''};
inline constexpr char extCharsList[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',';',':','?','\"','\n'};
inline constexpr string extCharMap[] = {" a"," b"," c"," d"," e"," f"," g"," h"," i"," j"," k"," l"," m"," n"," o"," p"," q"," r"," s"," t"," u"," v"," w"," x"," y"," z"," ,"," ."," /"," \'"," "}; // strings MUST be composed of characters from alphabet[]

inline char alpha_index[128];


#endif /* CORPUS_H_ */
