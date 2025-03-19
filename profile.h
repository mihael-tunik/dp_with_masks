#ifndef _PROFILE_H
#define _PROFILE_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <string.h>
#include <set>
#include <chrono>

using namespace std;

template <typename T>
void profile(int (*_func)(T &), T a);

template <typename T>
void profile(int (*_func)(T &), T a){
    std::chrono::high_resolution_clock::time_point start, stop;

    start = chrono::high_resolution_clock::now();
    (*_func)(a);
    stop = chrono::high_resolution_clock::now();
    
    printf("Ready in %lf s.\n", chrono::duration<double, milli>(stop-start).count()/1000);
}
#endif
