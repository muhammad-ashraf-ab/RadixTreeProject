#include <fstream>
#include <iostream>
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
    srand(1234);

    // TODO: In the following lines of code, please do not forget to replace the file address with your own.
    const char* dnaFileAddress = "<file address; replace me>\\1DNA_Test.txt";
    const char* nodesFileAddress = "<file address; replace me>\\1Node_Count.txt";
    const char* treeFileAddress = "<file address; replace me>\\1Tree.txt";

    const char* dnaFileAddress2 = "<file address; replace me>\\2DNA_Test.txt";
    const char* nodesFileAddress2 = "<file address; replace me>\\2Node_Count.txt";
    const char* treeFileAddress2 = "<file address; replace me>\\2Tree.txt";

    // Instantiate tree
    RadixTree rt;

    // Generate random DNA segments and add them to the tree. Prints output to DNA_Test.txt
    for (int i = 0; i < 10; i++)
        rt.addString(generateTestSegment(2 + rand() % 5, true));

    // Prints generated nodes and their prefixes to 1Node_Count.txt
    rt.printNodes(nodesFileAddress);

    // Prints all generated DNA segments, sorted alphabetically (via tree strings) to 1DNA_Test.txt
    rt.printString(dnaFileAddress);

    // Prints generated tree structure to 1Tree.txt
    rt.printTree(treeFileAddress);

    // Repeat the same process for a larger test sample
    for (int i = 0; i < 1000; i++)
        rt.addString(generateTestSegment(10 + rand() % 90, false));
    rt.printNodes(nodesFileAddress2);
    rt.printString(dnaFileAddress2);
    rt.printTree(treeFileAddress2);

    system("pause");
    return 0;
}