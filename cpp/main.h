/*
 * main.h
 *
 *  Created on: Oct 9, 2023
 *      Author: Matthew Hinton
 */

#ifndef MAIN_H_
#define MAIN_H_

// temporary for testing purposes:
#include "analyse.h"
#include "layouts.h"
#include "generate.h"

int setWeights(string weightsFile);
void typingTest(string weightsFile);

char* index_to_ngram(unsigned long long);
long long get_ngram_index(char* ngram);
void register_ngram(char* ngram);
void corpusRead(string);
void corpusRecord(string);
void corpusLoad(string);

void print_ngram_of_index(unsigned long long);
void print_all_possible_ngrams(); //note: a bit dangerous if you can't ctrl-c your terminal
void print_ngramMatrix();
void print_ngram(char* ngram);

const MetaLayout** loadMetaLayouts(fs::path);
void print_layout(const MetaLayout*);

layout_t rand_layout();
void initialiseMatrix();
void initialiseDupletMatrix();
void evolveDupletMatrix();

//void getCommands();

#endif /* MAIN_H_ */
