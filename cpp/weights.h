/*
 * weights.h
 *
 *  Created on: Oct 11, 2023
 *      Author: Matthew Hinton
 */

#ifndef WEIGHTS_H_
#define WEIGHTS_H_

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <math.h>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

inline constexpr char LayoutSize = 31; // not more than 127!

inline double digraphScores[LayoutSize][LayoutSize];

inline unsigned ngramLength;
//inline long long unsigned num_of_ngrams; // <-- maybe not needed


#endif /* WEIGHTS_H_ */
