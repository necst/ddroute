#ifndef UTILS
#define UTILS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <queue>
#include <random>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

#define BREAK_TIES_RANDOM 1
#define TIE_BREAK(same_score) (BREAK_TIES_RANDOM && (std::rand()%same_score == 0))

#define OPERANDS_LOOKAHEAD 10
#define OPERANDS_LOOKAHEAD_DECAY 0.5
#define SCORE_LA(sd) (2.25*(sd/3.0))
#define SCORE_SWAP(sd) (1.5*(sd/3.0))

inline bool IS_BETTER(double a, double b, bool tie_break, int &same_score){
    if(a<0) return 0;
    if(b<0 || a<b){
        if(tie_break) same_score = 1;
        return 1;
    }
    if(fabs(a-b)==0 && tie_break){
        same_score++;
        return TIE_BREAK(same_score);
    }
    return 0;
}

/**
 * Structure used for computing the lookahead ('next') component of the generalized distance function
 */
typedef struct front_layer_prefs_t{
    int size;
    int op_1[2*OPERANDS_LOOKAHEAD];
    int op_2[2*OPERANDS_LOOKAHEAD];
    double coeff[2*OPERANDS_LOOKAHEAD];
}front_layer_prefs;

/**
 * Array-based heap with additional indexing
 */
struct MyHeap{

    std::vector<double> scores;
    std::vector<int> vals;
    std::vector<int> idxs;
    int size;

    MyHeap(int size):size(size){
        scores = std::vector<double>();
        vals = std::vector<int>();
        idxs = std::vector<int>();

        scores.insert(scores.begin(),size,-1);
        vals.insert(vals.begin(),size,-1);
        idxs.insert(idxs.begin(),size,-1);
    }

    void heapify_up(int idx, bool tie_break, int &same_score){
        int parent = (idx-1)/2;
        int tmp;
        while(idx>0 && IS_BETTER(scores[vals[idx]],scores[vals[parent]],tie_break,same_score)){
            
            idxs[vals[idx]] = parent;
            idxs[vals[parent]] = idx;

            tmp = vals[idx];
            vals[idx] = vals[parent];
            vals[parent] = tmp;

            idx = parent;
            parent = (idx-1)/2;
        }
    }

    void heapify_down(int idx, bool tie_break, int &same_score){
        int left, right, min_i;
        int tmp;
        while(1){
            left = idx*2+1;
            right = idx*2+2;
            min_i = idx;

            if(left< size && IS_BETTER(scores[vals[left]],scores[vals[min_i]],tie_break,same_score)) min_i = left;
            if(right< size && IS_BETTER(scores[vals[right]],scores[vals[min_i]],tie_break,same_score)) min_i = right;

            if(min_i != idx){
                idxs[vals[idx]] = min_i;
                idxs[vals[min_i]] = idx;

                tmp = vals[idx];
                vals[idx] = vals[min_i];
                vals[min_i] = tmp;

                idx = min_i;
            }else return;
        }
    }

    //Before decreasing size
    void heap_pop_min(bool tie_break, int &same_score){
        vals[0] = vals[size-1];
        idxs[vals[0]] = 0;
        size--;
        heapify_down(0,tie_break,same_score);
    }

};

/**
 * Array-based heap with additional data and indexing
 */
struct MyTripleHeap{

    std::vector<double> scores;

    std::vector<double> depth;
    std::vector<int> swap;
    std::vector<double> la_score;

    std::vector<int> vals;
    std::vector<int> idxs;
    int size;

    MyTripleHeap(int size):size(size){
        scores = std::vector<double>();
        vals = std::vector<int>();
        idxs = std::vector<int>();

        scores.insert(scores.begin(),size,-1);
        vals.insert(vals.begin(),size,-1);
        idxs.insert(idxs.begin(),size,-1);

        depth.insert(depth.begin(),size,-1);
        swap.insert(swap.begin(),size,-1);
        la_score.insert(la_score.begin(),size,-1);
    }

    void heapify_up(int idx, bool tie_break, int &same_score){
        int parent = (idx-1)/2;
        int tmp;
        while(idx>0 && IS_BETTER(scores[vals[idx]],scores[vals[parent]],tie_break,same_score)){
            
            idxs[vals[idx]] = parent;
            idxs[vals[parent]] = idx;

            tmp = vals[idx];
            vals[idx] = vals[parent];
            vals[parent] = tmp;

            idx = parent;
            parent = (idx-1)/2;
        }
    }

    void heapify_down(int idx, bool tie_break, int &same_score){
        int left, right, min_i;
        int tmp;
        while(1){
            left = idx*2+1;
            right = idx*2+2;
            min_i = idx;

            if(left< size && IS_BETTER(scores[vals[left]],scores[vals[min_i]],tie_break,same_score)) min_i = left;
            if(right< size && IS_BETTER(scores[vals[right]],scores[vals[min_i]],tie_break,same_score)) min_i = right;

            if(min_i != idx){
                idxs[vals[idx]] = min_i;
                idxs[vals[min_i]] = idx;

                tmp = vals[idx];
                vals[idx] = vals[min_i];
                vals[min_i] = tmp;

                idx = min_i;
            }else return;
        }
    }

    void heap_pop_min(bool tie_break, int &same_score){
        vals[0] = vals[size-1];
        idxs[vals[0]] = 0;
        size--;
        heapify_down(0,tie_break,same_score);
    }

};

/**
 * Allocate matrix
 */
int **new_matrix(int rows, int cols){
    int **res;

    res = (int**) malloc(sizeof(int*)*rows);
    for(int i=0; i<rows; i++){
        res[i] = (int*) calloc(sizeof(int),cols);
    }

    return res;
}

/**
 * Deallocate matrix
 */
void free_matrix(int **mat, int rows){
    for(int i=0; i<rows; i++){
        free(mat[i]);
    }
    free(mat);
}

/**
 * Check if front layer is empty
 */
int is_empty_fl(std::vector<bool> &fl){
    for(unsigned int i=0; i<fl.size(); i++){
        if(fl[i]) return false;
    }
    return true;
}

#endif