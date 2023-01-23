#ifndef REDUCERS_H
#define REDUCERS_H

#include <pthread.h>


/*
        ------ REDUCERS NAMESPACE ------
    Contains:
    - the reducerArg_t struct
    - different reducer functions:
        -> one for perfect numbers
        -> TODO (BONUS)
*/
namespace reducers {
    
    /*      
            Template reducer struct in order 
        to achieve a general Map-Reduce.
    */
    template <typename T> struct reducerArg_t {
        // reducer id
        unsigned int rId;

        /*
                Barrier shared between
            all threads (mappers and reducers).
        */
        pthread_barrier_t *reducerBarrier;

        T *fromMappers;
        // constructor
        reducerArg_t(unsigned int id = 0, pthread_barrier_t *barrier = NULL, T *fromM = NULL) :
        rId(id), reducerBarrier(barrier), fromMappers(fromM) {}
    };

    /*
            Specific reducer thread function used for
        collecting the perfect numbers of the respective thread
        after the mappers parsed the input files.
    */
    void *reducerPerfectNumber(void *arg);
}


#endif