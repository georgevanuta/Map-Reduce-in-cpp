#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <vector>

// ------ HELPERS NAMESPACE ------
namespace helpers {
    
    /*
            Exits if the given condition is met,
        printing the giving message to stdout.
    */
    void exitIf(const bool cond, const std::string msg);

    /*
            Exits if invalid input is sent to 
        the app via cmd line args.
    */
    void exitIfInvalidArgs(const int argc, const char *argv[]);


    /*
            Returns true is a given string
        is a positive integer.
    */
    bool isPosInt(const std::string nrStr);

    /*
            Given a test.txt file path returns
        the path to the files used by the mapper.
    */
    std::vector<std::string> getFilePaths(const std::string filePath);

    /*
            Returns true if the given number
        is a perfect power.
    */
    bool isPerfectNumber(const unsigned long long int n, const unsigned int exponent);

}


#endif