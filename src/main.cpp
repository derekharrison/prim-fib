/*
 * main.cpp
 *
 *  Created on: Aug 9, 2020
 *      Author: d-w-h
 */

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "../inc/fib_heap.hpp"
#include "../inc/prim.hpp"
#include "../inc/memory.hpp"
#include "../inc/user_types.hpp"
#include "../inc/support.hpp"

int main(int argc, char* argv[])
{
    //Declarations
    int s = 2; //Start vertex. The minimum index for vertices is 1
    int n = 470; //Number of vertices
    int num_edges = 600; //Number of edges

    //Create edges
    std::vector< edge > edges;
    for(int i = 0; i < num_edges; ++i) {
        int start_vert = rand() % n + 1;
        int end_vert = rand() % n + 1;
        float weight =  (( (float) rand() ) / RAND_MAX + 1.0) * 3.14159;

        edge edge_elem;
        edge_elem.start_vertex = start_vert;
        edge_elem.end_vertex = end_vert;
        edge_elem.weight = weight;
        edges.push_back(edge_elem);
    }

    //Compute minimum spanning tree
    mst_props min_span_props = mst(n, edges, s);

    //Print results
    print_mst(n, min_span_props.node_refs);
    std::cout << "size of minimum spanning tree: " << min_span_props.mst_weight << std::endl;
    std::cout << "done" << std::endl;

    //Free node references
    free_node_refs(min_span_props.node_refs, n);

    return 0;
}
