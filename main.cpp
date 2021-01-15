#include <ctime>
#include<iostream>
#include <fstream>
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

    cout<< "*** DEBUG INFO | Generated DNA segment of length " << len << ": " << testSegment << endl;
    return testSegment;
}

int main() {
    // Initialize randomization seed
    srand(time(0));

    //Three files to generate outputs in. Replace "<Insert Address Here>" with your preferred address.
    const char *dnaFileAddress = "E:\\school\\structures bs\\data structures\\project\\inout\\DNA_Test.txt";
    const char *nodesFileAddress = "E:\\school\\structures bs\\data structures\\project\\inout\\Node_Count.txt";
    const char *treeFileAddress = "E:\\school\\structures bs\\data structures\\project\\inout\\Tree.txt";

    // Instantiate tree
    RadixTree rt;

    // Generate random DNA segments and add them to the tree. Prints output to DNA_Test.txt
    for (int i = 0; i < 10; i++)
        rt.addString(generateTestSegment(2 + rand() % 5));
    //Prints generated nodes and their prefixes to Node_Count.txt
	rt.printNodes(nodesFileAddress);
	rt.printString(dnaFileAddress);
	//Prints generated tree structure to Tree.txt
	rt.printTree(treeFileAddress);
    system("pause");
    return 0;
}
