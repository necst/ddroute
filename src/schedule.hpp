#ifndef SCHEDULE
#define SCHEDULE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <queue>
#include <random>
#include <algorithm>
#include <unordered_map>

/**
 * Internal representation of quantum operations
 */
struct DDROperation{

    std::string name;
    int qubit;
    std::vector<double> gargs;
    std::vector<int> cargs;
    bool is2g;
    int oth;
    int oth_idx;
    int depth;
    int depth_2g;
    int first_operand;
    int gate_depth;
    int gate_id;

    DDROperation(std::string &name,int qubit,std::vector<double> &gargs,bool is2g, int gate_depth, int gate_id, std::vector<int>& cargs)
        :name(name),qubit(qubit),gargs(gargs),cargs(cargs),is2g(is2g),gate_depth(gate_depth),gate_id(gate_id){}
};

/**
 * Quantum circuit representation used in DDRoute
 */
struct DDRSchedule{
    int num_qubit;
    std::vector<std::vector<DDROperation>> schedule;
    std::vector<int> qubit_depth;
    std::vector<int> qubit_depth_2g;
    std::unordered_map<std::string,int> op_count;

    /**
     * DDRSchedule constructor
     * @param num_qubit: number of logical qubits
     */
    DDRSchedule(int num_qubit):num_qubit(num_qubit){
        schedule.insert(schedule.begin(),num_qubit,std::vector<DDROperation>());
        qubit_depth.insert(qubit_depth.begin(),num_qubit,0);
        qubit_depth_2g.insert(qubit_depth_2g.begin(),num_qubit,0);
    }

    /**
     * Insert a single-qubit quantum gate in the circuit
     * @param name: gate name
     * @param qubit: quantum gate operand
     * @param gargs: gate additional arguments
     * @param cargs: classical arguments
     * @param gate_depth: gate duration (default: 1)
     * @param gate_id: gate numerical id (optional)
     */
    void add_operation_1(std::string &name, int qubit, std::vector<double> &gargs, std::vector<int> &cargs, int gate_depth = 1, int gate_id = -1){
        if(qubit<0 || qubit>=num_qubit) return;
        DDROperation op = DDROperation(name,qubit,gargs,false,gate_depth,gate_id,cargs);
        op.depth = qubit_depth[qubit];
        op.depth_2g = qubit_depth_2g[qubit];
        qubit_depth[qubit]+=gate_depth;
        schedule[qubit].push_back(op);

        if(op_count.find(name)!=op_count.end()){
            op_count[name] = op_count[name] + 1;
        }else{
            op_count[name] = 1;
        }
    }

    /**
     * Insert a two-qubit quantum gate in the circuit
     * @param name: gate name
     * @param qubit1: quantum gate first operand
     * @param qubit2: quantum gate second operand
     * @param gargs: gate additional arguments
     * @param cargs: classical arguments
     * @param gate_depth: gate duration (default: 1)
     * @param gate_id: gate numerical id (optional)
     */
    void add_operation_2(std::string &name, int qubit1, int qubit2, std::vector<double> &gargs, std::vector<int> &cargs,  int gate_depth = 1, int gate_id = -1){
        if(qubit1<0 || qubit1>=num_qubit) return;
        if(qubit2<0 || qubit2>=num_qubit) return;
        DDROperation op1 = DDROperation(name,qubit1,gargs,true,gate_depth,gate_id,cargs);
        DDROperation op2 = DDROperation(name,qubit2,gargs,true,gate_depth,gate_id,cargs);

        op1.depth = MAX(qubit_depth[qubit1],qubit_depth[qubit2]);
        op1.depth_2g = MAX(qubit_depth_2g[qubit1],qubit_depth_2g[qubit2]);
        op1.oth = qubit2;
        op1.oth_idx = schedule[qubit2].size();

        op2.depth = MAX(qubit_depth[qubit1],qubit_depth[qubit2]);
        op2.depth_2g = MAX(qubit_depth_2g[qubit1],qubit_depth_2g[qubit2]);
        op2.oth = qubit1;
        op2.oth_idx = schedule[qubit1].size();

        op1.first_operand = qubit1;
        op2.first_operand = qubit1;

        qubit_depth[qubit1] = op1.depth + gate_depth;
        qubit_depth_2g[qubit1] = op1.depth_2g + gate_depth;
        qubit_depth[qubit2] = op2.depth + gate_depth;
        qubit_depth_2g[qubit2] = op2.depth_2g + gate_depth;

        schedule[qubit1].push_back(op1);
        schedule[qubit2].push_back(op2);

        if(op_count.find(name)!=op_count.end()){
            op_count[name] = op_count[name] + 1;
        }else{
            op_count[name] = 1;
        }
    }

    void pop_operation(int line){

        if(schedule[line].size()==0) return;

        if(op_count.find(schedule[line][schedule[line].size()-1].name)!=op_count.end()){
            op_count[schedule[line][schedule[line].size()-1].name] = op_count[schedule[line][schedule[line].size()-1].name] - 1;
        }

        qubit_depth[line] -= schedule[line][schedule[line].size()-1].gate_depth;

        if(schedule[line][schedule[line].size()-1].is2g){
            int oth = schedule[line][schedule[line].size()-1].oth;
            qubit_depth[oth] -= schedule[line][schedule[line].size()-1].gate_depth;
            qubit_depth_2g[oth] -= schedule[line][schedule[line].size()-1].gate_depth;
            qubit_depth_2g[line] -= schedule[line][schedule[line].size()-1].gate_depth;
            schedule[oth].pop_back();
        }

        schedule[line].pop_back();
    }

    /**
     * Quantum circuit depth
     */
    int depth(){
        return *std::max_element(qubit_depth.begin(),qubit_depth.end());
    }

    /**
     * Quantum circuit two-qubit-gates depth
     */
    int depth_2g(){
        return *std::max_element(qubit_depth_2g.begin(),qubit_depth_2g.end());
    }
    
    /**
     * Quantum circuit gate count
     */
    int gate_count(std::string &name){
        if(op_count.find(name)!=op_count.end()){
            return op_count[name];
        }
        return 0;
    }

};

/**
 * Schedule Reader, used to iterate over the operations of a DDRSchedule object
 */
struct DDRScheduleReader{

    DDRSchedule& s;
    std::vector<int> pcs;

    int last_read_x;
    int last_read_y;
    bool valid_coord;

    /**
     * DDRScheduleReader constructor
     * @param s: DDRSchedule to read
     */
    DDRScheduleReader(DDRSchedule &s):s(s){
        pcs.insert(pcs.begin(),s.num_qubit,0);
        valid_coord = false;
    }

    /**
     * Iterate over the next gate in the quantum circuit
     * @return True if the current gate is valid, False if the end of the circuit has been reached
     */
    bool next(){
        for(int i=0; i<s.num_qubit; i++){
            while(pcs[i] < (int)s.schedule[i].size() && !s.schedule[i][pcs[i]].name.compare("DOUBLE_swap"))
                pcs[i]++;
            if(pcs[i] < (int)s.schedule[i].size()){
                if(!s.schedule[i][pcs[i]].is2g){
                    last_read_x = i;
                    last_read_y = pcs[i];
                    pcs[i]++;
                    while(pcs[i] < (int)s.schedule[i].size() && !s.schedule[i][pcs[i]].name.compare("DOUBLE_swap"))
                        pcs[i]++;
                    valid_coord = true;
                    return true;
                }
                if(s.schedule[i][pcs[i]].oth_idx == pcs[s.schedule[i][pcs[i]].oth]){
                    last_read_x = i;
                    last_read_y = pcs[i];
                    pcs[s.schedule[i][pcs[i]].oth]++;
                    while(pcs[s.schedule[i][pcs[i]].oth] < (int)s.schedule[s.schedule[i][pcs[i]].oth].size() && !s.schedule[s.schedule[i][pcs[i]].oth][pcs[s.schedule[i][pcs[i]].oth]].name.compare("DOUBLE_swap"))
                        pcs[s.schedule[i][pcs[i]].oth]++;
                    pcs[i]++;
                    while(pcs[i] < (int)s.schedule[i].size() && !s.schedule[i][pcs[i]].name.compare("DOUBLE_swap"))
                        pcs[i]++;
                    valid_coord = true;
                    return true;
                }
            }
        }
        valid_coord = false;
        return false;
    }

    /**
     * Current gate name
     */
    std::string name(){
        if(!valid_coord) next();
        return s.schedule[last_read_x][last_read_y].name;
    }

    /**
     * Current gate quantum operands
     */
    std::vector<int> operands(){
        if(!valid_coord) next();
        std::vector<int> res;
        if(s.schedule[last_read_x][last_read_y].is2g){
            if(s.schedule[last_read_x][last_read_y].first_operand == s.schedule[last_read_x][last_read_y].qubit){
                res.push_back(s.schedule[last_read_x][last_read_y].qubit);
                res.push_back(s.schedule[last_read_x][last_read_y].oth);
                return res;
            }
            res.push_back(s.schedule[last_read_x][last_read_y].oth);
            res.push_back(s.schedule[last_read_x][last_read_y].qubit);
            return res;
        }
        res.push_back(s.schedule[last_read_x][last_read_y].qubit);
        return res;
    }

    /**
     * Current gate additional arguments
     */
    std::vector<double> gate_args(){
        if(!valid_coord) next();
        return s.schedule[last_read_x][last_read_y].gargs;
    }

    /**
     * Current gate classical arguments
     */
    std::vector<int> cargs(){
        if(!valid_coord) next();
        return s.schedule[last_read_x][last_read_y].cargs;
    }

    /**
     * Current gate id
     */
    int gate_id(){
        return s.schedule[last_read_x][last_read_y].gate_id;
    }

    /**
     * Quantum circuit depth
     */
    int depth(){
        return s.depth();
    }

    /**
     * Qubit depth
     * @param q: qubit
     */
    int qubit_depth(int q){
        return s.qubit_depth[q];
    }

    /**
     * Quantum circuit gate count
     * @param name: name of the quantum gate to count
     */
    int gate_count(std::string &name){
        return s.gate_count(name);
    }

};

#endif