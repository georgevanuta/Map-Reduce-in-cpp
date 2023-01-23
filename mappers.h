#ifndef MAPPERS_H
#define MAPPERS_H

#include "helpers.h"


/*
        ------ MAPPERS NAMESPACE ------
    Contains:
    - the mapperArg_t struct
    - different mapper functions:
        -> one for perfect numbers
        -> TODO (BONUS)

*/
namespace mappers {
    
    /*      
            Template mapper struct in order 
        to achieve a general Map-Reduce.
    */
    template <typename T> struct mapperArg_t {
        // mapper id
        unsigned int mId;

        // all the input file paths
        std::vector<std::string> filePaths; 

        /*
                Barrier shared between
            all threads (mappers and reducers).
        */
        pthread_barrier_t *reducerBarrier;

        /*
                A mapper needs to know the
            number of reducers in order to
            know the last power the perfect
            number need to be.
        */
        unsigned int nrReducers;

        // this is where the general mapper is
        T *result;

        /*
                A boolean vector to allow
            dynamic splitting of the files
            to mappers. If permissionToFiles[i]
            is files, a mapper is doing / has done
            its processing on file[i].
        */
        std::vector<bool> *permissionToFiles;

        /*
                A lock to not let two threads access
            the permissionToFiles at the same time.
        */
        pthread_mutex_t *permissionLock;



        // constructor
        mapperArg_t(unsigned int id = 0, 
                    std::vector<std::string> files = {},
                    pthread_barrier_t *barrier = NULL, 
                    unsigned int nrR = 0, 
                    T* res = NULL,
                    std::vector<bool> *perms = NULL,
                    pthread_mutex_t *lock = NULL) :
            mId(id),
            filePaths(files), 
            reducerBarrier(barrier), 
            nrReducers(nrR), 
            result(res), 
            permissionToFiles(perms),
            permissionLock(lock) {}
    };


    /*
            Mapper function used for
        searching for perfect numbers in
        a file.
    */
    void *mapperPerfectNumber(void *arg);
}


#endif