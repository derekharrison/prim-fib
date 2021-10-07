/*
 * prim.cpp
 *
 *  Created on: Oct 7, 2021
 *      Author: d-w-h
 */

#include <cstdlib>
#include <iostream>
#include <time.h>
#include <vector>
#include <math.h>

#include "../inc/fib_heap.hpp"
#include "../inc/memory.hpp"
#include "../inc/user_types.hpp"

int map_index(int n, int index, int s) {
    int r;

    if(index >= s) { r = index - s; }
    else { r = n - s + index; }

    return r;
}

void populate_weight_and_ref(int size_graph,
                             std::vector<edge>& edges,
                             int start_vertex,
                             FibHeap* H,
                             float** weight_mat,
                             node** node_refs) {

    //Allocate set flags
    int** elem_is_set = int2D(size_graph);

    for(int i = 0; i < size_graph; ++i) {
        node_refs[i] = new node;
        node_refs[i]->key = inf;
        node_refs[i]->index = i;
        node_refs[i]->in_q = true;
        node_refs[i]->pi = NULL;
        if(i == 0) {
            node_refs[i]->key = 0.0;
        }
        fib_heap_insert(H, node_refs[i]);
    }

    //Add references to adjacent nodes
    int num_edges = (int) edges.size();
    for(int i = 0; i < num_edges; ++i) {
        int start_index = edges[i].start_vertex - 1;
        int end_index = edges[i].end_vertex - 1;
        float weight = edges[i].weight;

        int start =  map_index(size_graph, start_index, start_vertex);
        int end = map_index(size_graph, end_index, start_vertex);

        node_refs[start]->adj_nodes.push_back(end);
        node_refs[end]->adj_nodes.push_back(start);

        bool is_set = elem_is_set[start][end] == SETVAR;
        if(!is_set) {
            weight_mat[start][end] = weight;
            weight_mat[end][start] = weight;
            elem_is_set[start][end] = SETVAR;
            elem_is_set[end][start] = SETVAR;
        }
        else {
            bool is_greater = weight_mat[start][end] >= weight;
            if(is_greater) {
                weight_mat[start][end] = weight;
                weight_mat[end][start] = weight;
            }
        }
    }

    //Free set flags
    free_int2D(elem_is_set, size_graph);
}

void prim(FibHeap* H, float** w, node** node_refs) {

    //Perform Prim's algorithm
    while(H->n > 0) {
        node* u = fib_heap_extract_min(H);

        //Set u presence in set Q to false
        u->in_q = false;

        int num_adj_nodes = (int) u->adj_nodes.size();
        for(int i = 0; i < num_adj_nodes; ++i) {
            int index_ref = u->adj_nodes[i];
            node* v = node_refs[index_ref];

            if(v->in_q && v->key > w[u->index][v->index]) {
                float weight = w[u->index][v->index];
                v->pi = u;
                v->key = weight;
                fib_heap_decrease_key(H, v, weight);
            }

        }
    }
}

float weight_mst(int size_heap, node** node_refs) {
    float total_weight_mst = 0.0;
    for(int i = 0; i < size_heap; ++i) {
        if(node_refs[i]->pi != NULL) {
            float weight = node_refs[i]->key;
            total_weight_mst += weight;
        }
    }

    return total_weight_mst;
}

void print_mst(int size_heap, node** node_arr) {
    for(int i = 0; i < size_heap; ++i) {
        if(node_arr[i]->pi != NULL) {
            int parent_index = node_arr[i]->pi->index;
            int current_index = node_arr[i]->index;
            float weight = node_arr[i]->key;
            std::cout << "node index: " << current_index << ", ";
            std::cout << "parent index: " << parent_index << ", ";
            std::cout << "weight: " << weight;
            std::cout << std::endl;
        }
    }
}

mst_props mst(int n, std::vector<edge>& edges, int s) {

    //Declarations
    FibHeap H;
    float mst_weight = 0.0;

    //Map start vertex s to s - 1
    s = s - 1;

    //Initialize heap reference and weight matrix
    int num_nodes = n;
    node** node_refs = new node*[num_nodes]; //The caller of mst(n, edges, s) needs to free node references
    float** weight_mat = float2D(n);

    //Populate weight and heap references
    populate_weight_and_ref(n, edges, s, &H, weight_mat, node_refs);

    //Perform Prim's algorithm
    prim(&H, weight_mat, node_refs);

    //Compute MST weight
    mst_weight = weight_mst(n, node_refs);

    //Store MST properties
    mst_props min_span_props;
    min_span_props.mst_weight = mst_weight;
    min_span_props.node_refs = node_refs;

    //Free weight matrix
    free_float2D(weight_mat, n);

    return min_span_props;
}

