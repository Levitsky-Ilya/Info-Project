#include <iostream>
#include <string>

using namespace std;

bool near(float data, float num, float range1, float range2)
{
	if (((data + range1) >= num)&&((data - range2) <= num)) {
		return true;
	} else {
		return false;
	}
}


string writer(string part1, string part2, string part3, string wrName)
{
	if (part3 == "") {
		if (part2 == "") {
			return part1;
		}
		return part1 + wrName + part2;
	}
	return part1 + wrName + part2 + wrName + part3;
}
