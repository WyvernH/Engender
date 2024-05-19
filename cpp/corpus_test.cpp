#include "corpus.h"

void gen_alpha_index();
void print_ngramMatrix();
unsigned long long get_ngram_index(char*);
char* index_to_ngram(unsigned long long);






char* index_to_ngram(long long unsigned index) {

    char* ngram = new char [ngramLength];
    long long unsigned indexCounter = index;

    for (unsigned i{ }; i < ngramLength; i++) {
        ngram[i] = indexCounter / pow(LayoutSize, ngramLength - 1 - i);
        indexCounter %= (int)pow(LayoutSize, ngramLength - 1 - i);
    }

    return ngram;
}

unsigned long long get_ngram_index(char* ngram) {
    unsigned long long index{ };
    for (unsigned i{ }; i < ngramLength; i++) {
        index += ngram[i] * pow(LayoutSize, ngramLength - 1 - i);
    }
    return index;
}






void update_ngram(char* ngram, char index) {
    unsigned i{ };
    while (i < ngramLength - 1) {
        ngram[i] = ngram[i + 1];
        i++;
    }
    ngram[i] = index;
}

void register_ngram(char* ngram, ngram_t* corpusDat[], unsigned long long num_of_ngrams_stored) {
    bool sameAs = false;
    unsigned long long j{ };

    if (ngramData != nullptr) {
        while (j < num_of_ngrams_stored) {
            sameAs = true;
            for (unsigned k{ }; k < ngramLength; k++) {
                if (ngramData[j].ngram[k] != ngram[k]) sameAs = false;
            }
            if (sameAs) break;
            j++;
        }
    }
    if (sameAs) ngramData[j].frequency++;
    else {
        j = 0;
        if (corpusDat[0] != nullptr) {
            while (j < ngram_t::ngramCount - num_of_ngrams_stored) {
                sameAs = true;
                for (unsigned k{ }; k < ngramLength; k++) {
                    if ((*corpusDat[j]).ngram[k] != ngram[k]) sameAs = false;
                }
                if (sameAs) break;
                j++;
            }
        }
        if (sameAs) (*corpusDat[j]).frequency++;
        else {
            corpusDat[ngram_t::ngramCount - num_of_ngrams_stored - 1] = new ngram_t(1, ngram);
        }
    }
}


void corpusRead(string corpusFile) {

    unsigned long long unrecognised_char_count{ };

    unsigned long long num_of_ngrams_stored = ngram_t::ngramCount;

    unsigned max_extCharMap_size{ };
    for (unsigned i{ }; i < sizeof(extCharsList) / sizeof(char); i++) {
        if (extCharMap[i].length() > max_extCharMap_size) max_extCharMap_size = extCharMap[i].length();
    }

    char* ngram = new char [ngramLength]();

    streampos size;
    char* memblock;

    ifstream corpus ("corpus/" + corpusFile, ios::in | ios::ate);
    if (corpus.is_open()) {
        size = corpus.tellg();
        long memsize = size;
        memblock = new char [memsize];
        corpus.seekg (0, ios::beg);
        corpus.read (memblock, size);
        corpus.close();

        //ngram_t** corpusDat = new ngram_t* [memsize * max_extCharMap_size](nullptr);
        ngram_t* corpusDat[memsize * max_extCharMap_size](nullptr);

        for (long memblockIndex{ }; memblockIndex < memsize; memblockIndex++) {

            // get char to put in ngram
            char index = alpha_index[(int)memblock[memblockIndex]];

            // test if c is a known character
            if (index == 0 && memblock[memblockIndex] != alphabet[0] && false) {
                delete[] ngram;
                ngram = new char [ngramLength]();
                printf("corpusRead: warning: Unrecognised character '%c' in corpus file. Re-initialised ngram and moved on.\n", memblock[memblockIndex]);
                unrecognised_char_count++;
            } else if (index >= 0) {

                // update ngram
                update_ngram(ngram, index);

                // register ngram
                register_ngram(ngram, corpusDat, num_of_ngrams_stored);

            } else if ((unsigned)(-index - 1) < sizeof(extCharsList) / sizeof(char)) { // the extended chararacter map contains strings of characters to use in place of any non-alphabetical character

                if (extCharMap[-index - 1].length()) {

                    // update ngram
                    update_ngram(ngram, alpha_index[(int)extCharMap[-index - 1].at(0)]);

                    // register ngram
                    register_ngram(ngram, corpusDat, num_of_ngrams_stored);

                    for (unsigned j{1}; j < extCharMap[-index - 1].length(); j++) {
                        //update ngram
                        update_ngram(ngram, alpha_index[(int)extCharMap[-index - 1].at(j)]);

                        // register ngram
                        register_ngram(ngram, corpusDat, num_of_ngrams_stored);
                    }
                }
            } else {
                // maybe do something here for special behaviour when alpha_index is set to a big negative value
            }
        }

        delete[] memblock;

        //ngramData realloc
        unsigned long long total_ngram_count = ngram_t::ngramCount;

        ngram_t* temp_ngram_data = new ngram_t [total_ngram_count];
        unsigned long long i{ };
        while (i < num_of_ngrams_stored) {
            temp_ngram_data[i] = ngramData[i];
            i++;
        }
        while (i < total_ngram_count) {
            temp_ngram_data[i] = *corpusDat[i - num_of_ngrams_stored];
            i++;
        }
        delete[] ngramData;
        ngramData = temp_ngram_data;

        for (unsigned long long i{ }; i < total_ngram_count - num_of_ngrams_stored; i++) delete corpusDat[i];

    } else cout << "Unable to open corpus file ./corpus/" << corpusFile << "\n";

    if (unrecognised_char_count) {
        if (unrecognised_char_count == 1) {
            cout << "\ncorpusRead: note: There was one unrecognised character in corpus file (" + corpusFile + ").\n\n";
        } else {
            cout << "\ncorpusRead: note: There were " << unrecognised_char_count << " unrecognised characters in corpus file (" + corpusFile + ").\n\n";
        }
    }
}

// write ngram frequency data to a file (note: should be compatible with csv format)
void corpusRecord(string corpusDataFile) {
    ofstream corpusData("corpus/" + corpusDataFile);
    corpusData << "index,frequency\n";
    for (unsigned long long i{ }; i < ngram_t::ngramCount; i++)
        corpusData << get_ngram_index(ngramData[i].ngram) << "," << ngramData[i].frequency << "\n";
    corpusData.close();
}

// only considers lines until the first empty line
void corpusLoad(string corpusDataFile) {

    unsigned long long num_of_ngrams_stored = ngram_t::ngramCount;

    ifstream corpusData("corpus/" + corpusDataFile);

    string line;
    unsigned long long lineCount{ };
    unsigned long long corpusDataIndex{ };
    unsigned long long index;
    unsigned long long freq;

    if(corpusData.is_open()) {

        getline(corpusData, line);

        while (corpusData.peek() != EOF) {
            getline(corpusData, line);
            if (line == "") break;
            lineCount++;
        }

        printf("LineCount = %llu\n", lineCount);

        ngram_t* corpusDat[lineCount];

        corpusData.close();
        corpusData.open("corpus/" + corpusDataFile, ifstream::in);

        getline(corpusData, line);

        while (corpusData.peek() != EOF) {
            getline(corpusData, line);
            if (line == "") break;
            stringstream ss(line);
            ss >> index;
            if (ss.peek() == ',') {
                ss.ignore();
            }
            ss >> freq;

            corpusDat[corpusDataIndex] = new ngram_t(freq, index_to_ngram(index));
            corpusDataIndex++;
        }

        corpusData.close();

        //ngramData realloc
        unsigned long long total_ngram_count = ngram_t::ngramCount;

        ngram_t* temp_ngram_data = new ngram_t [total_ngram_count];
        unsigned long long i{ };
        while (i < num_of_ngrams_stored) {
            temp_ngram_data[i] = ngramData[i];
            i++;
        }
        while (i < total_ngram_count) {
            temp_ngram_data[i] = *corpusDat[i - num_of_ngrams_stored];
            i++;
        }
        delete[] ngramData;
        ngramData = temp_ngram_data;

        for (unsigned long long i{ }; i < total_ngram_count - num_of_ngrams_stored; i++) delete corpusDat[i];

    } else cout << "Unable to open corpus data file ./corpus/" << corpusDataFile << "\n";

}




int main(int argc, char* argv[]) {

    gen_alpha_index();

    ngramLength = 6;
    //ngramData = nullptr;

    corpusRead("testcorpus.txt");
    printf("\n\n\n\n\n\n\n\n");
    corpusRead("simplecorpus.txt");

    //corpusLoad("combination_corpus.csv");

    corpusRecord("combination_corpus.csv");

    printf("ngramCount = %llu\n", ngram_t::ngramCount);

    print_ngramMatrix();

    delete[] ngramData;

    return 0;
}



// a couple of fun print functions

void print_ngram_of_index(unsigned long long index) {
    if (index >= pow(sizeof(alphabet) / sizeof(char), ngramLength)) {
        return;
    }
    char* ngram = index_to_ngram(index);
    printf("(");
    unsigned j{ };
    while (j < ngramLength - 1) {
        printf("%2d, ", ngram[j]);
        j++;
    }
    printf("%2d)", ngram[j]);
}

void print_all_possible_ngrams() { //note: a bit dangerous if you can't ctrl-c your terminal
    for (unsigned long long i{ }; i < pow(sizeof(alphabet) / sizeof(char), ngramLength); i++) {
        print_ngram_of_index(i);
    }
}


// a few debugging print functions

void print_ngramMatrix() {
    for (unsigned long long i{ }; i < ngram_t::ngramCount; i++) {
        printf("%2llu  (", ngramData[i].frequency);
        unsigned j{ };
        while(j < ngramLength - 1) {
            printf("%2d, ", ngramData[i].ngram[j]);
            j++;
        }
        printf("%2d)\n", ngramData[i].ngram[j]);
    }
}

void print_ngram(char* ngram) {
    printf("(");
    unsigned i{ };
    while (i < ngramLength - 1) {
        printf("%2d, ", ngram[i]);
        i++;
    }
    printf("%2d)", ngram[i]);
}





void gen_alpha_index() {
    int extra_char_count = sizeof(extCharsList) / sizeof(char);
    for (int i{ }; i < extra_char_count; i++) {
        alpha_index[(int)extCharsList[i]] = -(i + 1);
    }
    for (unsigned i{ }; i < sizeof(alphabet) / sizeof(char); i++) {
        alpha_index[(int)alphabet[i]] = i;
    }
}
