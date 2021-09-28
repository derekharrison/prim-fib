/*
 * user_types.hpp
 *
 *  Created on: Aug 13, 2020
 *      Author: d-w-h
 */

#ifndef USER_TYPES_HPP_
#define USER_TYPES_HPP_

#include <vector>

const float inf = 3e+8;
const int SETVAR = 314159;

typedef struct Edge {
    int start_vertex;
    int end_vertex;
    float weight;
} edge;

typedef struct FibHeapProperties {
    bool deg_is_num_child;
    int num_nodes;
} fib_props;

typedef struct Node {
    Node* left;
    Node* right;
    Node* p;
    Node* child;
    Node* pi;

    std::vector<int> adj_nodes;

    float key;
    int degree;
    int index;
    bool mark;

    bool in_q;
} node;

typedef struct MSTProperties {
	float mst_weight;
	node** node_arr;
} mst_props;

class FibHeap {
public:
    int n;
    node* min;
    FibHeap() { min = NULL; n = 0; }
};

#endif /* USER_TYPES_HPP_ */
