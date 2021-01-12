#include <ctime>
#include <iostream>
#include "RadixTree.h"
using namespace std;

char* generateTestSegment(const int len) {
    // Initialize randomization seed
    srand(time(0));

    // Define the array of DNA letters
    const char DNA[4] = { 'A', 'C', 'G', 'T' };

    // Create character array for storing DNA segment
    char* testSegment = (char*)malloc((len+1) * sizeof(char));
    for (int i = 0; i < len; i++) testSegment[i] = DNA[rand() % 4];

    // Set null terminator
    testSegment[len] = 0;

    cout << "*** DEBUG INFO | Generated Test Segment: " << testSegment << endl;
    return testSegment;
}

int main() {
    RadixTree rt;
    cout << "Hello, World!" << endl;

    // rt.addString(generateTestSegment(30));
    // rt.addString(generateTestSegment(40));

    system("pause");
    return 0;
}
