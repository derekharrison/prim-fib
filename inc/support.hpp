/*
 * support.hpp
 *
 *  Created on: Oct 7, 2021
 *      Author: d-w-h
 */

#ifndef INC_SUPPORT_HPP_
#define INC_SUPPORT_HPP_

#include "../inc/user_types.hpp"

bool check_fib_heap(FibHeap* H);
void print_root_list(node* z);
void print_child_list(node* child);
void print_list(node* z);
void print_mst(int size_heap, node** node_arr);

#endif /* INC_SUPPORT_HPP_ */
