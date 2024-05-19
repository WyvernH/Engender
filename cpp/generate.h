/*
 * generate.h
 *
 *  Created on: Oct 16, 2023
 *      Author: Matthew Hinton
 */

#ifndef GENERATE_H_
#define GENERATE_H_

#include "analyse.h"

struct node_t {
    layout_t layout;
    node_t** synapse; // <-- array of pointers to nodes "above" it; used to evolve the matrix
};

inline node_t* nodes;

inline unsigned matrixDepth;
inline unsigned matrixBreadth;

layout_t alphabetical_layout();


#endif /* GENERATE_H_ */
