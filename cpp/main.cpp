//============================================================================
// Name        : main.cpp
// Author      : Matthew Hinton
// Version     :
// Copyright   : Copyright (C) 2023 Matthew Hinton
// Description : Main function call for Engender, includes hardcoded settings
//============================================================================

#include "main.h"


void gen_alpha_index() {
    int extra_char_count = sizeof(extCharsList) / sizeof(char);
    for (int i{ }; i < extra_char_count; i++) {
        alpha_index[(int)extCharsList[i]] = -(i + 1);
    }
    for (unsigned i{ }; i < sizeof(alphabet) / sizeof(char); i++) {
        alpha_index[(int)alphabet[i]] = i;
    }
}

int main(int argc, char* argv[]) {

    /* From layouts.cpp:
     *
     gen_alpha_index();

    fs::path layoutDir = fs::current_path();

    MetaLayout** layouts = loadMetaLayouts(layoutDir);

    printf("Number of layouts: %d\n\n", MetaLayout::numberof);

    // print all layouts
    cout << "Here is a list of all the loaded layouts:" << endl
        << string(41, '-') << endl;
    for (int i{ }; i < MetaLayout::numberof; i++)
        print_layout(layouts[i]);

    return 0;
     */

    // digraphScores[0] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0};
    // digraphScores[1] = {6.0, 7.0, 8.0, 9.0, 10., 11.};
    // digraphScores[2] = {12., 13., 14., 15., 16., 17.};
    // digraphScores[3] = {18., 19., 20., 21., 22., 23.};
    // digraphScores[4] = {24., 25., 26., 27., 28., 29.};
    // digraphScores[5] = {30., 31., 32., 33., 34., 35.};

    // Manually set some digraphScores
    // for (int i{ }; i < LayoutSize; i++) {
    //     for (int j{ }; j < LayoutSize; j++) {
    //         digraphScores[i][j] = pow(4.0, pow((LayoutSize * i + j - 480) / 480.0, 31));
    //     }
    // }

    //typingTest(defaultWeightsFile);
    setWeights(defaultWeightsFile);

    printf("ngramLength = %u\n", ngramLength);

    // print out digraphScores[][]
    for (int i{ }; i < LayoutSize; i++) {
        double score;
        printf("digraphScores[%2d] = { ", i);
        int j{ };
        while (j < LayoutSize - 1) {
            score = digraphScores[i][j];
            printf("%.3f,", score);
            j++;
        }
        score = digraphScores[i][j];
        printf("%.3f }\n", score);
    }

    gen_alpha_index();

    //corpusRead("testcorpus.txt");
    printf("\n\n\n\n\n\n\n\n");
    //corpusRead("simplecorpus.txt");

    corpusLoad("combination_corpus.csv");

    //corpusRecord("combination_corpus.csv");

    printf("ngramCount = %llu\n", ngram_t::ngramCount);

    print_ngramMatrix();

    printf("ngramLength = %d\n", ngramLength);

    /*
     * from layouts.cpp
     */
    fs::path layoutDir = fs::current_path() / "layouts";

    const MetaLayout** layouts = loadMetaLayouts(layoutDir);

    printf("Number of layouts: %d\n\n", MetaLayout::numberof);

    // print all layouts
    cout << "Here is a list of all the loaded layouts:" << endl
        << string(41, '-') << endl;
    for (int i{ }; i < MetaLayout::numberof; i++)
        print_layout(layouts[i]);


    /*
    char* ngram = new char [ngramLength]();

    ngram[0] = 1;
    ngram[1] = 3;
    ngram[2] = 2;
    ngram[3] = 4;
    ngram[4] = 29;
    ngram[5] = 16;


    long double score = colemak.layout.ngramScore(ngram);
    printf("Score ");
    print_ngram(ngram);
    printf(" = %Lf\n\n", score);

    delete[] ngram;
    */


    // score layouts and print result
    cout << endl << endl;
    for (int i{ }; i < MetaLayout::numberof; i++) {
        cout << (*layouts[i]).name << " score: " << (*layouts[i]).score() << endl;
    }



    cout << endl << endl << endl;
    MetaLayout repermuted_layout((*layouts[0]).layout.evolve(12));
    cout << endl << endl << endl;
    print_layout(&repermuted_layout);

    cout << repermuted_layout.score() << endl << endl << endl;


    int x;
    printf("Pointer to x: [%p] !!!\n", &x);
    cout << endl << endl;

    cout << "Start generating layouts." << endl << endl << endl;






    initialiseDupletMatrix();

    for (int i{ }; i < 100; i++) {
        evolveDupletMatrix();
        const MetaLayout generated{nodes[0].layout};
        print_layout(&generated);
        cout << generated.score() << endl << endl;
    }


    delete[] ngramData;

    return 0;
}
