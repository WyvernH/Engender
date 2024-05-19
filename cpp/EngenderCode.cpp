//============================================================================
// Name        : EngenderCode.cpp
// Author      : Matthew Hinton
// Version     :
// Copyright   : 2022
// Description : Engender program as one c++ file
//============================================================================

#include "EngenderCode.h"
using namespace std;

// temporary solution to analysis !!! alter this layout
void defineColemak() {
	colemak.letter[0] = 0;
	colemak.letter[1] = 1;
	colemak.letter[2] = 2;
	colemak.letter[3] = 3;
	colemak.letter[4] = 4;
	colemak.letter[5] = 5;

	colemak.verify_key();
}

void setVariables() {
	// Define the alphabetical layout
	for (int i{ }; i < LAYOUT_SIZE; i++) {
		alphabeticalLayout.key[i] = i;
		alphabeticalLayout.letter[i] = i;
	}
}

// Flags for general operation
int setFlags(bool c, bool w, bool d, bool p, bool r, bool o, bool e) {
	corpus = c;				// Set to {true} to use generated corpus file instead of words file. Set to {true} only after running: Engender >>> gencorpus
	weights = w;			// Set to {true} to use generated weights file instead of ???
	dynamic = d;			// Set to {true} to generate layout for use with a dynamic key on thumb
	postSynapticEvolution = p;
	randomInitialization = r;
	optimizeOutputs = o;
	conservativeEvolution = e;

	return 0;
}

// Settings and variables for parsing words file and calculating scoring function
int setCorpusInfo(string c, string w, string n) {
	corpusFile = c;		// Default words file to read data from
	weightsFile = w; 		// Default weights file to read data from
	newCorpus = n;
	alphabet = {' ','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',',','.','/','\''};
	Alphabet = {' ','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',';',':','?','\"'};

	return 0;
}

void initializeDigraphMatrix() {

	for (int i{ }; i < sizeof(digraphScores)/sizeof(digraphScores[0]); i++) {
		for (int j{ }; j < sizeof(digraphScores[i])/sizeof(digraphScores[i][0]); j++) {
			digraphScores[i][j] = 0.0;
		}
	}

	// !!! Delete this before changing LAYOUT_SIZE !!!
	digraphScores[2][0] = 3.0;
	digraphScores[2][1] = 2.0;
	digraphScores[2][2] = 1.0;
	digraphScores[2][3] = 1.0;
	digraphScores[2][4] = 2.0;
	digraphScores[2][5] = 3.0;

	/*		Equivalent to this for LAYOUT_SIZE = 30:
		digraphScores[0] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};		// T1 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
		digraphScores[1] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};		// P1 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
		digraphScores[2] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};		// P2 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
		digraphScores[3] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};		// P3 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
		digraphScores[4] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};		// R1 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
		digraphScores[5] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};		// R2 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
		digraphScores[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};		// R3 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
		digraphScores[7] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};		// M1 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
		digraphScores[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}; 		// M2 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
		digraphScores[9] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},		// M3 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
		digraphScores[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};		// I1 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
		digraphScores[11] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};		// I2 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
		digraphScores[12] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};		// I3 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
		digraphScores[13] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};		// I4 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
		digraphScores[14] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};		// I5 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
		digraphScores[15] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};		// I6 > T1, P1, P2, P3, R1, R2, R3, M1, M2, M3, I1, I2, I3, I4, I5, I6
	*/
}

// Set parameters for keyboard scoring data
int setWeights() {

	if (weights) {
		ifstream weights;
		weights.open(weightsFile, ios::in);

		if (weights.is_open()) {

			// Populate digraphScores[][] with all weights from weights file
			{

			}

		} else {

		}

		weights.close();
	}

	// Calculate and set ngramLength
	double nGramRatio{ };
	for (int i{ }; i < LAYOUT_SIZE / 2 + 1; i++) {
		for (int j{ }; j < LAYOUT_SIZE / 2 + 1; j++) {
			for (int k{ }; k < LAYOUT_SIZE / 2 + 1; k++) {
				for (int l{ }; l < LAYOUT_SIZE / 2 + 1; l++) {
					nGramRatio = (digraphScores[i][j] - digraphScores[0][i]) / (digraphScores[k][l] - digraphScores[0][k]);
					if (nGramRatio > ngramLength) {
						double score = digraphScores[i][l];
						ngramLength = ceil(score);
					}
				}
			}
		}
	}

	// Assign the finger that presses key n. 0:thumb 1:pinky, 2:ring, 3:middle, 4:index
	// keyFingers = {0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1};

	return 0;
}

void setNgramLength() {
	// Calculate and set ngramLength
	double nGramRatio{ };
	for (int i{ }; i < LAYOUT_SIZE / 2 + 1; i++) {
		for (int j{ }; j < LAYOUT_SIZE / 2 + 1; j++) {
			for (int k{ }; k < LAYOUT_SIZE / 2 + 1; k++) {
				for (int l{ }; l < LAYOUT_SIZE / 2 + 1; l++) {
					nGramRatio = (digraphScores[i][j] - digraphScores[0][i]) / (digraphScores[k][l] - digraphScores[0][k]);
					if (nGramRatio > ngramLength) {
						double score = digraphScores[i][l];
						ngramLength = ceil(score);
					}
				}
			}
		}
	}
}

void loadLayouts() {
	// Search layouts directory for layout files and create entries for each in the loadedLayouts matrix
}

bool digraphIsSameHand(int key1, int key2) {
	bool sameHand;
	if ((key1 > LAYOUT_SIZE / 2 && key2 > LAYOUT_SIZE / 2) || (key1 < LAYOUT_SIZE / 2 + 1 && key2 < LAYOUT_SIZE / 2 + 1)) {
		sameHand = true;
	} else { sameHand = false; }

	return sameHand;
}

// Define layout class functions
void layout::set_name(string n) {
	name = n;
}
void layout::set_creator(string c) {
	creator = c;
}
void layout::verify_key() {
	for (int i{ }; i < LAYOUT_SIZE; i++) {
		key[letter[i]] = i;
	}
}
void layout::verify_letter() {
	for (int i{ }; i < LAYOUT_SIZE; i++) {
		letter[key[i]] = i;
	}
}
void layout::evolve(int degree_of_permutation) {

	int permutationKeyIndexes[degree_of_permutation];
	int permutationLetterIndexes[degree_of_permutation];	// The letters on the keys in permutationKeyIndexes, but to be scrambled
	int randIndex{ };

	// Pick the keys to permute and store their indexes in permutationKeyIndexes. Store the indexes of the letters of those keys in permutationLetterIndexes.
	initializeKeyIndexVector(LAYOUT_SIZE);
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

long double layout::ngramScore(vector<int> ngram) {

	long double ngramScore{ }, temporaryScore{ }, tempSum{ };		// Does this need to be long double, long long double?
	int n{ ngramLength - 1 };

	for (int k{1}; k < ngramLength; k++) {		// k is the same k as in the G function in my portfolio post
		tempSum = 0;
		for (int i{1}; i < k; i++) {
			tempSum += digraphScores[key[ngram[n - k]]][key[ngram[n - i]]];
		}
		ngramScore = digraphScores[key[ngram[n - k]]][key[ngram[n]]] - tempSum + (k - 2) * digraphScores[0][key[ngram[n - k]]];

		if (temporaryScore > ngramScore) {
			ngramScore = temporaryScore;
		}
		temporaryScore = ngramScore;
	}

	return ngramScore;
}
long double layout::score() {

	long double scoreSum{ };		// Does this need to be long double?

	for (int i{ }; i < ngramIndexMatrix.size(); i++) {
		scoreSum += ngramScore(ngramIndexMatrix[i]);
	}

	return scoreSum;
}

// Parse corpus file and record n-gram data in vectors ngramIndexMatrix and ngramFrequencyArray
void corpusRecord() {

	// Set a few reusable variables
	long corpusLength{ };
	long skip{ };
	long skippedNGram{ };
	long ngramCount{ };
	bool repeated{true};

	// re-initialize ngramIndexMatrix and ngramFrequencyArray
	ngramIndexMatrix = {};
	ngramFrequencyArray = {};

	ifstream corpus;
	corpus.open(corpusFile, ios::in | ios::ate);

	if (corpus.is_open()) {

		// Allocate a vector for the n-gram characters
		corpusLength = corpus.tellg();
		char ngramCharacterMatrix[corpusLength - ngramLength][ngramLength];

		// Populate ngramCharacterMatrix with all n-grams from corpus and count the number of repeated n-grams in integer skip
		{
			string fullCorpus;

			fullCorpus.reserve(corpus.tellg());
			fullCorpus.assign(istreambuf_iterator<char>(corpus), istreambuf_iterator<char>());

			for (unsigned long i{ }; i < fullCorpus.length() - ngramLength; i++) {
				repeated = true;
				for (int j{ }; j < ngramLength; j++) {
					ngramCharacterMatrix[i][j] = fullCorpus.at(i + j);
				}
				for (unsigned long m{ }; m < i; m++) {
					for (int n{ }; n < ngramLength; n++) {
						if (ngramCharacterMatrix[i][n] != ngramCharacterMatrix[m][n]) {
							repeated = false;
						}
					}
					if (repeated) {
						skip++;
						break;
					}
				}
			}
		}

		// Allocate vectors for the definitions of n-grams and their frequencies
		ngramIndexMatrix.resize(corpusLength - ngramLength - skip, vector<int>(ngramLength));
		ngramFrequencyArray.resize(corpusLength - ngramLength - skip);

		// Populate
		for (long n{ }; n < corpusLength - ngramLength; n++) {
			for (long i{ }; i < n; i++) {
				repeated = true;
				for (int c{ }; c < ngramLength; c++) {
					if (ngramCharacterMatrix[n][c] != ngramCharacterMatrix[i][c]) {
						repeated = false;
					}
				}
				if (repeated) {
					skippedNGram = i;
					break;
				}
			}
			if (repeated) {
				ngramFrequencyArray[skippedNGram]++;
			} else {
				for (int c{ }; c < ngramLength; c++) {
					for (int i{ }; i < 31; i++) {
						if (ngramCharacterMatrix[n][c] == alphabet[i]) {
							ngramIndexMatrix[ngramCount][c] = i;
						}
					}
				}
				ngramCount++;
			}
		}

	} else {

	}

	corpus.close();

}

void corpusRecord2() {

		// Set a few reusable variables
		long corpusLength{ };
		long skip{ };
		long skippedNGram{ };
		long ngramCount{ };
		bool repeated{true};

		// re-initialize ngramIndexMatrix and ngramFrequencyArray
		ngramIndexMatrix = {};
		ngramFrequencyArray = {};



		if (true) {

			// Allocate a vector for the n-gram characters
			corpusLength = 9;
			char ngramCharacterMatrix[corpusLength - ngramLength][ngramLength];

			// Populate ngramCharacterMatrix with all n-grams from corpus and count the number of repeated n-grams in integer skip
			{
				string fullCorpus;

				fullCorpus = "abcabcabc";

				for (unsigned long i{ }; i < fullCorpus.length() - ngramLength; i++) {
					repeated = true;
					for (int j{ }; j < ngramLength; j++) {
						ngramCharacterMatrix[i][j] = fullCorpus.at(i + j);
					}
					for (unsigned long m{ }; m < i; m++) {
						for (int n{ }; n < ngramLength; n++) {
							if (ngramCharacterMatrix[i][n] != ngramCharacterMatrix[m][n]) {
								repeated = false;
							}
						}
						if (repeated) {
							skip++;
							break;
						}
					}
				}
			}

			// Allocate vectors for the definitions of n-grams and their frequencies
			ngramIndexMatrix.resize(corpusLength - ngramLength - skip, vector<int>(ngramLength));
			ngramFrequencyArray.resize(corpusLength - ngramLength - skip);

			// Populate
			for (long n{ }; n < corpusLength - ngramLength; n++) {
				for (long i{ }; i < n; i++) {
					repeated = true;
					for (int c{ }; c < ngramLength; c++) {
						if (ngramCharacterMatrix[n][c] != ngramCharacterMatrix[i][c]) {
							repeated = false;
						}
					}
					if (repeated) {
						skippedNGram = i;
						break;
					}
				}
				if (repeated) {
					ngramFrequencyArray[skippedNGram]++;
				} else {
					for (int c{ }; c < ngramLength; c++) {
						for (int i{ }; i < 31; i++) {
							if (ngramCharacterMatrix[n][c] == alphabet[i]) {
								ngramIndexMatrix[ngramCount][c] = i;
							}
						}
					}
					ngramCount++;
				}
			}

		} else {

		}

}
// Edit corpus file so it only contains characters in the lowercase alphabet array
int corpusFormat() {

	char tempChar{ };

	fstream corpus;
	corpus.open(corpusFile, ios::in | ios::out | ios::ate);

	if (corpus.is_open()) {
		for (long i{ }; i < corpus.tellg(); i++) {
			corpus.seekg(i);
			corpus >> tempChar;
			for (int c{ }; c < 31; c++) {
				if (tempChar == alphabet[c]) { break; }
				if (tempChar == Alphabet[c]) {
					corpus.seekp(corpus.tellg());
					corpus << alphabet[c];
					break;
				} else {
					for (unsigned long e{ }; e < extraCharList.size(); e++) {
						if (tempChar == extraCharList[e].character) {
							/*
							 * 								 *
							 * DO SOME STUFF HERE DEPENDING ON THE CHARACTER
							 * CREATE A FUNCTION THAT TELLS WHAT TO DO HERE IN "class extended_char"
							 *
						 	 */
						} else {
							cout << "Error: " + corpusFile + "contains undefined characters. Please remove these characters or add them to the class \'extended_char\' in EngenderCode.h.\n";
							corpus.close();
							return 1;
						}
					}
				}
			}
		}
	}

	corpus.close();

	return 0;

}

int settings(bool corpusBool, bool weightsBool, bool dynamicBool, bool postSynapticEvolutionBool, bool randomInitializationBool, bool optimizeOutputsBool, bool conservativeEvolutionBool, string corpusName, string weightsName, string newCorpusName) {
	setVariables();
	setFlags(corpusBool, weightsBool, dynamicBool, postSynapticEvolutionBool, randomInitializationBool, optimizeOutputsBool, conservativeEvolutionBool);
	setCorpusInfo(corpusName, weightsName, newCorpusName);	// default corpus and weights files
	//setWeights();
	if (corpusName != newCorpusName) {
		corpusRecord();
	}

	return 0;
}



/*
 *
 * Setting up the neural matrix
 *
 */


// Function for permuting a layout in the neural matrix
layout permute(layout base, layout permutation) {
	layout newlayout;

	for (int n{ }; n < LAYOUT_SIZE; n++) {
		newlayout.letter[n] = base.letter[permutation.letter[n]];
	}
	newlayout.verify_key();

	return newlayout;
}

// set keyIndexVector to be the natural set from 0 to LAYOUT_SIZE
void initializeKeyIndexVector(int size) {
	keyIndexVector.resize(size);
	for (int l{ }; l < size; l++) {
		keyIndexVector[l] = l;
	};
}

// return a random index of the set keyIndexVector
int seedRandomIndex() {
	srand ( time(NULL) );
	return rand() % keyIndexVector.size();
}

// boolean random: if true, initialize random layouts. boolean outputRandom: if true, output synapses are random. If false, they are alphabetical; they don't do any re-permutation. If true, set evolveOutput to true in evolveMatrix. If false, set evolveOutput to false in evolveeMatrix.
void initializeMatrix(bool random, bool outputRandom) {

	int randIndex{ };	// Declare a reusable variable

	if (random) {

		initializeKeyIndexVector(LAYOUT_SIZE);

		for (int i{ }; i < LAYOUT_SIZE; i++) {

			for (int j{ }; j < LAYOUT_SIZE; j++) {

				for (int k{ }; k < LAYOUT_SIZE; k++) {

					for (int c{ }; c < LAYOUT_SIZE; c++) {
						randIndex = seedRandomIndex();
						hiddenSynapse[i][j][k].key[randIndex] = c;
						keyIndexVector.erase(keyIndexVector.begin() + randIndex - 1);		// Find some more efficient way of storing the index info ???
					}
					hiddenSynapse[i][j][k].verify_key();
				}

				initializeKeyIndexVector(LAYOUT_SIZE);

				for (int c{ }; c < LAYOUT_SIZE; c++) {
					randIndex = seedRandomIndex();
					hiddenNode[i][j].key[randIndex] = c;
					keyIndexVector.erase(keyIndexVector.begin() + randIndex - 1);		// Find some more efficient way of storing the index info ???
				}
				hiddenNode[i][j].verify_key();
			}

			initializeKeyIndexVector(LAYOUT_SIZE);

			for (int c{ }; c < LAYOUT_SIZE; c++) {
				randIndex = seedRandomIndex();
				inputNode[i].key[randIndex] = c;
				keyIndexVector.erase(keyIndexVector.begin() + randIndex - 1);		// Find some more efficient way of storing the index info ???
			}
			inputNode[i].verify_key();

			if (outputRandom) {
				initializeKeyIndexVector(LAYOUT_SIZE);			// Important Note!!! Are output synapses supposed to be randomized? Not according to my portfolio post.

				for (int c{ }; c < LAYOUT_SIZE; c++) {
					randIndex = seedRandomIndex();
					outputSynapse[i].key[randIndex] = c;
					keyIndexVector.erase(keyIndexVector.begin() + randIndex - 1);		// Find some more efficient way of storing the index info ???
				}
				outputSynapse[i].verify_key();
			}

		}


	} else {

		for (int i{ }; i < LAYOUT_SIZE; i++) {
			for (int j{ }; j < LAYOUT_SIZE; j++) {
				for (int k{ }; k < LAYOUT_SIZE; k++) {
					inputNode[i] = alphabeticalLayout;
					hiddenNode[i][j] = alphabeticalLayout;
					hiddenSynapse[i][j][k] = alphabeticalLayout;
					outputSynapse[i] = alphabeticalLayout;
				}
			}
		}

	}

}

// This function groups synapses by their pre-synaptic node. Should it be done by post-synaptic node ???
void evaluateMatrix(bool evolveOutput) {						// Calculate the value of the output node by running through the entire matrix from new inputs

	long double layoutScore[LAYOUT_SIZE][LAYOUT_SIZE];		// Does this need to be long long double?
	int tempOrder[LAYOUT_SIZE];
	layout tempLayout;

	// Fill in inputNodeOrder[]. The best node gets 1. The worst gets LAYOUT_SIZE. Is there a more efficient way to do this ???
	for (int j{ }; j < LAYOUT_SIZE; j++) {
		layoutScore[j][0] = inputNode[j].score();
		tempOrder[j] = j;
	}
	for (int j{1}; j < LAYOUT_SIZE; j++) {
		for (int c{j}; c > 0; --c) {
			if (layoutScore[j][0] < layoutScore[j - c][0]) {
				// Swap elements k and k-c in tempOrder and in layoutScore
				swap(layoutScore[j][0], layoutScore[j - c][0]);
				swap(tempOrder[j], tempOrder[j - c]);
			}
		}
	}
	for (int j{ }; j < LAYOUT_SIZE; j++) {
		inputNodeOrder[tempOrder[j]] = j + 1;
		tempOrder[j] = j;
	}

	// Calculate the nodes of the first hidden layer
	for (int k{ }; k < LAYOUT_SIZE; k++) {
		hiddenNode[0][k] = permute(inputNode[0], hiddenSynapse[0][0][k]);
		layoutScore[0][k] = hiddenNode[0][k].score();
		for (int j{1}; j < LAYOUT_SIZE; j++) {
			tempLayout = permute(inputNode[j], hiddenSynapse[0][j][k]);
			layoutScore[j][k] = tempLayout.score();
			if (tempLayout.score() < layoutScore[j - 1][k]) {
				hiddenNode[0][k] = tempLayout;
			}
		}
	}
	// Fill in hiddenSynapseOrder[0][][]. The best synapse gets 1. The worst gets LAYOUT_SIZE. Is there a more efficient way to do this ???
	for (int j{ }; j < LAYOUT_SIZE; j++) {
		for (int k{1}; k < LAYOUT_SIZE; k++) {
			for (int c{k}; c > 0; --c) {
				if (layoutScore[j][k] < layoutScore[j][k - c]) {
					// Swap elements k and k-c in tempOrder and in layoutScore
					swap(layoutScore[j][k], layoutScore[j][k - c]);
					swap(tempOrder[k], tempOrder[k - c]);
				}
			}
		}
		for (int k{ }; k < LAYOUT_SIZE; k++) {
			hiddenSynapseOrder[0][j][tempOrder[k]] = k + 1;
			tempOrder[k] = k;
		}
	}

	// Calculate all the other hidden nodes
	for (int i{1}; i < LAYOUT_SIZE; i++) {
		for (int k{ }; k < LAYOUT_SIZE; k++) {
			hiddenNode[i][k] = permute(hiddenNode[i - 1][0], hiddenSynapse[i][0][k]);
			layoutScore[0][k] = hiddenNode[0][k].score();
			for (int j{1}; j < LAYOUT_SIZE; j++) {
				tempLayout = permute(hiddenNode[i - 1][j], hiddenSynapse[i][j][k]);
				layoutScore[j][k] = tempLayout.score();
				if (tempLayout.score() < layoutScore[j - 1][k]) {
					hiddenNode[i][k] = tempLayout;
				}
			}
		}
		// Fill in hiddenSynapseOrder[i][][]. The best synapse gets 1. The worst gets LAYOUT_SIZE. Is there a more efficient way to do this ???
		for (int j{ }; j < LAYOUT_SIZE; j++) {
			for (int k{1}; k < LAYOUT_SIZE; k++) {
				for (int c{k}; c > 0; --c) {
					if (layoutScore[j][k] < layoutScore[j][k - c]) {
						// Swap elements k and k-c in tempOrder and in layoutScore
						swap(layoutScore[j][k], layoutScore[j][k - c]);
						swap(tempOrder[k], tempOrder[k - c]);
					}
				}
			}
			for (int k{ }; k < LAYOUT_SIZE; k++) {
				hiddenSynapseOrder[i][j][tempOrder[k]] = k + 1;
				tempOrder[k] = k;
			}
		}
	}

	// Calculate the output node
	outputNode = permute(hiddenNode[LAYOUT_SIZE - 1][0], outputSynapse[0]);
	layoutScore[0][0] = outputNode.score();
	for (int j{1}; j < LAYOUT_SIZE; j++) {
		tempLayout = permute(hiddenNode[LAYOUT_SIZE - 1][j], outputSynapse[j]);
		layoutScore[j][0] = tempLayout.score();
		if (tempLayout.score() < layoutScore[j - 1][0]) {
			outputNode = tempLayout;
		}
	}
	// Fill in outputSynapseOrder[]. The best synapse gets 1. The worst gets LAYOUT_SIZE. Is there a more efficient way to do this ???
	for (int j{1}; j < LAYOUT_SIZE; j++) {
		for (int c{j}; c > 0; --c) {
			if (layoutScore[j][0] < layoutScore[j - c][0]) {
				// Swap elements k and k-c in tempOrder and in layoutScore
				swap(layoutScore[j][0], layoutScore[j][0]);
				swap(tempOrder[j], tempOrder[j - c]);
			}
		}
	}
	for (int j{ }; j < LAYOUT_SIZE; j++) {
		outputSynapseOrder[tempOrder[j]] = j + 1;
		tempOrder[j] = j;
	}

	// Set some meta information
	outputNode.set_creator("Engender Neural Matrix");

}
// This function groups synapses by their post-synaptic node !!! do this in the function !!!
void evaluateMatrix2() {

	long double layoutScore[LAYOUT_SIZE];		// Does this need to be long long double?
	int tempOrder[LAYOUT_SIZE];
	layout tempLayout;

	// Fill in inputNodeOrder[]. The best node gets 1. The worst gets LAYOUT_SIZE. Is there a more efficient way to do this ???
	for (int k{ }; k < LAYOUT_SIZE; k++) {
		layoutScore[k] = inputNode[k].score();
		tempOrder[k] = k;
	}
	for (int k{1}; k < LAYOUT_SIZE; k++) {
		for (int c{k}; c > 0; --c) {
			if (layoutScore[k] < layoutScore[k - c]) {
				// Swap elements k and k-c in tempOrder and in layoutScore
				swap(layoutScore[k], layoutScore[k - c]);
				swap(tempOrder[k], tempOrder[k - c]);
			}
		}
	}
	for (int k{ }; k < LAYOUT_SIZE; k++) {
		inputNodeOrder[tempOrder[k]] = k + 1;
		tempOrder[k] = k;
	}

	// Calculate the nodes of the first hidden layer
	for (int k{ }; k < LAYOUT_SIZE; k++) {
		hiddenNode[0][k] = permute(inputNode[0], hiddenSynapse[0][0][k]);
		layoutScore[0] = hiddenNode[0][k].score();
		for (int j{1}; j < LAYOUT_SIZE; j++) {
			tempLayout = permute(inputNode[j], hiddenSynapse[0][j][k]);
			layoutScore[j] = tempLayout.score();
			if (tempLayout.score() < layoutScore[j - 1]) {
				hiddenNode[0][k] = tempLayout;
			}
			// Fill in the temporary order for hiddenSynapseOrder[0][][].
			for (int c{j}; c > 0; --c) {
				if (layoutScore[j] < layoutScore[j - c]) {
					// Swap elements j and j-c in tempOrder and in layoutScore
					swap(layoutScore[j], layoutScore[j - c]);
					swap(tempOrder[j], tempOrder[j - c]);
				}
			}
		}
		// Fill in hiddenSynapseOrder[0][][]. The best synapse gets 1. The worst gets LAYOUT_SIZE. Is there a more efficient way to do this ???
		for (int j{ }; j < LAYOUT_SIZE; j++) {
			hiddenSynapseOrder[0][tempOrder[j]][k] = j + 1;
			tempOrder[j] = j;
		}
	}


	// Calculate all the other hidden nodes
	for (int i{1}; i < LAYOUT_SIZE; i++) {
		for (int k{ }; k < LAYOUT_SIZE; k++) {
			hiddenNode[i][k] = permute(hiddenNode[i - 1][0], hiddenSynapse[i][0][k]);
			layoutScore[0] = hiddenNode[i][k].score();
			for (int j{1}; j < LAYOUT_SIZE; j++) {
				tempLayout = permute(inputNode[j], hiddenSynapse[i][j][k]);
				layoutScore[j] = tempLayout.score();
				if (tempLayout.score() < layoutScore[j - 1]) {
					hiddenNode[i][k] = tempLayout;
				}
				// Fill in the temporary order for hiddenSynapseOrder[i][][].
				for (int c{j}; c > 0; --c) {
					if (layoutScore[j] < layoutScore[j - c]) {
						// Swap elements j and j-c in tempOrder and in layoutScore
						swap(layoutScore[j], layoutScore[j - c]);
						swap(tempOrder[j], tempOrder[j - c]);
					}
				}
			}
			// Fill in hiddenSynapseOrder[i][][]. The best synapse gets 1. The worst gets LAYOUT_SIZE. Is there a more efficient way to do this ???
			for (int j{ }; j < LAYOUT_SIZE; j++) {
				hiddenSynapseOrder[i][tempOrder[j]][k] = j + 1;
				tempOrder[j] = j;
			}
		}
	}

	// Calculate the output node
	outputNode = permute(hiddenNode[LAYOUT_SIZE - 1][0], outputSynapse[0]);
	layoutScore[0] = outputNode.score();
	for (int j{1}; j < LAYOUT_SIZE; j++) {
		tempLayout = permute(hiddenNode[LAYOUT_SIZE - 1][j], outputSynapse[j]);
		layoutScore[j] = tempLayout.score();
		if (tempLayout.score() < layoutScore[j - 1]) {
			outputNode = tempLayout;
		}
	}
	// Fill in outputSynapseOrder[]. The best synapse gets 1. The worst gets LAYOUT_SIZE. Is there a more efficient way to do this ???
	for (int j{1}; j < LAYOUT_SIZE; j++) {
		for (int c{j}; c > 0; --c) {
			if (layoutScore[j] < layoutScore[j - c]) {
				// Swap elements k and k-c in tempOrder and in layoutScore
				swap(layoutScore[j], layoutScore[j]);
				swap(tempOrder[j], tempOrder[j - c]);
			}
		}
	}
	for (int j{ }; j < LAYOUT_SIZE; j++) {
		outputSynapseOrder[tempOrder[j]] = j + 1;
		tempOrder[j] = j;
	}

	// Set some meta information
	outputNode.set_creator("Engender Neural Matrix");

}

void evolveSynapses(bool conservativeEvolution, bool evolveOutput) {			// Re-permute each synapse and input node depending on the scores of the resulting layouts. If conservativeEvolution, only re-permute the worst synapse by one swap.

	temporaryOutputNode = outputNode;
	// Do not use evolve() for conservative evolution !!! Instead, define a function to swap two keys in the worst layout. !!! If we only ever use conservative evolution, get rid of synapse and node order arrays! Only keep track of the worst ones!
	if (conservativeEvolution) {

		for (int i{ }; i < LAYOUT_SIZE; i++) {
			for (int j{ }; j < LAYOUT_SIZE; j++) {
				for (int k{ }; k < LAYOUT_SIZE; k++) {
					if (hiddenSynapseOrder[i][j][k] == LAYOUT_SIZE - 1) {
						hiddenSynapse[i][j][k].evolve(2);
					}
				}
			}
			if (inputNodeOrder[i] == LAYOUT_SIZE) {
				inputNode[i].evolve(2);
			}
			if (outputSynapseOrder[i] == LAYOUT_SIZE && evolveOutput) {
				outputSynapse[i].evolve(2);
			}
		}

	} else {

		for (int i{ }; i < LAYOUT_SIZE; i++) {
			for (int j{ }; j < LAYOUT_SIZE; j++) {
				for (int k{ }; k < LAYOUT_SIZE; k++) {
					hiddenSynapse[i][j][k].evolve(hiddenSynapseOrder[i][j][k]);
				}
			}
			inputNode[i].evolve(inputNodeOrder[i]);
			if (evolveOutput) {
				outputSynapse[i].evolve(outputSynapseOrder[i]);
			}
		}
	}

}

// Combine evaluateMatrix and evolveSynapses.
void evolveMatrix(bool conservativeEvolution, bool evolveOutput) {

}

// Combine evaluateMatrix2 and evolveSynapses.
void evolveMatrix2(bool conservativeEvolution, bool evolveOutput) {

}

bool compareLayouts(layout lay1, layout lay2) {
	bool equal{true};
	for (int i{ }; i < LAYOUT_SIZE; i++) {
		if (lay1.letter[i] != lay2.letter[i]) {
			equal = false;
		}
	}
	return equal;
}

// Generate a layout
void generate() {

	printf("0");							// !!!
	initializeMatrix(randomInitialization, optimizeOutputs);
	int c{ };
	while (compareLayouts(temporaryOutputNode, outputNode) && c < 1000) {
		printf("1");						// !!!
		evaluateMatrix2();
		evolveSynapses(conservativeEvolution, optimizeOutputs);
		c++;
	}
}

// Print a layout. !!! Update this to print more information.
void printLayout(layout lay) {
	for (int i{ }; i < LAYOUT_SIZE; i++) {
		cout << alphabet[lay.letter[i] + 1] + " ";		// lay.letter[i] + 1 because alphabet[] begins with a space
	}
	cout << "\n\n";
}



void getCommands() {

	cout <<	"Welcome to Engender, the keyboard layout analyzer and engenderer.\n"
			"Type \"help\" for a list of commands.\n\n";

	string cmd;

	do {
			cout << ">>> ";
			cin >> cmd;

			// remove spaces at beginning of cmd
			bool cmdspace{true};
			while (cmdspace) {
				if (cmd.substr(0, 1) == " ") {

				}
				if (cmd.substr(0, 1) != " ") {
						cmdspace = false;
				}
			}

			cmd.pop_back(); // Remove the newline.

			if (cmd.substr(0, 4) == "help" || cmd.substr(0, 1) == "h") {
				cout << "[h]elp:\t\t\t\tPrint this help and exit.\n\n"
						"[a]nalyze <layout>:\tAnalyze given layout and print statistics\n"	// ... and print score?
						"[g]enerate:\t\t\tGenerate the optimal layout using the weights in" + weightsFile + " and the current corpus file.\n"
						"[r]ank:\t\t\t\tRank all layouts and list their scores\n\n"
						// "[f]lags:\t\t\t\tList all flags and their current status.\n"
						// "set <flag>:\t\t\tSet the specified flag.\n"
						// "clear <flag>\t\t\tClear the specified flag.\n\n"
						"[c]orpus <file>\t\t\tFormat and use a new corpus\n"
						"[w]eights <file>\t\t\tUse the new specified weights file\n"
						"[d]ynamic <bool>\t\t\tAnalize and optimize for a dynamic key layout?\n\n"
						"[q]uit:\t\t\t\tQuit the Engender program.\n\n";

			} else if (cmd.substr(0, 8) == "analyze" || cmd.substr(0, 2) == "a ") {
				// calculateScoringFunction(layoutIdentity(cmd.substr(8, string::npos)));

				// This code will analyze the layout hardcodedLayout
				cout << colemak.score();

			} else if (cmd.substr(0, 8) == "generate" || cmd.front() == 'g') {
				printf("2");				// !!!
				generate();
				printLayout(outputNode);

			} else if (cmd.substr(0, 4) == "rank" || cmd.front() == 'r') {

			}
			/*
			else if (cmd.substr(0, 4) == "flags" || cmd.substr(0, 1) == "f") {
				if (corpus) {
					cout << "Corpus:\t(set)\t\tIf set, use generated corpus file instead of words file\n";
				} else {
					cout << "Corpus:\t(unset)\tIf set, use generated corpus file instead of words file\n";
				}
				if (weights) {
					cout << "Weights:\t(set)\t\t???\n";
				} else {
					cout << "Weights:\t(unset)\t???\n";
				}
				if (dynamic) {
					cout << "Dynamic:\t(set)\t\tIf set, optimize for a dynamic key layout\n\n";
				} else {
					cout << "Dynamic:\t(unset)\tIf set, optimize for a dynamic key layout\n\n";
				}
			} else if (cmd.substr(0, 4) == "set ") {
				// remove "set " from cmd
				// remove whitespace from beginnig of cmd
				if (cmd.substr(0, 6) == "corpus" || cmd.substr(0, 6) == "Corpus") {
					corpus = true;
				} else if (cmd.substr(0, 7) == "weights" || cmd.substr(0, 6) == "Corpus") {
					weights = true;
				} else if (cmd.substr(0, 7) == "dynamic" || cmd.substr(0, 6) == "Dynamic") {
					dynamic = true;
				} else {
					cout << "Flag not found, type \'flags\' for a list of flags\n";
				}

			} else if (cmd.substr(0, 6) == "clear ") {
				// remove "clear " from cmd
				// remove whitespace from beginnig of cmd
				if (cmd.substr(0, 6) == "corpus" || cmd.substr(0, 6) == "Corpus") {
					corpus = false;
				} else if (cmd.substr(0, 7) == "weights" || cmd.substr(0, 6) == "Corpus") {
					weights = false;
				} else if (cmd.substr(0, 7) == "dynamic" || cmd.substr(0, 6) == "Dynamic") {
					dynamic = false;
				} else {
					cout << "Flag not found, type \'flags\' for a list of flags\n";
				}

			}
			*/
			else if (cmd.substr(0, 7) == "corpus " || cmd.substr(0, 2) == "c ") {
				// remove "corpus " from cmd
				// remove whitespace from beginnig of cmd
				if (cmd == "") {
					cout << "Enter a valid corpus file\n";
				} else {
					// remove whitespace from end of cmd
					corpusFile = cmd;
				}

			} else if (cmd.substr(0, 8) == "weights " || cmd.substr(0, 2) == "w ") {
				// remove "weights " from cmd
				// remove whitespace from beginnig of cmd
				if (cmd == "") {
					cout << "Enter a valid weights file\n";
				} else {
					// remove whitespace from end of cmd
					weightsFile = cmd;
				}

			} else if (cmd.substr(0, 8) == "dynamic " || cmd.substr(0, 2) == "d ") {
				// remove "dynamic " from cmd
				// remove whitespace from beginnig of cmd
				if (cmd.substr(0, 4) == "true" || cmd.substr(0, 4) == "True" || cmd.substr(0, 3) == "yes" || cmd.substr(0, 3) == "Yes") {
					dynamic = true;
				} else if (cmd.substr(0, 5) == "false" || cmd.substr(0, 5) == "False" || cmd.substr(0, 2) == "no" || cmd.substr(0, 2) == "No") {
					dynamic = false;
				} else {
					cout << "Please give \'dynamic\' a boolean value\n";
				}

			} else if (cmd.substr(0, 4) == "quit" || cmd.substr(0, 1) == "q") {
				cout << "\nExiting ...\n\n";
				break;

			} else {
				cout << "Unknown command. Type \"help\" for a list of commands.\n\n";
			}


		} while (true);

}








int main() {

	defineColemak();
	settings(false, false, false, true, true, false, false, "data\\shai_corpus.txt", "data\\wyvernh_weights.txt", "");
	setNgramLength();
	// corpusFormat();
	// corpusRecord();
	corpusRecord2();
	cout << colemak.score(); cout << "\n\n";
	getCommands();

	return 0;
}
