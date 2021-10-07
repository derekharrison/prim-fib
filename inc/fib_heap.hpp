/*
 * functions.hpp
 *
 *  Created on: Aug 13, 2020
 *      Author: d-w-h
 */

#ifndef FIBHEAP_HPP_
#define FIBHEAP_HPP_

#include "user_types.hpp"

void fib_heap_insert(FibHeap* H, node* x);
node* fib_heap_extract_min(FibHeap* H);
void fib_heap_decrease_key(FibHeap* H, node* x, float k);

#endif /* FIBHEAP_HPP_ */
