/*
 * memory.hpp
 *
 *  Created on: Oct 7, 2021
 *      Author: d-w-h
 */

#ifndef INC_MEMORY_HPP_
#define INC_MEMORY_HPP_

#include "user_types.hpp"

int** int2D(const int size);
float** float2D(const int size);
void free_int2D(int** p, int size);
void free_float2D(float** p, int size);
void free_node_refs(node** p, int size);

#endif /* INC_MEMORY_HPP_ */
