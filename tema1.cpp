#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <map>
#include <set>

#include "helpers.h"
#include "reducers.h"
#include "mappers.h"


int main(const int argc, const char *argv[])
{   
    /*
        Only accept clean input.
    */
    helpers::exitIfInvalidArgs(argc, argv);
    
    /*
        Input is clean, accept it.
    */
    const unsigned int NR_MAPPERS = std::stoi(argv[1]);
    const unsigned int NR_REDUCERS = std::stoi(argv[2]);
    const unsigned int NR_THREADS = NR_MAPPERS + NR_REDUCERS;

    /*
        Get the mapper file paths.
    */
    const std::string TEST_PATH = argv[3];
    std::vector<std::string> inputFilePaths = helpers::getFilePaths(TEST_PATH);
    
    /*
        Initialize threads and the barrier.
    */
    pthread_t mappers[NR_MAPPERS];
    mappers::mapperArg_t<std::map<int, std::map<int, std::vector<long long>>>> mapperArgs[NR_MAPPERS];
    
    pthread_t reducers[NR_REDUCERS];
    reducers::reducerArg_t<std::map<int, std::map<int, std::vector<long long>>>> reducerArgs[NR_REDUCERS];

    pthread_barrier_t reducersBarrier;
    pthread_barrier_init(&reducersBarrier, NULL, NR_MAPPERS + NR_REDUCERS);
    
    /*
        Used for testing the output of functions.
    */
    int r;

    /*
            This is where each mapper will
        hold its result from the processing of the files.
            The structure used is a map of multimaps
        with keys of type int (the power) and values of type
        int (the perfect numbers of the given power).
            All threads will share this multimap.
    */
    std::map<int, std::map<int, std::vector<long long>>> mappersRes;

    /*
            Initialize the 
        permission files vector.
    */
    std::vector<bool> permissionFiles(inputFilePaths.size(), true);

    /*
            Initialize the
        permission lock. 
    */
    pthread_mutex_t permissionLock;
    r = pthread_mutex_init(&permissionLock, NULL);
    helpers::exitIf(r, "[ERROR]: Couldn't initialize the permission lock.");


    /*
        Create the mappers and reducers in one go.
    */
    for (unsigned int id = 0; id < NR_MAPPERS + NR_REDUCERS; id++) {
        if (id < NR_MAPPERS) {
            mappers::mapperArg_t<std::map<int, std::map<int, std::vector<long long>>>> mapperArg(
                id,
                inputFilePaths, 
                &reducersBarrier, 
                NR_REDUCERS,
                &mappersRes,
                &permissionFiles,
                &permissionLock);
            mapperArgs[id] = mapperArg;

            r = pthread_create(&mappers[id], NULL, mappers::mapperPerfectNumber, &mapperArgs[id]);
            helpers::exitIf(r, "[ERROR]: Couldn't create a mapper thread with id = " + std::to_string(id) + ".");
        } else {
            unsigned int actualId = id - NR_MAPPERS;
            reducers::reducerArg_t<std::map<int, std::map<int, std::vector<long long>>>> reducerArg(
                actualId,
                &reducersBarrier,
                &mappersRes);
            reducerArgs[actualId] = reducerArg;

            r = pthread_create(&reducers[actualId], NULL, reducers::reducerPerfectNumber, &reducerArgs[actualId]);
            helpers::exitIf(r, "[ERROR]: Couldn't create a reducer thread with id = " + std::to_string(actualId) + ".");
        }
    }


    /*
            One final join on all threads so there's no zombie threads.
        Also used in order to not destroy the barrier before all threads are
        finished with it.
    */
    for (unsigned int id = 0; id < NR_THREADS; id++) {
        if (id < NR_MAPPERS) {

            r = pthread_join(mappers[id], NULL);
            helpers::exitIf(r, "[ERROR]: Couldn't join a mapper thread with id = " + std::to_string(id) + ".");
        } else {
            unsigned int actualId = id - NR_MAPPERS;
            r = pthread_join(reducers[actualId], NULL);
            helpers::exitIf(r, "[ERROR]: Couldn't join a reducer thread with id = " + std::to_string(actualId) + ".");
        }

    }

    /*
        At the end, destroy the used structures.
    */
    pthread_barrier_destroy(&reducersBarrier);
    pthread_mutex_destroy(&permissionLock);

    return 0;
}
