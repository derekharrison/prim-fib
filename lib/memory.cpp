/*
 * memory.cpp
 *
 *  Created on: Oct 7, 2021
 *      Author: d-w-h
 */

#include "../inc/user_types.hpp"

int** int2D(const int size) {
    int** p = new int*[size];

    for(int i = 0; i < size; ++i)
        p[i] = new int[size];

    return p;
}

float** float2D(const int size) {
    float** p = new float*[size];

    for(int i = 0; i < size; ++i)
        p[i] = new float[size];

    return p;
}

void free_int2D(int** p, int size) {
    for(int i = 0; i < size; ++i)
        delete [] p[i];

    delete [] p;
}

void free_float2D(float** p, int size) {
    for(int i = 0; i < size; ++i)
        delete [] p[i];

    delete [] p;
}

void free_node_refs(node** p, int size) {
    for(int i = 0; i < size; ++i)
        delete p[i];

    delete [] p;
}
