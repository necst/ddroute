#ifndef TOPOLOGY
#define TOPOLOGY

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <queue>
#include <random>

#include "utils.hpp"

struct DDRTopology{
    int num_nodes;
    std::vector<std::vector<int>> adjacent;
    std::vector<std::vector<int>> distance;
    std::vector<std::vector<int>> predecessor;
    bool distances_updated;
    
    DDRTopology(int num_nodes) : num_nodes(num_nodes){
        adjacent.insert(adjacent.begin(),num_nodes,std::vector<int>());
        distance.insert(distance.begin(),num_nodes,std::vector<int>());
        predecessor.insert(predecessor.begin(),num_nodes,std::vector<int>());
        for(int i=0; i<num_nodes; i++){
            distance[i].insert(distance[i].begin(),num_nodes,-1);
            predecessor[i].insert(predecessor[i].begin(),num_nodes,-1);
        }
        distances_updated = false;
    }

    void add_edge(int a, int b, bool symmetric){
        bool found=false;

        if(a==b) return;

        for(unsigned int i=0; i<adjacent[a].size(); i++){
            if(adjacent[a][i]==b)
                found = true;
        }
        if(!found){
            adjacent[a].push_back(b);
            distances_updated = false;
        }
        if(symmetric){
            found=false;
            for(unsigned int i=0; i<adjacent[b].size(); i++){
                if(adjacent[b][i]==a)
                    found = true;
            }
            if(!found){
                adjacent[b].push_back(a);
                distances_updated = false;
            }
        }
    }

    void compute_distances(int source){

        MyHeap heap = MyHeap(num_nodes);
        std::vector<bool> visited;
        visited.insert(visited.begin(),num_nodes,false);

        for(int i=0; i<num_nodes; i++){
            if(i==source){
                predecessor[source][i] = i;
                distance[source][i] = 0;
                heap.scores[i] = 0;
                heap.vals[0] = i;
                heap.idxs[i] = 0;
            }else{
                predecessor[source][i] = -1;
                distance[source][i] = -1;
                heap.scores[i] = -1;
                heap.vals[i < source ? i+1 : i] = i;
                heap.idxs[i] = i < source ? i+1 : i;
            }
        }
        int done_count = 0;
        int same_score=1;
        while(done_count < num_nodes - 1){
            int min_n = heap.vals[0];
            visited[min_n] = true;
            int min_d = heap.scores[min_n];

            heap.heap_pop_min(false,same_score);

            for(unsigned int i=0; i<adjacent[min_n].size(); i++){

                int adj = adjacent[min_n][i];

                if(visited[adj]) continue;

                int curr_score = min_d + 1;

                if(IS_BETTER(curr_score,heap.scores[adj],false,same_score)){
                    heap.scores[adj] = curr_score;
                    distance[source][adj] = curr_score;
                    predecessor[source][adj] = min_n;

                    same_score=1;
                    heap.heapify_up(heap.idxs[adj],true,same_score);
                }
            }

            heap.idxs[min_n] = num_nodes-1;
            heap.vals[heap.size] = min_n;
            done_count++;
        }

    }

    int get_distance(int a, int b){
        if(!distances_updated){

            for(int i=0; i<num_nodes; i++)
                compute_distances(i);

            distances_updated = true;
        }

        return distance[a][b];
    }

    int get_predecessor(int a, int b){
        if(!distances_updated){

            for(int i=0; i<num_nodes; i++)
                compute_distances(i);

            distances_updated = true;
        }

        return predecessor[a][b];
    }

    inline bool is_connected(int a, int b){
        for(unsigned int i=0; i<adjacent[a].size(); i++){
            if(adjacent[a][i] == b) return true;
        }
        return false;
    }

    int get_nearest_free(int source, std::vector<int> &ptl){
        int min_dist = -1;
        int min_node = -1;
        int same_score = 1;
        for(int i=0; i<num_nodes; i++){
            if(IS_BETTER(get_distance(source,i),min_dist,true,same_score) && ptl[i]<0){
                min_dist = get_distance(source,i);
                min_node = i;
            }
        }
        return min_node;
    }

};

#endif