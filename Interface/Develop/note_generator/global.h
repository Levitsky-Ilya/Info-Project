#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <string>

using namespace std;

/* Function for comparing two float quantities */
bool near(float data, float num, float range1, float range2);

/* Function for writing durations properly */
string writer(string part1, string part2, string part3, string wrName);

#endif // GLOBAL_H
