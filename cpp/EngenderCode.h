/*
 * EngenderCode.h
 *
 *  Created on: May 4, 2022
 *      Author: Matthew Hinton
 */

#ifndef ENGENDERCODE_H_
#define ENGENDERCODE_H_

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <array>

#define LAYOUT_SIZE 6		// Must must must be even or code will break

using namespace std;

class layout {
	public:
		//long long long int id;
		string name;											// Name of the layout
		string creator{"Engender keyboard layout generator"};	// Name of the creator of the layout
		int key[LAYOUT_SIZE];									// key[n] returns the key on which the letter with index n resides
		int letter[LAYOUT_SIZE];								// letter[n] returns the index of the letter assigned to key n

		void set_name(string n);
		void set_creator(string c);
		void verify_key();
		void verify_letter();
		void evolve(int degree_of_permutation);				// Re-permute the layout by a certain degree of permutation

		long double ngramScore(vector<int> ngram);		// long long?
		long double score();							// long long?
};

class extended_chars {
	public:
		char character;
		short actionIndex;
};

struct ngraphScore_t {

	long double score;
};


using index_matrix = vector<vector<int>>;
using finger_matrix = vector<vector<short>>;
using layout_matrix = vector<vector<layout>>;


// Flags for general operation
bool corpus;
bool weights;
bool dynamic;
bool postSynapticEvolution;
bool randomInitialization;
bool optimizeOutputs;
bool conservativeEvolution;

// Settings and variables for parsing words file and calculating scoring function
string corpusFile;
string weightsFile;
string newCorpus;
vector<char> alphabet;
vector<char> Alphabet;
vector<extended_chars> extraCharList;
vector<layout> loadedLayouts;

double digraphScores[LAYOUT_SIZE / 2 + 1][LAYOUT_SIZE / 2 + 1];			// +2 for adding the space key and dynamic key??? If dynamic flag is set to false, the extra row and column will be disregarded
bool digraphIsSFB[LAYOUT_SIZE / 2 + 1][LAYOUT_SIZE / 2 + 1];
vector<short> keyFingers;

vector<ngraphScore_t> ngraphScores;				// Vector of scoring functions of n-graphs
int ngramLength;								// Length of all n-grams to record
long long layoutScore;							// Declare variable for overall layout score

index_matrix ngramIndexMatrix;					// Index numbers of letters in a vector of all n-grams
vector<int> ngraphIndexVector;					// Index numbers of keys in an n-gram
vector<long long> ngramFrequencyArray;			// Vector for storing the frequency of each n-gram


/*
int variableNum{ };
VarInfo *variables = new VarInfo[variableNum];
*/


// Set up neural matrix. Integer arrays are to record the order of layouts in input nodes or at the end of synapses in terms of score. The number associated with each input node or synapse determines its next degree of permutation. Is it too random to completely scramble the worst ones? Should only the single worst be scrambled by a single swap? It would take longer to optimize but might be necessary for results at the end.
layout inputNode[LAYOUT_SIZE];
int inputNodeOrder[LAYOUT_SIZE];
layout hiddenNode[LAYOUT_SIZE][LAYOUT_SIZE];
layout outputNode;
layout hiddenSynapse[LAYOUT_SIZE][LAYOUT_SIZE][LAYOUT_SIZE];		// hiddenSynapse[i][j][k] represents the synapse between node[i-1][j] and node[i][k] if i = 0, node[i-1][j] is an input node.
int hiddenSynapseOrder[LAYOUT_SIZE][LAYOUT_SIZE][LAYOUT_SIZE];
layout outputSynapse[LAYOUT_SIZE];
int outputSynapseOrder[LAYOUT_SIZE];
layout temporaryOutputNode;

// temporary solution to analysis !!!
layout colemak;

// Variables and functions for initializing and evolving matrix
layout alphabeticalLayout;
vector<int> keyIndexVector;
void initializeKeyIndexVector(int size);
int seedRandomIndex();


#endif /* ENGENDERCODE_H_ */
