#include <stdio.h>
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;

#define LAYOUT_SIZE 6


class layout {
public:
    int key[LAYOUT_SIZE];									// key[n] returns the key on which the letter with index n resides
};

layout colemak;

unsigned ngramLength;

long double digraphScores[6][6];



long double ngramTimeRecurse(layout lyt, vector<int> ngram, long double greatestTime, long double timeSum, int length, int remainder) {
    long double time;
    for (int i{1}; i < remainder; ++i) {
        time = timeSum + digraphScores[lyt.key[ngram[length - remainder]]][lyt.key[ngram[length - remainder + i]]];
        if (i == remainder - 1) {
            double t = time;
            printf("End: time = %f\n", t);
            if (time > greatestTime) {
                greatestTime = time;
            }
        } else {
            greatestTime = ngramTimeRecurse(lyt, ngram, greatestTime, time, length, remainder - i);
        }
    }
    return greatestTime;
}

long double ngramScore(layout lyt, vector<int> ngram, unsigned length) {
    long double gtime = ngramTimeRecurse(lyt, ngram, 0, 0, length, length);
    printf("Greatest Time: %Lf\n", gtime);

    long double gnm1time = ngramTimeRecurse(lyt, ngram, 0, 0, length - 1, length - 1);
    printf("Greatest n-1 Time: %Lf\n", gnm1time);

    printf("Time Difference: %Lf\n", gtime - gnm1time);

    return ngramTimeRecurse(lyt, ngram, 0, 0, length, length) - ngramTimeRecurse(lyt, ngram, 0, 0, length -1, length - 1);
}


// long double ngramTimeRecurse(long double greatestTime, long double timeSum, int remainder) {
//   long double time;
//   for (int i{1}; i < remainder; ++i) {
//     time = timeSum * 10 + i;
//     if (i == remainder - 1) {
//       double t = time;
//       printf("[ %.0f ]\n", t);
//     } else {
//       greatestTime = ngramTimeRecurse(greatestTime, time, remainder - i);
//     }
//   }
//   return greatestTime;
// }


int main() {

    // digraphScores[0] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0};
    // digraphScores[1] = {6.0, 7.0, 8.0, 9.0, 10., 11.};
    // digraphScores[2] = {12., 13., 14., 15., 16., 17.};
    // digraphScores[3] = {18., 19., 20., 21., 22., 23.};
    // digraphScores[4] = {24., 25., 26., 27., 28., 29.};
    // digraphScores[5] = {30., 31., 32., 33., 34., 35.};

    for (int i{ }; i < 6; i++) {
        for (int j{ }; j < 6; j++) {
            digraphScores[i][j] = 6.0 * i + j;
        }
    }

    colemak.key[0] = 0;
    colemak.key[1] = 1;
    colemak.key[2] = 2;
    colemak.key[3] = 3;
    colemak.key[4] = 4;
    colemak.key[5] = 5;

    ngramLength = 4;

    int ngramFrequencies[(int)pow(LAYOUT_SIZE, ngramLength)];

    // test initialisation of ngramFrequencies array
    for (int i{ }; i < pow(LAYOUT_SIZE, ngramLength); i++) {
        if (i % 10 == 0) {
            ngramFrequencies[i] = 1;
        } else {
            ngramFrequencies[i] = 0;
        }
    }

    vector<int> vngram (ngramLength);

    vngram[0] = 1;
    vngram[1] = 3;
    vngram[2] = 2;
    vngram[3] = 4;

    int ngram[ngramLength];


    long double score = ngramScore(colemak, vngram, ngramLength);
    printf("Score = %Lf\n\n", score);

    // print out digraphScores[][]
    for (int i{ }; i < 6; i++) {
        long double score;
        printf("digraphScores[%d] = { ", i);
        for (int j{ }; j < 5; j++) {
            score = digraphScores[i][j];
            printf("%2.0Lf, ", score);
        }
        score = digraphScores[i][5];
        printf("%2.0Lf }\n", score);
    }

    // print out ngramFrequencies[]
    printf("ngramFrequencies[] = { ");
    for (int i{ }; i < pow(LAYOUT_SIZE, ngramLength) - 1; i++) {
        printf("%d, ", ngramFrequencies[i]);
    }
    printf("%d }\n", ngramFrequencies[(int)pow(LAYOUT_SIZE, ngramLength) - 1]);


    return 0;
}
