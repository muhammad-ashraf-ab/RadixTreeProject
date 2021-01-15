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
// It is possible to display the output in the console as well, but some characters used in...
// ...the tree visualization seem to be unsupported in some consoles, and also console echo...
// ...makes the output process slow down, so it is not advised to echo large tests in console.
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

    // Initialize randomization seed -- don't forget to change if you want to test new results
    srand(1234);

    // Please do not forget to set the address of the folder you would like the output to be printed in
    // Example address format: "C:\\Users\\Administrator\\Documents\\"

    char address[] = "";

    runRadixTreeDNATest(address, sizeof(address), true, 3, 7, 7); // Small test with console echo - for manual tracing and proof of concept
    runRadixTreeDNATest(address, sizeof(address), false, 10, 100, 10000); // Large test without echo - for capability testing

    system("pause");
    return 0;
}

char* intToChars(int n) {

    // Get number of digits of "n"
    int digitsNum = 1, temp = n;
    while (temp /= 10) digitsNum++;

    // Allocate memory for that number of digits + one more character for null terminator
    char* c = (char*)calloc(digitsNum + 1, sizeof(char));

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

    // Create character array for storing DNA segment
    char* testSegment = (char*)malloc((len + 1) * sizeof(char));
    for (int i = 0; i < len; i++) testSegment[i] = DNA[rand() % 4];

    // Set null terminator
    testSegment[len] = 0;

    // Print generated segment onto console if echo enabled
    if (echo) cout << "Generated DNA segment of length " << len << ": " << testSegment << endl;
    return testSegment;
}

void runRadixTreeDNATest(char address[], int addressSize, bool echo, int min, int max, int num) {
    static unsigned int test_num = 0; // Initialization of variable to store test number
    char* n = intToChars(++test_num); // Increment and convert on every call
    RadixTree rt; // Instantiate tree

    // Generate random DNA segments and add them to the tree
    // Prints debug output to console if echo is set to true
    for (int i = 0; i < num; i++)
        rt.addString(generateTestSegment(min + rand() % max, echo));

    if (echo) cout << endl << endl;

    // We now need to calculate the number of characters for the different file addresses
    // The file address is composed of the folder address provided to the function, the test number, and the file itself

    // For the file segment of the file address, we have the following arrays with their sizes already determined and fixed
    const char segments[13] = { 'S','e','g','m','e','n','t','s','.','t','x','t','\0' }; // "Segments.txt"
    const char nodes[10] = { 'N','o','d','e','s','.','t','x','t','\0' }; // "Nodes.txt"
    const char tree[9] = { 'T','r','e','e','.','t','x','t','\0' }; // "Tree.txt"

    // For the test number segment, a number of characters equal to the number of digits of the test number is needed
    // This code keeps incrementing before every iteration check until the character array reachs its null terminator
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

    // Finally, the files
    for (int i = 0; i < 13; i++) segmentsFileAddress[offset + i] = segments[i];
    for (int i = 0; i < 10; i++) nodesFileAddress[offset + i] = nodes[i];
    for (int i = 0; i < 9; i++) treeFileAddress[offset + i] = tree[i];

    // Now we can proceed with the actual test

    // Test Stage #1 ------------------------------------------------------------------------------------------
    // Prints all generated DNA segments, sorted alphabetically (via tree strings) to <Test Number>DNA_Test.txt
    // Prints debug output to console if echo is set to true
    rt.printStrings(segmentsFileAddress, echo);
    // --------------------------------------------------------------------------------------------------------

    if (echo) cout << endl << endl;

    // Test Stage #2 ------------------------------------------------------------------------------------------
    // Prints generated nodes and their prefixes to <Test Number>Node_Count.txt
    // Prints debug output to console if echo is set to true
    rt.printNodes(nodesFileAddress, echo);
    // --------------------------------------------------------------------------------------------------------

    if (echo) cout << endl << endl;

    // Test Stage #3 ------------------------------------------------------------------------------------------
    // Prints generated tree structure to <Test Number>Tree.txt
    // Prints debug output to console if echo is set to true
    rt.printTree(treeFileAddress, echo);
    // --------------------------------------------------------------------------------------------------------
}