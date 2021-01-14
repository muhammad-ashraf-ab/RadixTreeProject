#include <ctime>
#include <fstream>
#include "RadixTree.h"
using namespace std;

ofstream generatedDNAFile;

char* generateTestSegment(const int len) {

    // Define the array of DNA letters
    const char DNA[4] = { 'A', 'C', 'G', 'T' };

    // Create character array for storing DNA segment
    char* testSegment = (char*)malloc((len + 1) * sizeof(char));
    for (int i = 0; i < len; i++) testSegment[i] = DNA[rand() % 4];

    // Set null terminator
    testSegment[len] = 0;

    generatedDNAFile << "*** DEBUG INFO | Generated DNA segment of length " << len << ": " << testSegment << endl;
    return testSegment;
}

int main() {
    // Initialize randomization seed
    srand(time(0));

    //Three files to generate outputs in. Replace "<Insert Address Here>" with your preferred address.
    char *dnaFileAddress = "<Insert Address Here>\\DNA_Test.txt";
    char *nodesFileAddress = "<Insert Address Here>\\Node_Count.txt";
    char *treeFileAddress = "<Insert Address Here>\\Tree.txt";

    // Instantiate tree
    RadixTree rt;

    // Generate random DNA segments and add them to the tree. Prints output to DNA_Test.txt
    generatedDNAFile.open(dnaFileAddress);
    for (int i = 0; i < 1000; i++)
        rt.addString(generateTestSegment(10 + rand() % 90));
    generatedDNAFile.close();

    //Prints generated nodes and their prefixes to Node_Count.txt
	rt.printNodes(nodesFileAddress);

	//Prints generated tree structure to Tree.txt
	rt.printTree(treeFileAddress);
    system("pause");
    return 0;
}
