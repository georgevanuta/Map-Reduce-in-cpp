#include "helpers.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <ctype.h>
#include <vector>
#include <fstream>
#include <math.h>
#include <iostream>


void helpers::exitIf(const bool cond, const std::string msg) {
    if (cond) {
        printf("%s\n", msg.c_str());
        exit(-1);
    }
}


void helpers::exitIfInvalidArgs(const int argc, const char *argv[]) {
    helpers::exitIf(argc != 4, "[USAGE]: ./tema1 <NR_MAPPERS> <NR_REDUCERS> <TEST_FILE_PATH>");
    helpers::exitIf(!helpers::isPosInt(argv[1]), "[ERROR]: NR_MAPPERS must be a positive int.");
    helpers::exitIf(!helpers::isPosInt(argv[2]), "[ERROR]: NR_REDUCERS must be a positive int.");
}


bool helpers::isPosInt(const std::string nrStr) {
    return (std::all_of(nrStr.begin(), nrStr.end(), [](char c){return isdigit(c);}) && !nrStr.empty());
}


std::vector<std::string> helpers::getFilePaths(const std::string filePath) {
    std::ifstream inputFile(filePath);
    exitIf(inputFile.fail(), "[ERROR]: " + filePath + " file path doesn't exist.");
    std::string lineRead;

    getline(inputFile, lineRead);
    exitIf(!isPosInt(lineRead), "[ERROR]: First line of input file must be a positive int.");
    const unsigned long nrLines = std::stol(lineRead);

    std::vector<std::string> filePaths;

    for (unsigned long i = 0; i < nrLines; i++) {
        getline(inputFile, lineRead);
        filePaths.push_back(lineRead);
    }

    return filePaths;
}


bool helpers::isPerfectNumber(const unsigned long long int n, const unsigned int exp) {
    // base cases
    if (n == 1) return true;
    if (n == 0) return false;
    if (std::pow(2, exp) == n) return true;

    unsigned long long int low = 2;
    unsigned long long int high = n / exp;
    unsigned long long int mid;

    while (high - low > 1) {
        mid = (low + high) / 2;
        auto value = std::pow(mid, exp);
        if (value == n) return true;

        if (value > n) high = mid;
        else low = mid; 
    }

    return false;
}


