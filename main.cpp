#include <ctime>
#include<iostream>
#include <fstream>
#include "RadixTree.h"
using namespace std;


char* generateTestSegment(const int len, bool echo) {

    // Define the array of DNA letters
    const char DNA[4] = { 'A', 'C', 'G', 'T' };

    // Create character array for storing DNA segment
    char* testSegment = (char*)malloc((len + 1) * sizeof(char));
    for (int i = 0; i < len; i++) testSegment[i] = DNA[rand() % 4];

    // Set null terminator
    testSegment[len] = 0;

    if (echo) cout << "*** DEBUG INFO | Generated DNA segment of length " << len << ": " << testSegment << endl;
    return testSegment;
}

int main() {
    // Initialize randomization seed
    srand(time(0));

    // TODO: DO NOT FORGET TO REPLACE THE ADDRESS WITH YOUR OWN ADDRESS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    const char* dnaFileAddress = "<insert your address here>\\DNA_Test.txt";
    const char* nodesFileAddress = "<insert your address here>\\Node_Count.txt";
    const char* treeFileAddress = "<insert your address here>\\Tree.txt";

    const char* two_dnaFileAddress = "<insert your address here>\\DNA_Test2.txt";
    const char* two_nodesFileAddress = "<insert your address here>\\Node_Count2.txt";
    const char* two_treeFileAddress = "<insert your address here>\\Tree2.txt";

    // Instantiate tree
    RadixTree rt;

    // Generate random DNA segments and add them to the tree. Prints output to DNA_Test.txt
    for (int i = 0; i < 10; i++)
        rt.addString(generateTestSegment(2 + rand() % 5, true));
    //Prints generated nodes and their prefixes to Node_Count.txt
    rt.printNodes(nodesFileAddress);
    rt.printString(dnaFileAddress);
    //Prints generated tree structure to Tree.txt
    rt.printTree(treeFileAddress);

    for (int i = 0; i < 10000; i++)
        rt.addString(generateTestSegment(10 + rand() % 90, false));

    rt.printNodes(two_nodesFileAddress);
    rt.printString(two_dnaFileAddress);
    rt.printTree(two_treeFileAddress);

    system("pause");
    return 0;
}
