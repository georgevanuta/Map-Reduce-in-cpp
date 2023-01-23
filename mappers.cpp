#include "mappers.h"

#include <map>
#include <vector>
#include <fstream>
#include <pthread.h>


void *mappers::mapperPerfectNumber(void *arg) {
        mappers::mapperArg_t<std::map<int, std::map<int, std::vector<long long>>>> mapperArg = 
            *(mappers::mapperArg_t<std::map<int, std::map<int, std::vector<long long>>>> *)arg;
        
        std::map<int, std::vector<long long>> mapperEntry;

        for (long unsigned int i = 0; i < mapperArg.filePaths.size(); i++) {
            /* 
                    Only one thread can see the
                permission of a file at a given time.
            */
            pthread_mutex_lock(mapperArg.permissionLock);
            
            /*
                    If the file is not operated on / 
                has not been operated on, this thread
                will operate on it.
            */
            if (mapperArg.permissionToFiles->at(i)) {
                /*
                        Set permission to false, so that
                    no other thread tries accessing this file.
                */
                mapperArg.permissionToFiles->at(i) = false;
                pthread_mutex_unlock(mapperArg.permissionLock);
                
                /*
                        Now, the thread starts collecting
                    all of the perfect numbers from the file.
                */
                std::ifstream inputFile(mapperArg.filePaths[i]);
                std::string lineStr;

                getline(inputFile, lineStr);
                helpers::exitIf(!helpers::isPosInt(lineStr), "[ERROR]: first line must be a positive int.");

                const unsigned int nr = std::stoi(lineStr);
                unsigned long value;
                for (unsigned int lineNr = 0; lineNr < nr; lineNr++) {
                    getline(inputFile, lineStr);
                    value = std::stol(lineStr);
                    for (unsigned int pwr = 2; pwr < mapperArg.nrReducers + 2; pwr++) {
                        if (helpers::isPerfectNumber(value, pwr)) {
                            mapperEntry[pwr].push_back(value);
                        }
                    }
                }

            } 
            /*
                    If the file is operated on / has been
                operated on, continue onwards to the next file.
            */
            else {
                pthread_mutex_unlock(mapperArg.permissionLock);
                continue;
            }
        }

        mapperArg.result->insert(std::make_pair(mapperArg.mId, mapperEntry));

        /*
                Wait on barrier after doing all the mapper work
            in order to make the reducers start processing after
            all the mappers have finished.
        */
        pthread_barrier_wait(mapperArg.reducerBarrier);
        pthread_exit(NULL);
    }