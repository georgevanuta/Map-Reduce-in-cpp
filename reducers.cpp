#include "reducers.h"
#include <pthread.h>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>


void *reducers::reducerPerfectNumber(void *arg) {
        reducers::reducerArg_t<std::map<int, std::map<int, std::vector<long long>>>> reducerArg =
            *(reducers::reducerArg_t<std::map<int, std::map<int, std::vector<long long>>>> *)arg;
        /*
                Wait on barrier before doing any reducing
            in order to let the mappers end their processing.
        */
        pthread_barrier_wait(reducerArg.reducerBarrier);

        std::map<int, std::map<int, std::vector<long long>>> mapRes =
            *(reducerArg.fromMappers);
        
        // reduce
        std::set<long long> individualPerfects;
        for (const auto &x : mapRes) {
            if (x.second.find(reducerArg.rId + 2) == x.second.end()) continue;
            for (long el : x.second.at(reducerArg.rId + 2)) {
                individualPerfects.insert(el);
            }
        }
        
        // write size of perfect numbers to output file
        std::ofstream reducerOutputFile("out" + std::to_string(reducerArg.rId + 2) + ".txt");
        reducerOutputFile << individualPerfects.size();

        pthread_exit(NULL);
    }