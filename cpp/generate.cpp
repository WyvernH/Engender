/*
 * generate.cpp
 *
 *  Created on: Oct 16, 2023
 *      Author: Matthew Hinton
 */

#include "generate.h"


layout_t rand_layout() {
    layout_t alpha_layout( alphabetical_layout() );
    return alpha_layout.evolve(LayoutSize - 1);
}

void computeMatrix() {

}

void initialiseMatrix() {
    matrixDepth = matrixBreadth = LayoutSize;
    nodes = new node_t [matrixDepth * matrixBreadth + 1];
    for (unsigned j{ }; j < matrixBreadth; j++)
        nodes[0 + j].layout = rand_layout();
    for (unsigned i{1}; i < matrixDepth; i++)
        for (unsigned j{ }; j < matrixBreadth; j++) {
            nodes[matrixBreadth * i + j].synapse = new node_t* [matrixBreadth];
            for (unsigned k{ }; k < LayoutSize; k++)
                nodes[matrixBreadth * i + j].synapse[k] = &nodes[matrixBreadth * (i-1) + k];
        }
    nodes[matrixDepth * matrixBreadth].synapse = new node_t* [matrixBreadth];
    for (unsigned k{ }; k < LayoutSize; k++)
        nodes[matrixDepth * matrixBreadth].synapse[k] = &nodes[matrixBreadth * (matrixDepth - 1) + k];
    computeMatrix();
}



// idea where matrix loops two lines and evolves output linearly from left to right
void initialiseDupletMatrix() {
    matrixBreadth = LayoutSize;
    nodes = new node_t [2 * matrixBreadth];
    for (unsigned i{ }; i < matrixBreadth; i++) {
        nodes[i].layout = rand_layout();
        nodes[i].synapse = new node_t* [matrixBreadth];
        nodes[matrixBreadth + i].synapse = new node_t* [matrixBreadth];
        for (unsigned k{ }; k < matrixBreadth; k++) {
            nodes[i].synapse[k] = &nodes[matrixBreadth + i];
            nodes[matrixBreadth + i].synapse[k] = &nodes[i];
        }
    }
}

void evolveDupletMatrix() {
    layout_t temp_layout, iterated_layout;
    double temp_score = 1000;
    double iterated_score = 1000;
    for (unsigned i{ }; i < matrixBreadth; i++) {
        for (unsigned k{ }; k < matrixBreadth; k++) {
            iterated_layout = (*nodes[matrixBreadth + i].synapse[k]).layout.evolve(k);
            iterated_score = iterated_layout.score();
            if (iterated_score <= temp_score) {
                temp_layout = iterated_layout;
                temp_score = iterated_score;
            }
        }
        nodes[matrixBreadth + i].layout = temp_layout;
        temp_score = 1000;
    }
    // for (unsigned k{ }; k < matrixBreadth; k++) {
    //     iterated_layout = (*nodes[0].synapse)[0].layout.evolve(k);
    //     //iterated_score = iterated_layout.score();
    //     cout << iterated_score << endl << temp_score << endl;
    // }s
    // for (unsigned i{ }; i < matrixBreadth; i++) {
    //     for (unsigned k{ }; k < matrixBreadth; k++) {
    //         iterated_layout = (*nodes[i].synapse)[k].layout.evolve(k);
    //         iterated_score = iterated_layout.score();
    //         if (iterated_score <= temp_score) {
    //             temp_layout = iterated_layout;
    //             temp_score = iterated_score;
    //         }
    //     }
    //     nodes[i].layout = iterated_layout;
    //     temp_score = 100000;
    // }
    for (unsigned i{ }; i < matrixBreadth; i++) {
        for (unsigned k{ }; k < matrixBreadth; k++) {
            iterated_layout = (*nodes[i].synapse[k]).layout.evolve(k);
            iterated_score = iterated_layout.score();
            if (iterated_score <= temp_score) {
                temp_layout = iterated_layout;
                temp_score = iterated_score;
            }
        }
        nodes[i].layout = temp_layout;
        temp_score = 1000;
    }
}


void evolveNode() {
    layout_t temp_layout, iterated_layout;
    double temp_score = 1000;
    double iterated_score = 1000;

    for (unsigned k{ }; k < LayoutSize; k++) {
        iterated_layout = nodes[0].layout.evolve(k);
        iterated_score = iterated_layout.score();
        if (iterated_score <= temp_score) {
            temp_layout = iterated_layout;
            temp_score = iterated_score;
        }
    }
    nodes[0].layout = temp_layout;
}
