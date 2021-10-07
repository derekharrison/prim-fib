/*
 * functions.cpp
 *
 *  Created on: Aug 13, 2020
 *      Author: d-w-h
 */

#include <cstdlib>
#include <iostream>
#include <time.h>
#include <vector>
#include <math.h>

#include "../inc/memory.hpp"
#include "../inc/user_types.hpp"

void fib_heap_insert(FibHeap* H, node* x) {
    x->degree = 0;
    x->p = NULL;
    x->pi = NULL;
    x->child = NULL;
    x->mark = false;

    if(H->min == NULL) {
        x->left = x;
        x->right = x;
        H->min = x;
        H->n = 0;
    }
    else {
        x->left = H->min;
        x->right = H->min->right;
        H->min->right->left = x;
        H->min->right = x;
        if(x->key < H->min->key) {
            H->min = x;
        }
    }

    H->n = H->n + 1;
}

void print_root_list(node* z) {
    node* xt = z;
    if(xt != NULL) {
        do {
            std::cout << "xt->key: " << xt->key;
            std::cout << ", xt->degree: " << xt->degree << std::endl;
            xt = xt->right;
        } while(xt != z);
    }
}

void make_child_of(FibHeap* H, node* y, node* x) {

    //Remove node from root list
    y->left->right = y->right;
    y->right->left = y->left;

    if(x->child == NULL) {
        x->child = y;
        y->p = x;
        y->left = y;
        y->right = y;
    }
    else {
        y->left = x->child;
        y->right = x->child->right;
        y->p = x;
        x->child->right->left = y;
        x->child->right = y;
    }

    //Set mark
    y->mark = false;

    x->degree = x->degree + 1;
}

void link(FibHeap* H, node** A, node* x, node* y, int d) {
    //Make y child of x;
     make_child_of(H, y, x);

     A[d] = NULL;
     A[d+1] = x;

    if(y == H->min) {
        H->min = x;
    }
}

void link_dup_deg(FibHeap* H, node** A, node*& x, bool& there_is_dup) {
    int d = x->degree;
    //There is a node with the same degree and node A[d] is not node x
    if(A[d] != NULL && A[d] != x) {
        there_is_dup = true;
        node* y = A[d];
        //Link x and y
        if(y->key > x->key) {
            //Make y child of x;
            link(H, A, x, y, d);
        }
        else {
            //Make x child of y;
            link(H, A, y, x, d);
            x = y;
        }
    }
    //There is no node with the same degree or node A[d] is node x
    else {
        A[d] = x;
    }
}

void consolidate(FibHeap* H) {

    //Compute upper bound root list size
    double golden = (1.0 + sqrt(5.0)) / 2.0;
    double f = log(H->n) / log(golden);
    int D = floor(f + 0.01) + 1;

    //Allocate memory for root list construction
    node** A = new node*[D + 1];
    for(int i = 0; i < D + 1; ++i) {
        A[i] = NULL;
    }

    //Ensure all root nodes have unique degrees
    node* x = H->min;
    if(x != NULL) {
        bool there_is_dup = true;
        while(there_is_dup) {
            there_is_dup = false;
            x = H->min;
            do {
                link_dup_deg(H, A, x, there_is_dup);
                x = x->right;
            } while(x != H->min);
        }
    }

    //Reconstruct root list
    H->min = NULL;
    for(int i = 0; i < D + 1; ++i) {
        if(A[i] != NULL) {
            if(H->min == NULL) {
                A[i]->left = A[i];
                A[i]->right = A[i];
                A[i]->p = NULL;
                H->min = A[i];
            }
            else {
                A[i]->left = H->min;
                A[i]->right = H->min->right;
                H->min->right->left = A[i];
                H->min->right = A[i];
                A[i]->p = NULL;
                if(A[i]->key < H->min->key) {
                    H->min = A[i];
                }
            }
        }
    }
}

void print_child_list(node* child) {
    node* xt = child;
    if(xt != NULL) {
        do {
            std::cout << "xt->child->key: " << xt->key;
            std::cout << ", xt->child->degree: " << xt->degree << std::endl;
            if(xt->child != NULL) {
                std::cout << "xt->child->child->key: " << xt->child->key << std::endl;
            }
            xt = xt->right;
        } while(xt != child);
    }
}

void print_list(node* z) {
    node* xt = z;
    if(xt != NULL) {
        do {
            std::cout << "xt->key: " << xt->key;
            std::cout << ", xt->degree: " << xt->degree << std::endl;
            if(xt->child != NULL) {
                print_child_list(xt->child);
            }
            xt = xt->right;
        } while(xt != z);
    }
}

bool numbers_children_match(node* z, int& num_nodes) {
    bool nums_match = true;
    int num_of_nodes = 0;

    node* xt = z->child;
    if(xt != NULL) {
        do {
            num_of_nodes++;
            if(xt->child != NULL) {
                nums_match = numbers_children_match(xt, num_nodes);
                if(!nums_match) { return false; }
            }
            xt = xt->right;
        } while(xt != z->child);

        num_nodes = num_nodes + num_of_nodes;

        if(num_of_nodes == z->degree) { nums_match = true; }
        else { nums_match = false; }
    }

    return nums_match;
}

fib_props numbers_match(node* z) {

    bool nums_match = true;
    int num_nodes = 0;
    fib_props fib_heap_props = { nums_match, num_nodes };

    node* xt = z;
    if(xt != NULL) {
        do {
            num_nodes++;
            nums_match = numbers_children_match(xt, num_nodes);
            fib_heap_props.deg_is_num_child = nums_match;
            fib_heap_props.num_nodes = num_nodes;
            if(!nums_match) { return fib_heap_props; }
            xt = xt->right;
        } while(xt != z);
    }

    fib_heap_props.deg_is_num_child = nums_match;
    fib_heap_props.num_nodes = num_nodes;

    return fib_heap_props;
}

bool is_fib_heap_children(node* z) {
    bool is_fibheap = true;

    node* xt = z->child;
    if(xt != NULL) {
        do {
            if(xt->p->key > xt->key) {
                return is_fibheap = false;
            }
            if(xt->child != NULL) {
                is_fibheap = is_fib_heap_children(xt);
                if(!is_fibheap) { return false; }
            }
            xt = xt->right;
        } while(xt != z->child);
    }

    return is_fibheap;
}

void nullify_children_parent_node(node* z) {
    node* xt = z->child;
    if(xt != NULL) {
        do {
            xt->p = NULL;
            xt = xt->right;
        } while(xt != z->child);
    }
}

bool is_fib_heap(node* z) {
    bool is_fibheap = true;

    node* xt = z;
    if(xt != NULL) {
        do {
            is_fibheap = is_fib_heap_children(xt);
            if(!is_fibheap) { return false; }
            xt = xt->right;
        } while(xt != z);
    }

    return is_fibheap;
}

node* fib_heap_extract_min(FibHeap* H) {

    node* z = H->min;

    if(z != NULL) {
        //Add each child of z to root list
        node* y = z->child;
        if(y != NULL) {
            //Set children's parent node to NULL
            nullify_children_parent_node(z);

            y->left->right = z->right;
            z->right->left = y->left;
            y->left = z;
            z->right = y;
            z->degree = 0;

            z->child = NULL;
        }

        //Remove z from root list
        z->left->right = z->right;
        z->right->left = z->left;

        if(z == z->right) {
            H->min = NULL;
        }
        else {
            H->min = z->right;
            consolidate(H);
        }

        H->n = H->n - 1;

    }

    return z;

}

void cut(FibHeap* H, node* x, node* y) {

    //If x is only child set child of parent to null
    if(x == x->right) {
        y->child = NULL;
        y->degree = 0;
    }
    else {
        y->child = x->right;
        y->degree = y->degree - 1;
    }

    //Remove x from child list of y and add x to root list of H
    x->left->right = x->right;
    x->right->left = x->left;

    x->right = H->min->right;
    x->left = H->min;

    H->min->right->left = x;
    H->min->right = x;

    x->p = NULL;
    x->mark = false;
}

void cascading_cut(FibHeap* H, node* y) {
    node* z = y->p;
    if(z != NULL) {
        if(y->mark == false) {
            y->mark = true;
        }
        else {
            cut(H, y, z);
            cascading_cut(H, z);
        }
    }
}

void fib_heap_decrease_key(FibHeap* H, node* x, float k) {
    if(k > x->key) {
        const char* s = "new key is greater than current key";
        std::cout << s << std::endl;
        throw s;
    }

    x->key = k;
    node* y = x->p;
    if(y != NULL && x->key < y->key) {
        cut(H, x, y);
        cascading_cut(H, y);
    }

    if(x->key < H->min->key) {
        H->min = x;
    }
}

int map_index(int n, int index, int s) {
    int r;

    if(index >= s) { r = index - s; }
    else { r = n - s + index; }

    return r;
}

void populate_weight_and_ref(FibHeap* H,
                             int start_vertex,
                             float** weight_mat,
                             int size_graph,
                             std::vector<edge>& edges,
                             node** v_ref) {

    int** elem_is_set = int2D(size_graph);

    for(int i = 0; i < size_graph; ++i) {
        v_ref[i] = new node;
        v_ref[i]->key = inf;
        v_ref[i]->index = i;
        v_ref[i]->in_q = true;
        v_ref[i]->pi = NULL;
        if(i == 0) {
            v_ref[i]->key = 0.0;
        }
        fib_heap_insert(H, v_ref[i]);
    }

    //Add references to adjacent nodes
    int num_edges = (int) edges.size();
    for(int i = 0; i < num_edges; ++i) {
        int start_index = edges[i].start_vertex - 1;
        int end_index = edges[i].end_vertex - 1;
        float weight = edges[i].weight;

        int start =  map_index(size_graph, start_index, start_vertex);
        int end = map_index(size_graph, end_index, start_vertex);

        v_ref[start]->adj_nodes.push_back(end);
        v_ref[end]->adj_nodes.push_back(start);

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
}

bool check_fib_heap(FibHeap* H) {
    //This is the general test for the fibonacci heap.
    //The function returns true if the heap satisfies
    //the fibonacci heap properties

    //Compute heap properties
    fib_props fh_props = numbers_match(H->min);
    bool heap_is_fibheap = is_fib_heap(H->min);

    //Check if number of children equal node degrees
    bool deg_is_num_childs = fh_props.deg_is_num_child;

    //Check if number of nodes counted in heap equals H.n
    int num_nodes = fh_props.num_nodes;
    bool num_nodes_match = (num_nodes == H->n);

    //Check to see if heap is properly structured
    bool heap_is_ok = num_nodes_match && deg_is_num_childs && heap_is_fibheap;

    return heap_is_ok;
}


void prim(FibHeap* H, float** w, node** v_ref) {

    //Perform Prim's algorithm
    while(H->n > 0) {
        node* u = fib_heap_extract_min(H);

        //Set u presence in set Q to false
        u->in_q = false;

        int num_adj_nodes = (int) u->adj_nodes.size();
        for(int i = 0; i < num_adj_nodes; ++i) {
            int index_ref = u->adj_nodes[i];
            node* v = v_ref[index_ref];

            if(v->in_q && v->key > w[u->index][v->index]) {
                float weight = w[u->index][v->index];
                v->pi = u;
                v->key = weight;
                fib_heap_decrease_key(H, v, weight);
            }

        }
    }
}

float weight_mst(int size_heap, node** v_ref) {
    float total_weight_mst = 0.0;
    for(int i = 0; i < size_heap; ++i) {
        if(v_ref[i]->pi != NULL) {
            float weight = v_ref[i]->key;
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
    node** v_ref = new node*[num_nodes];
    float** weight_mat = float2D(n);

    //Populate weight and heap references
    populate_weight_and_ref(&H, s, weight_mat, n, edges, v_ref);

    //Perform Prim's algorithm
    prim(&H, weight_mat, v_ref);

    //Compute MST weight
    mst_weight = weight_mst(n, v_ref);

    //Store MST properties
    mst_props min_span_props;
    min_span_props.mst_weight = mst_weight;
    min_span_props.node_arr = v_ref;

    return min_span_props;
}
