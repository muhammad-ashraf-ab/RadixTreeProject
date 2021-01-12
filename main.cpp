#include <ctime>
#include <iostream>
#include "RadixTree.h"
using namespace std;

char* generateTestSegment(const int len) {

    // Define the array of DNA letters
    const char DNA[4] = { 'A', 'C', 'G', 'T' };

    // Create character array for storing DNA segment
    char* testSegment = (char*)malloc((len + 1) * sizeof(char));
    for (int i = 0; i < len; i++) testSegment[i] = DNA[rand() % 4];

    // Set null terminator
    testSegment[len] = 0;

    cout << "*** DEBUG INFO | Generated DNA segment of length " << len << ": " << testSegment << endl;
    return testSegment;
}

int main() {
    // Initialize randomization seed
    srand(time(0));

    // Instantiate tree
    RadixTree rt;

    // Generate random DNA segments and add them to tree
    for (int i = 0; i < 10; i++)
        rt.addString(generateTestSegment(10 + rand() % 90));

    system("pause");
    return 0;
}
