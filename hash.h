#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <ctime>
#include <cstdlib>

using namespace std;

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }

    // Generate random R values using srand and rand
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }

    // Convert a character to a number 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const {
        if (letter >= '0' && letter <= '9') {
            return 26 + (letter - '0');  // '0' to '9' -> 26 to 35
        }
        if (letter >= 'A' && letter <= 'Z') {
            letter += 32;  // Convert uppercase to lowercase by adding 32
        }
        return letter - 'a';  // 'a' to 'z' -> 0 to 25
    }

    // Hash function entry point (h(k))
    HASH_INDEX_T operator()(const std::string& k) {
        const int chunkSize = 6;
        HASH_INDEX_T w[5] = {0,0,0,0,0};
        int n = k.length();
        for (int index = 4; index >=0; index--) {
            if (n <= 0) break;
            int start = std::max(0, n - chunkSize);
            HASH_INDEX_T baseValue = 1;
            HASH_INDEX_T chunkValue = 0;
            for (int j=start; j <n; j++) {
                chunkValue*=36;
                HASH_INDEX_T number = letterDigitToNumber(k[j]);
                chunkValue += number;
            }
            w[index] = chunkValue;
            n -= chunkSize;
        }

        // Compute final hash value
        HASH_INDEX_T hash = 0;
        for (int i = 0; i < 5; ++i) {
            hash += rValues[i] * w[i];
        }
        return hash;
    }
};

#endif // HASH_H