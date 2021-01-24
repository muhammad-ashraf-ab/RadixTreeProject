#include <iostream>
using namespace std;

#include "RadixTree.h"

// ---------------------------------------------------------------------------------------------
// This function converts digits of an integer "n" into characters
// It is used as part of the DNA Test function
//
char* intToChars(int n);
// Returns character array (pointer to characters) representing the integer "n"
// ---------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------
// This function is used to generate DNA segments of length "len"
// Has the option to echo output to console
//
char* generateTestSegment(const int len, bool echo);
// Returns character array (ptc) of length "len" representing a random pseudo-DNA segment
// ---------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------
// The test used in this driver file for testing the implemented Radix Tree data structure,...
// ...it is responsible for randomly generating various pseudo-DNA segments, inserting them...
// ...into a radix tree, then sorting them traversing the tree and printing them as well as...
// ...printing the different nodes in the tree with their prefixes, then finally printing a...
// ...visualization of the tree. This output is mainly saved into three files per test.
// 
// It is possible to display the output on the console as well, but some characters used in...
// ...the tree visualization seem to be unsupported on some consoles, and also console echo...
// ...makes the output process slow down, so it is not advised to echo large tests on console.
//
// Its parameters are as follows:
//
// - address: Character array representing the address to the folder in which to save output
// - addressSize: Size of the above parameter, provide it using "sizeof" function
// - echo: Boolean variable to choose whether or not to echo output into console
// *** Please know that the output of the tree on command prompt consoles doesn't work ***
// - min: The minimum length of the DNA segment
// - max: The maximum length of the DNA segment
// - num: The number of DNA segments
//
void runRadixTreeDNATest(char address[], int addressSize, bool echo, int min, int max, int num);
// ---------------------------------------------------------------------------------------------

int main() {

    // Initialize randomization seed -- don't forget to change if you want to test new cases
    srand(1337);

    // Please do not forget to set the address of the folder you would like the output to be printed in
    // Example address format: "C:\\Users\\Administrator\\Documents\\" <-- Notice the terminating "\\"
    // If you do not want to print anything into external files, please leave the address blank (= "")
    // TODO: Do not forget to remove your personal address

    char address[] = "H:\\#_College\\#semester_5_fall\\CSE331 - Data Structures and Algorithms\\Project\\Source Code\\Latest (Personal)\\Text Files\\";

    runRadixTreeDNATest(address, sizeof(address), true, 4, 7, 10); // Small test with console echo - for manual output tracing & verification
    runRadixTreeDNATest(address, sizeof(address), false, 10, 100, 1000); // Problem Statement test - for validating requirements satisfaction
    runRadixTreeDNATest(address, sizeof(address), false, 100, 1000, 50000); // Larger test without echo - for capability & efficiency testing

    cout << endl << "All tests have been completed successfully. Press ENTER to exit...\n\n";
    cin.ignore();
    return 0;

}

char* intToChars(int n) {

    // Get number of digits of "n"
    int digitsNum = 1, temp = n;
    while (temp /= 10) digitsNum++;

    // Allocate memory for that number of digits + one more character for null terminator
    char* c = (char*) calloc(digitsNum + 1, sizeof(char));

    // Set each character to its corresponding digit
    for (int i = digitsNum - 1; i >= 0; i--) {

        c[i] = char('0' + n % 10);
        n /= 10;

    }

    // Set terminating null character
    c[digitsNum] = 0;

    return c;

}

char* generateTestSegment(const int len, bool echo) {

    // Define the array of DNA letters
    const char DNA[4] = { 'A', 'C', 'G', 'T' };

    // Create character array for storing DNA segment of "len" characters + 1 for null terminator
    char* testSegment = new char[len + 1];
    for (int i = 0; i < len; i++) testSegment[i] = DNA[rand() % 4];

    // Set null terminator
    testSegment[len] = 0;

    // Print generated segment onto console if echo enabled
    if (echo) cout << "Generated DNA segment of length " << len << ": " << testSegment << endl;
    return testSegment;

}

void runRadixTreeDNATest(char address[], int addressSize, bool echo, int min, int max, int num) {

    static unsigned int testNum = 0; // Initialization of variable to store test number
    char* n = intToChars(++testNum); // Increment and convert on every call
    RadixTree* rt = new RadixTree(); // Instantiate a Radix Tree

    cout << "Generating and inserting DNA Segments for Test #" << testNum << "...\n" << (echo ? "\n" : "");

    // Generate random DNA segments and add them to the tree
    // Prints debug output to console if echo is set to true
    for (int i = 0; i < num; i++) {

        // Provided that min and max are different, a segment of length between them is generated
        // Otherwise, if they are equal, a segment of length "min" is generated
        char* testSegment = generateTestSegment((max - min) ? (min + rand() % (max - min + 1)) : min, echo);
        rt->addString(testSegment);
        delete[] testSegment;

    }

    cout << endl;

    // We now need to calculate the number of characters for the different file addresses
    // The file address is composed of the folder address provided to the function, the test number, and the file itself

    // For the file segment of the file address, we have the following arrays with their sizes already determined and fixed
    const char segments[13] = { 'S','e','g','m','e','n','t','s','.','t','x','t','\0' }; // "Segments.txt"
    const char nodes[10] = { 'N','o','d','e','s','.','t','x','t','\0' }; // "Nodes.txt"
    const char tree[9] = { 'T','r','e','e','.','t','x','t','\0' }; // "Tree.txt"

    // For the test number segment, a number of characters equal to the number of digits of the test number is needed
    // This code keeps incrementing before every iteration check until the character array reaches its null terminator
    // This effectively leads to "digits" ending up as the number of characters in this array (excluding the null terminator)
    int digits = 0; while (n[++digits]);

    // For the address segment, we are provided its size in the "addressSize" variable, with which we can determine its length
    // However we need to take into consideration the null terminator, hence we subtract 1 from the resulting value

    // Now we can create our addresses character arrays:
    char segmentsFileAddress[addressSize / sizeof(char) - 1 + digits + 13];
    char nodesFileAddress[addressSize / sizeof(char) - 1 + digits + 10];
    char treeFileAddress[addressSize / sizeof(char) - 1 + digits + 9];

    // Next we initialize a variable to be used for iterating over the different addresses
    int offset = 0;

    // First, the folder segment (- 1 for NULL terminator)
    for (; offset < addressSize / sizeof(char) - 1; offset++)
        segmentsFileAddress[offset] = nodesFileAddress[offset] = treeFileAddress[offset] = address[offset];

    // Next, the test number
    for (int i = 0; i < digits; i++, offset++)
        segmentsFileAddress[offset] = nodesFileAddress[offset] = treeFileAddress[offset] = n[i];

    // And since we no longer need it, de-allocate its memory
    free(n);

    // Finally, the files
    for (int i = 0; i < 13; i++) segmentsFileAddress[offset + i] = segments[i];
    for (int i = 0; i < 10; i++) nodesFileAddress[offset + i] = nodes[i];
    for (int i = 0; i < 9; i++) treeFileAddress[offset + i] = tree[i];

    // Now we can proceed with the actual test

    // Test Stage #1 ------------------------------------------------------------------------------------------
    // Prints all generated DNA segments, sorted alphabetically (via tree strings) to <Test Number>DNA_Test.txt
    // Prints debug output to console if echo is set to true
    //
    cout << "Commencing Test #" << testNum << " Stage #1: ";
    cout << "Sorting all Radix Tree strings using the Heap Sort algorithm...\n" << (echo ? "\n" : "");

    rt->sortAndPrintStrings(*address ? segmentsFileAddress : "", echo);

    cout << (echo ? "\n" : "") << "Test #" << testNum << " Stage #1 complete.\n\n" << (echo ? "\n\n" : "");
    // --------------------------------------------------------------------------------------------------------



    // Test Stage #2 ------------------------------------------------------------------------------------------
    // Prints generated nodes and their prefixes to <Test Number>Node_Count.txt
    // Prints debug output to console if echo is set to true
    //
    cout << "Commencing Test #" << testNum << " Stage #2: ";
    cout << "Printing all Radix Tree nodes with their respective prefixes...\n" << (echo ? "\n" : "");

    rt->printNodes(*address ? nodesFileAddress : "", echo);

    cout << (echo ? "\n" : "") << "Test #" << testNum << " Stage #2 complete.\n\n" << (echo ? "\n\n" : "");
    // --------------------------------------------------------------------------------------------------------



    // Test Stage #3 ------------------------------------------------------------------------------------------
    // Prints generated tree structure to <Test Number>Tree.txt
    // Prints debug output to console if echo is set to true
    //
    cout << "Commencing Test #" << testNum << " Stage #3: ";
    cout << "Printing a visualization of the entirety of the Radix Tree...\n" << (echo ? "\n" : "");

    rt->printTree(*address ? treeFileAddress : "", echo);

    cout << (echo ? "\n" : "") << "Test #" << testNum << " Stage #3 complete.\n\n" << (echo ? "\n\n" : "");
    // --------------------------------------------------------------------------------------------------------

    // Finally, de-allocate the memory occupied by the Radix Tree
    //
    cout << (echo ? "\n" : "") << "Cleaning up, please wait...\n";
    cout << "If this is the last test, you may force exit.\n\n" << (echo ? "\n\n" : "");

    delete rt; // FIXME: Very slow, can't put my hands on why yet

}