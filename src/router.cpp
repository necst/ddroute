#ifndef ROUTER
#define ROUTER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <queue>
#include <random>

#include "utils.hpp"
#include "schedule.hpp"
#include "topology.hpp"

#include <pybind11/pybind11.h>

namespace py = pybind11;

/**
 * DDRouter instance
 */
struct DDRouter{

    int num_lq;
    int num_pq;

    DDRSchedule& c;
    DDRTopology& t;
    DDRSchedule pc;

    // Logical-to-physical map
    std::vector<int> ltp_map;
    std::vector<int> ptl_map;

    double swap_depth;

    /**
     * DDRouter constructor
     * @param c: DDRSchedule of the logical circuit
     * @param t: DDRTopology of the targeted quantum processor
     */
    DDRouter(DDRSchedule& c, DDRTopology& t):c(c),t(t),pc(t.num_nodes){
        num_lq = c.num_qubit;
        num_pq = t.num_nodes;

        if(num_lq > num_pq){
            throw py::value_error("Cannot map " + std::to_string(num_lq) + " logical qubits onto " + std::to_string(num_pq) + " physical qubits");
        }

        ltp_map.insert(ltp_map.begin(),num_lq,-1);
        ptl_map.insert(ptl_map.begin(),num_pq,-1);
        swap_depth = 3;
    }

    /**
     * Set SWAP gate duration
     * @param value: SWAP duration
     */
    void set_swap_depth(double value){
        if(value < 0){
            throw py::value_error("Invalid SWAP duration: " + std::to_string(value) + ", required duration >= 0 ");
        }

        swap_depth = value;
    }

    /**
     * Set initial layout
     * @param ltp: logical to physical map (i.e. ltp[0] is the physical qubit where the logical qubit 0 is mapped)
     */
    void set_initial_layout(std::vector<int> &ltp){
        for(int i=0; i<num_lq; i++){
            if(ltp[i] < 0 || ltp[i] >= t.num_nodes){
                throw py::value_error("Invalid logical-to-physical mapping: found value " + std::to_string(ltp[i]) + ", required 0 <= pi(q) < " + std::to_string(t.num_nodes));
            }
            ltp_map[i] = ltp[i];
        }
        for(int i=0; i<num_pq; i++){
            ptl_map[i] = -1;
        }
        for(int i=0; i<num_lq; i++){
            ptl_map[ltp_map[i]] = i;
        }
    }

    /**
     * Compute the front layer of the DAG considering two-qubit gates only
     */
    void compute_front_layer_2g(std::vector<int> &pcs, std::vector<int> &fl_pcs, std::vector<bool> &front_layer){
        for(int i=0; i<num_lq; i++){
            fl_pcs[i] = pcs[i];
            while(fl_pcs[i] < (int)c.schedule[i].size() && !c.schedule[i][fl_pcs[i]].is2g)
                fl_pcs[i]++;
            if(fl_pcs[i] < (int)c.schedule[i].size()){
                front_layer[i] = true;
            }else front_layer[i] = false;
        }
        for(int i=0; i<num_lq; i++){
            if(front_layer[i] && front_layer[c.schedule[i][fl_pcs[i]].oth] && 
                c.schedule[i][fl_pcs[i]].oth_idx == fl_pcs[c.schedule[i][fl_pcs[i]].oth]){
                
                front_layer[i] = true;
            }else front_layer[i] = false;
        }
    }

    /**
     * Set initial layout using DDPlace
     */
    std::vector<int> depth_driven_place(){
        std::vector<bool> front_layer;
        std::vector<int> pcs;
        std::vector<int> fl_pcs;

        for(int i=0; i<num_lq; i++){
            ltp_map[i] = -1;
        }
        for(int i=0; i<num_pq; i++){
            ptl_map[i] = -1;
        }

        front_layer.insert(front_layer.begin(),num_lq,false);
        pcs.insert(pcs.begin(),num_lq,0);
        fl_pcs.insert(fl_pcs.begin(),num_lq,0);

        int last_placed = std::rand()%num_pq;

        int mapped_qubits = 0;

        while(mapped_qubits < num_lq){
            compute_front_layer_2g(pcs,fl_pcs,front_layer);
            double min_depth = -1;
            int min_op = -1;
            int min_nt = -1;
            int same_score = 1;

            for(int i=0; i<num_lq; i++){
                if(front_layer[i]){
                    if(IS_BETTER(c.schedule[i][fl_pcs[i]].depth_2g,min_depth,true,same_score)){
                        min_depth = c.schedule[i][fl_pcs[i]].depth_2g;
                        min_op = i;
                        min_nt = c.schedule[i][fl_pcs[i]].oth;
                    }
                }
            }
            if(min_op<0){
                for(int i=0; i<num_lq; i++){
                    if(ltp_map[i] < 0){
                        min_op = i;
                        break;
                    }
                }
            }else{
                pcs[min_op] = fl_pcs[min_op]+1;
                pcs[min_nt] = fl_pcs[min_nt]+1;
            }

            int target;
            if(ltp_map[min_op] < 0){
                if(min_nt >= 0 && ltp_map[min_nt]>=0){
                    target = t.get_nearest_free(ltp_map[min_nt],ptl_map);
                }else{
                    target = t.get_nearest_free(last_placed,ptl_map);
                }
                ltp_map[min_op] = target;
                ptl_map[target] = min_op;
                last_placed = target;
                mapped_qubits++;
            }

            if(min_nt >= 0 && ltp_map[min_nt]<0){
                target = t.get_nearest_free(ltp_map[min_op],ptl_map);
                ltp_map[min_nt] = target;
                ptl_map[target] = min_nt;
                last_placed = target;
                mapped_qubits++;
            }

        }

        std::vector<int> result = std::vector<int>();
        for(int lq = 0; lq<num_lq; lq++){
            result.push_back(ltp_map[lq]);
        }
        
        return result;
    }

    /**
     * Heuristic for selecting gates to compute the lookahead score
     */
    double gate_scheduling_score(int q, int pc){
        return c.schedule[q][pc].depth_2g;
    }

    /**
     * Select the gates used to compute the lookahead score
     */
    void prepare_front_layer_pref(std::vector<int> &fl_pcs_c, std::vector<bool> &front_layer_c, front_layer_prefs &flp, std::vector<int> &ltp_map){
        int done = 0;
        flp.size = 0;
        int gates_done = 0;

        int op_best = 0;
        double curr_score = -1;
        double op_best_score = -1;
        int same_score = 1;
        double coeff = 1.0;

        while(!done){
            op_best_score = -1;
            same_score = 1;
            compute_front_layer_2g(fl_pcs_c,fl_pcs_c,front_layer_c);

            if(is_empty_fl(front_layer_c)){
                done = 1;
                break;
            }
            for(int i=0; i<num_lq; i++){
                if(front_layer_c[i]){
                    curr_score = gate_scheduling_score(i,fl_pcs_c[i]);
                    if(IS_BETTER(curr_score,op_best_score,true,same_score)){
                        op_best_score = curr_score;
                        op_best = i;
                    }
                }
            }
            flp.op_1[flp.size] = ltp_map[op_best];
            flp.op_2[flp.size] = ltp_map[c.schedule[op_best][fl_pcs_c[op_best]].oth];
            flp.coeff[flp.size] = coeff;
            flp.size++;

            flp.op_2[flp.size] = ltp_map[c.schedule[op_best][fl_pcs_c[op_best]].oth];
            flp.op_1[flp.size] = ltp_map[op_best];
            flp.coeff[flp.size] = coeff;
            flp.size++;
            
            coeff *= OPERANDS_LOOKAHEAD_DECAY;
            gates_done++;
            fl_pcs_c[c.schedule[op_best][fl_pcs_c[op_best]].oth]++;
            fl_pcs_c[op_best]++;
            if(gates_done==OPERANDS_LOOKAHEAD) done = 1;
        }
    }

    /**
     * Generalized distance function
     */
    inline double path_score(double depth, int nswap, double la_score){
        return depth + nswap*(SCORE_SWAP(swap_depth)) + la_score*(SCORE_LA(swap_depth));
    }

    /**
     * Compute the optimal path between source and dest, using the generalized distance function as distance metric
     * Return the optimal path as an array of predecessors
     */
    void schedule_dijkstra(int source, int dest, front_layer_prefs &flp, std::vector<int> &path){
        MyTripleHeap heap = MyTripleHeap(num_pq);

        std::vector<bool> visited;
        visited.insert(visited.begin(),num_pq,false);

        // Initialize heap
        for(int i=0; i<num_pq; i++){
            if(i==source){
                path[i] = i;
                heap.depth[i] = pc.qubit_depth[i];
                heap.swap[i] = 0;
                heap.la_score[i] = 0;
                heap.scores[i] = path_score(pc.qubit_depth[i],0,0.0);
                heap.vals[0] = i;
                heap.idxs[i] = 0;
            }else{
                path[i] = -1;
                heap.depth[i] = -1;
                heap.scores[i] = -1;
                heap.vals[i < source ? i+1 : i] = i;
                heap.idxs[i] = i < source ? i+1 : i;
            }
        }

        int done_count = 0;
        int min_q;
        double min_d;
        int min_dist;
        double min_la;
        int adj, same_score = 1;

        while(done_count<num_pq-1){
            min_q = heap.vals[0];

            visited[min_q] = true;

            min_d = heap.depth[min_q];
            min_dist = heap.swap[min_q];
            min_la = heap.la_score[min_q];

            if(min_q == dest) return;

            same_score=1;
            heap.heap_pop_min(false,same_score);

            for(unsigned int i=0; i<t.adjacent[min_q].size(); i++){
                adj = t.adjacent[min_q][i];
                
                if(visited[adj]) continue;

                same_score = 1;
                double curr_depth = MAX(min_d,pc.qubit_depth[adj]) + swap_depth;
                int curr_std_dist = min_dist + 1;
                double curr_la = min_la;

                // Compute new lookahead score
                for(int i=0; i< flp.size; i++){
                    if((flp.op_1[i] == adj)  && flp.op_2[i] != source){
                        if(t.get_distance(min_q,flp.op_2[i])         < t.get_distance(adj,flp.op_2[i])){
                            curr_la += flp.coeff[i];
                        }else if(t.get_distance(min_q,flp.op_2[i])   > t.get_distance(adj,flp.op_2[i])){
                            curr_la += flp.coeff[i];
                        }
                    }
                    else if(flp.op_1[i] == source  && flp.op_2[i] !=adj && flp.op_2[i] != dest){
                        if(t.get_distance(min_q,flp.op_2[i])        > t.get_distance(adj,flp.op_2[i])){
                            curr_la += flp.coeff[i];
                        }else if(t.get_distance(min_q,flp.op_2[i])   < t.get_distance(adj,flp.op_2[i])){
                            curr_la += flp.coeff[i];
                        }
                    }
                }

                double curr_score = path_score(curr_depth,curr_std_dist,curr_la);

                if(IS_BETTER(curr_score,heap.scores[adj],false,same_score)){
                    heap.scores[adj] = curr_score;
                    heap.depth[adj] = curr_depth;
                    heap.swap[adj] = curr_std_dist;
                    heap.la_score[adj] = curr_la;
                    path[adj] = min_q;

                    same_score = 1;
                    heap.heapify_up(heap.idxs[adj],true,same_score);
                }
            }

            heap.idxs[min_q] = heap.size;
            heap.vals[heap.size] = min_q;
            done_count++;
        }

    }

    /**
     * Insert a SWAP between physical qubits in the physical circuit,
     * modify the logical-to-physical and physical-to-logical mappings accordingly
     */
    void perform_SWAP(int q1, int q2, std::vector<int> &pcs){

        std::string newname = std::string("swap");
        double depth_to_insert = swap_depth;

        if(
            pc.schedule[q1].size()>0 &&
            (
                !pc.schedule[q1][pc.schedule[q1].size()-1].name.compare("swap")
            ) &&
            pc.schedule[q1][pc.schedule[q1].size()-1].oth == q2 &&
            pc.schedule[q2].size()>0 &&
            (
                !pc.schedule[q2][pc.schedule[q2].size()-1].name.compare("swap")
            ) &&
            pc.schedule[q2][pc.schedule[q2].size()-1].oth == q1
            
            ){
            pc.schedule[q1][pc.schedule[q1].size()-1].name = std::string("DOUBLE_swap");
            pc.schedule[q2][pc.schedule[q2].size()-1].name = std::string("DOUBLE_swap");
            pc.schedule[q1][pc.schedule[q1].size()-1].gate_depth = 0;
            pc.schedule[q2][pc.schedule[q2].size()-1].gate_depth = 0;

            newname = std::string("DOUBLE_swap");
            depth_to_insert = 0;

            pc.qubit_depth[q1] -= swap_depth;
            pc.qubit_depth[q2] -= swap_depth;

            pc.op_count["swap"] = pc.op_count["swap"] - 1;
            if(pc.gate_count(newname)==0){
                pc.op_count["DOUBLE_swap"] = 1;
            }else{
                pc.op_count["DOUBLE_swap"] = pc.op_count["DOUBLE_swap"] + 1;
            }
        }

        if(ptl_map[q1]>=0){
            while(pcs[ptl_map[q1]] < (int)c.schedule[ptl_map[q1]].size()
                && !c.schedule[ptl_map[q1]][pcs[ptl_map[q1]]].is2g
                && pc.qubit_depth[q1] + c.schedule[ptl_map[q1]][pcs[ptl_map[q1]]].gate_depth <= pc.qubit_depth[q2]){
                
                pc.add_operation_1(c.schedule[ptl_map[q1]][pcs[ptl_map[q1]]].name,q1,c.schedule[ptl_map[q1]][pcs[ptl_map[q1]]].gargs,c.schedule[ptl_map[q1]][pcs[ptl_map[q1]]].cargs,c.schedule[ptl_map[q1]][pcs[ptl_map[q1]]].gate_depth,c.schedule[ptl_map[q1]][pcs[ptl_map[q1]]].gate_id);

                pcs[ptl_map[q1]]++;
            }
        }
        if(ptl_map[q2]>=0){
            while(pcs[ptl_map[q2]] < (int)c.schedule[ptl_map[q2]].size()
                && !c.schedule[ptl_map[q2]][pcs[ptl_map[q2]]].is2g
                && pc.qubit_depth[q2] + c.schedule[ptl_map[q2]][pcs[ptl_map[q2]]].gate_depth <= pc.qubit_depth[q1]){
                
                pc.add_operation_1(c.schedule[ptl_map[q2]][pcs[ptl_map[q2]]].name,q2,c.schedule[ptl_map[q2]][pcs[ptl_map[q2]]].gargs,c.schedule[ptl_map[q2]][pcs[ptl_map[q2]]].cargs,c.schedule[ptl_map[q2]][pcs[ptl_map[q2]]].gate_depth,c.schedule[ptl_map[q2]][pcs[ptl_map[q2]]].gate_id);

                pcs[ptl_map[q2]]++;
            }
        }

        // Insert SWAP operation
        std::vector<double> g_args = std::vector<double>();
        std::vector<int> c_args = std::vector<int>();
        pc.add_operation_2(newname,MIN(q1,q2),MAX(q1,q2),g_args,c_args,depth_to_insert);

        // Update maps
        int l1 = ptl_map[q1];
        int l2 = ptl_map[q2];
        ptl_map[q1] = l2;
        ptl_map[q2] = l1;
        if(l1>=0) ltp_map[l1] = q2;
        if(l2>=0) ltp_map[l2] = q1;
    }

    /**
     * Run DDRoute on the logical quantum circuit
     * @return A DDRScheduleReader instance of the routed quantum circuit
     */
    DDRScheduleReader depth_driven_routing(){

        // Front layer: array of bools, with positive entries corresponding to qubits in the front layer of the DAG
        std::vector<bool> front_layer;

        // Pointers to the current logical operation to be inserted in the physical circuit
        std::vector<int> pcs;

        // Pointers to the logical operation for qubits in the front layer
        std::vector<int> fl_pcs;

        // Structure containing the gates used for computing the lookahead score
        front_layer_prefs flp;

        // Front layer and pointer set used for computing the lookahead score
        std::vector<bool> front_layer_c;
        std::vector<int> fl_pcs_c;

        front_layer.insert(front_layer.begin(),num_lq,false);
        pcs.insert(pcs.begin(),num_lq,0);
        fl_pcs.insert(fl_pcs.begin(),num_lq,0);
        front_layer_c.insert(front_layer_c.begin(),num_lq,false);
        fl_pcs_c.insert(fl_pcs_c.begin(),num_lq,0);

        // For optimization purposes, optimal paths are cached and reused when possible
        std::vector<int> valid_cached_path = std::vector<int>(num_lq);
        std::vector<int> nonce_value = std::vector<int>(num_lq);
        std::vector<int> excluded = std::vector<int>(num_pq);
        std::vector<std::vector<int>> included_in_path = std::vector<std::vector<int>>();
        std::vector<std::vector<int>> sd_p = std::vector<std::vector<int>>();
        int global_nonce = 1;

        for(int i=0; i<num_lq; i++){
            valid_cached_path[i] = 0;
            nonce_value[i] = 0;
            included_in_path.push_back(std::vector<int>(num_pq));
            sd_p.push_back(std::vector<int>(num_pq));
            for(int j=0; j<num_pq; j++){
                included_in_path[i][j] = 0;
                sd_p[i][j] = -1;
            }
        }
        for(int i=0; i<num_pq; i++){
            excluded[i] = 0;
        }

        // Compute front layer
        compute_front_layer_2g(pcs,fl_pcs,front_layer);

        // Iterate until the front layer is empty
        while(!is_empty_fl(front_layer)){

            int executed = 0;

            // Check if there are two-qubit gates that can be executed
            for(int q=0; q<num_lq; q++){
                if(front_layer[q]){
                    int oth_q = c.schedule[q][fl_pcs[q]].oth;
                    if(oth_q <= q && t.is_connected(ltp_map[q],ltp_map[oth_q])){

                        // Execute single-qubit gates first
                        while(pcs[q] < fl_pcs[q]){
                            pc.add_operation_1(c.schedule[q][pcs[q]].name,ltp_map[q],c.schedule[q][pcs[q]].gargs,c.schedule[q][pcs[q]].cargs,c.schedule[q][pcs[q]].gate_depth,c.schedule[q][pcs[q]].gate_id);
                            pcs[q]++;
                        }
                        while(pcs[oth_q] < fl_pcs[oth_q]){
                            pc.add_operation_1(c.schedule[oth_q][pcs[oth_q]].name,ltp_map[oth_q],c.schedule[oth_q][pcs[oth_q]].gargs,c.schedule[oth_q][pcs[oth_q]].cargs,c.schedule[oth_q][pcs[oth_q]].gate_depth,c.schedule[oth_q][pcs[oth_q]].gate_id);
                            pcs[oth_q]++;
                        }

                        // Execute two-qubit gate
                        if(c.schedule[q][fl_pcs[q]].first_operand == q){
                            pc.add_operation_2(c.schedule[q][fl_pcs[q]].name,ltp_map[q],ltp_map[oth_q],c.schedule[q][fl_pcs[q]].gargs,c.schedule[q][fl_pcs[q]].cargs,c.schedule[q][fl_pcs[q]].gate_depth,c.schedule[q][fl_pcs[q]].gate_id);
                        }else{
                            pc.add_operation_2(c.schedule[q][fl_pcs[q]].name,ltp_map[oth_q],ltp_map[q],c.schedule[q][fl_pcs[q]].gargs,c.schedule[q][fl_pcs[q]].cargs,c.schedule[q][fl_pcs[q]].gate_depth,c.schedule[q][fl_pcs[q]].gate_id);
                        }
                        pcs[q]++;
                        pcs[oth_q]++;
                        executed++;
                    }
                }
            }

            // Check if any two-qubit gate has been inserted in the physical circuit
            if(executed > 0){
                
                // Update front layer
                compute_front_layer_2g(pcs,fl_pcs,front_layer);

                // Invalidate cached paths
                for(int lq=0; lq<num_lq; lq++)
                    valid_cached_path[lq] = 0;
            }else{

                bool swap_found = false;
                for(int pq=0; pq<num_pq; pq++)
                    excluded[pq] = 0;
                int force = 0;

                while(!swap_found){
                    int min_depth_qubit = -1;
                    double min_depth = -1;
                    int same_score = 1;

                    // Operand selection
                    for(int i=0; i<num_lq; i++){
                        if(front_layer[i] && !excluded[ltp_map[i]]){
                            double qscore = pc.qubit_depth[t.adjacent[ltp_map[i]][0]];
                            for(unsigned int j=1; j<t.adjacent[ltp_map[i]].size(); j++){
                                qscore = MIN(qscore,pc.qubit_depth[t.adjacent[ltp_map[i]][j]]);
                            }
                            qscore = MAX(qscore,pc.qubit_depth[ltp_map[i]]);
                            if(IS_BETTER(qscore,min_depth,true,same_score)){
                                min_depth = qscore;
                                min_depth_qubit = i;
                            }
                        }
                    }

                    if(min_depth_qubit>=0)
                        force = 0;
                    while(min_depth_qubit<0){

                        // Force qubit selection
                        same_score = 1;
                        for(int i=0; i<num_lq; i++){
                            if(front_layer[i] && excluded[i]<=force){
                                if(IS_BETTER(0,(min_depth_qubit==0),true,same_score))
                                    min_depth_qubit = i;
                            }
                        }
                        if(min_depth_qubit<0 || force==0) force++;
                    }

                    int succ=-1;
                    while(succ<0){
                        if(!valid_cached_path[min_depth_qubit]){

                            // Compute optimal paths
                            prepare_front_layer_pref(fl_pcs_c,front_layer_c,flp,ltp_map);
                            schedule_dijkstra(ltp_map[min_depth_qubit],ltp_map[c.schedule[min_depth_qubit][fl_pcs[min_depth_qubit]].oth],flp,sd_p[min_depth_qubit]);
                            
                            // Setup cache
                            nonce_value[min_depth_qubit] = global_nonce;
                            global_nonce++;

                            succ = ltp_map[c.schedule[min_depth_qubit][fl_pcs[min_depth_qubit]].oth];

                            included_in_path[min_depth_qubit][succ] = nonce_value[min_depth_qubit];
                            while(sd_p[min_depth_qubit][succ]!=ltp_map[min_depth_qubit]){
                                succ = sd_p[min_depth_qubit][succ];
                                included_in_path[min_depth_qubit][succ] = nonce_value[min_depth_qubit];
                            }
                            included_in_path[min_depth_qubit][ltp_map[min_depth_qubit]] = nonce_value[min_depth_qubit];
                        }
                        
                        // Select SWAP
                        succ = ltp_map[c.schedule[min_depth_qubit][fl_pcs[min_depth_qubit]].oth];
                        while(succ>=0 && sd_p[min_depth_qubit][succ]!=ltp_map[min_depth_qubit] && sd_p[min_depth_qubit][succ]!=succ){
                            succ = sd_p[min_depth_qubit][succ];
                        }
                        if(succ<0 || sd_p[min_depth_qubit][succ]!=ltp_map[min_depth_qubit]){
                            valid_cached_path[min_depth_qubit] = 0;
                            succ = -1;
                        }
                    }

                    // Check for validity of selected SWAP operation
                    if(((
                        (   
                            pc.schedule[ltp_map[min_depth_qubit]].size() == 0 ||
                            (
                                pc.schedule[ltp_map[min_depth_qubit]][pc.schedule[ltp_map[min_depth_qubit]].size()-1].name.compare("swap") &&
                                pc.schedule[ltp_map[min_depth_qubit]][pc.schedule[ltp_map[min_depth_qubit]].size()-1].name.compare("DOUBLE_swap")
                            )
                            || pc.schedule[ltp_map[min_depth_qubit]][pc.schedule[ltp_map[min_depth_qubit]].size()-1].oth != succ ||
                            pc.schedule[succ].size()==0 ||
                            (
                                pc.schedule[succ][pc.schedule[succ].size()-1].name.compare("swap") &&
                                pc.schedule[succ][pc.schedule[succ].size()-1].name.compare("DOUBLE_swap")
                            )
                            || pc.schedule[succ][pc.schedule[succ].size()-1].oth != ltp_map[min_depth_qubit]
                            || pc.schedule[succ][pc.schedule[succ].size()-1].name.compare(pc.schedule[ltp_map[min_depth_qubit]][pc.schedule[ltp_map[min_depth_qubit]].size()-1].name)
                        )
                        && 
                        (
                            ptl_map[succ]<0 || (!front_layer[ptl_map[succ]]) ||
                            IS_BETTER(
                                gate_scheduling_score(min_depth_qubit,fl_pcs[min_depth_qubit]),
                                gate_scheduling_score(ptl_map[succ],fl_pcs[ptl_map[succ]]),
                                false,same_score
                            )
                            || t.get_distance(ltp_map[min_depth_qubit],ltp_map[c.schedule[ptl_map[succ]][fl_pcs[ptl_map[succ]]].oth]) <= 
                            t.get_distance(succ,ltp_map[c.schedule[ptl_map[succ]][fl_pcs[ptl_map[succ]]].oth])
                        ))
                         || force
                        ) && (ltp_map[min_depth_qubit]!=succ)
                    ){

                        // Store path in cache
                        if(ptl_map[succ]>=0)
                            valid_cached_path[ptl_map[succ]] = 0;
                        for(int i=0; i<num_lq; i++){

                            if(included_in_path[i][ltp_map[min_depth_qubit]] == nonce_value[i]){
                                valid_cached_path[i] = 0;
                            }
                            if(included_in_path[i][succ] == nonce_value[i]){
                                valid_cached_path[i] = 0;
                            }
                        }
                        included_in_path[min_depth_qubit][ltp_map[min_depth_qubit]] = 0;

                        // Perform SWAP operation
                        perform_SWAP(ltp_map[min_depth_qubit],succ,pcs);
                        swap_found = true;
                        valid_cached_path[min_depth_qubit] = 1;
                    }else{
                        excluded[ltp_map[min_depth_qubit]] ++;
                    }
                }
            }
        }

        // Inserted missing single-qubit gates at the end of the circuit
        for(int q=0; q<num_lq; q++){
            while(pcs[q] < (int)c.schedule[q].size()){
                pc.add_operation_1(c.schedule[q][pcs[q]].name,ltp_map[q],c.schedule[q][pcs[q]].gargs,c.schedule[q][pcs[q]].cargs,c.schedule[q][pcs[q]].gate_depth,c.schedule[q][pcs[q]].gate_id);
                pcs[q]++;
            }
        }

        // Return a DDRScheduleReader of the physical circuit
        return DDRScheduleReader(pc);

    }

};

#endif