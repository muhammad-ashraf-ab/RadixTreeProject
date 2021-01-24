//----------------------------------------------------------------------------------------------------------------------
// This project was created for CSE_331 Data Structures And Algorithms course offered in
// Ain Shams University - Faculty of Engineering under the guidance and influence of Dr. Ashraf Abdel Raouf
//
// This implementation has been greatly influenced by the implementation found in the following source:
// https://kukuruku.co/post/radix-trees/
//----------------------------------------------------------------------------------------------------------------------
#include <iostream>
using namespace std;

#include "RadixTree.h"

int RadixTree::prefix(char* x, int n, char* key, int m) {

    // This function iterates over the two character arrays, comparing the value of each character every iteration.
    // This comparison loop will terminate in one of three situations:
    //
    // 1. Iterator reaches end of first character array, meaning all its characters were common. Returns its length.
    // 2. Iterator reaches end of second character array, meaning all its characters were common. Returns its length.
    // 3. Characters are not equal. Returns iterator value at that moment.

    int iterator = 0;

    for (; iterator < n && iterator < m; iterator++)
        if (x[iterator] != key[iterator]) break;

    return iterator;

}

RadixTree::Node* RadixTree::find(Node* t, char* x, int n) {

    // if n is uninitialized, its value is set as the size of "x" (INCLUDING null character, i.e. size of "abc" is 4)
    // this is consistent with the expected value of the "len" member of each node: number of character, null included

    if (!n) while (x[n++]);

    // if provided tree node "t" is null, then there is nothing to be found, we have reached the end of this branch

    if (!t) return 0;

    // otherwise, if node has value, find the common prefix between it and the key being searched for, "x"
    // the prefix function is provided the size of both character arrays INCLUDING the null character

    int k = prefix(x, n, t->key, t->len);

    // if there's nothing in common, repeat the process for the next node in this tree level

    if (k == 0) return find(t->next, x, n);

    // if all of "x" is prefix, this means the current node IS "x" itself, so return it

    if (k == n) return t;

    // if the entirety of the current node is a prefix itself...
    // this means that the string we're looking for could span more than one node...
    // therefore, continue searching for the remaining part of the searched-for value... how?
    // go to the node that links this tree level to the next level, a.k.a. t->link, and inside it...
    // "x + k" makes us skip ahead the prefix found and search for the rest of the string, meanwhile...
    // "n - k" is the size of the rest of the string.

    if (k == t->len) return find(t->link, x + k, n - k);

    return 0;

}

void RadixTree::split(Node* t, int k) {

    // Let's use the following example to explain this function:

    // Split the following node at position 4:  [parent ---- "ABCDEF null" ---- child]
    // Required Result:                         [parent ---- "ABCD" ---- "EF null" ---- child]

    // Create a node that carries everything after the first "k" characters in the current node

    Node* p = new Node(t->key + k, t->len - k); // In our example, this means: p = "EF null"

    // Set this newly created node's link/child node as the current one's (maintaining sequentiality)

    p->link = t->link; // In our example, this means: p = "EF null" ---- child

    // Set the current node's link/child as the newly created node

    t->link = p; // In our example, this means: t = "ABCDEF null" ---- "EF null" ---- child

    // Now take the first "k" characters and place them in a temporary character array

    char* a = new char[k];
    for (int i = 0; i < k; i++) a[i] = t->key[i];

    // Delete the current key and replace it with the temporary character array just created, with its size

    delete[] t->key;
    t->key = a;
    t->len = k;

    // In our example, this means: t = "ABCD" ---- "EF null" ---- child
    // Successfully splitting the node at position 4.

}

RadixTree::Node* RadixTree::insert(Node* t, char* x, int n) {

    // if n is uninitialized, its value is set as the size of "x" (INCLUDING null character, i.e. size of "abc" is 4)
    // this is consistent with the expected value of the "len" member of each node: number of character, null included

    if (!n) while (x[n++]);

    // if current tree node "t" is null, this means we've reached our insertion point, hence create and return the node
    // notice that this function is used either within another function (like addString) or recursively within itself...
    // i.e. the returned node goes to the root of the tree or somewhere else in it, depending on the parent function

    if (!t) return new Node(x, n);

    // otherwise, if node has value, find the common prefix between it and the key to be inserted, "x"
    // the prefix function is provided the size of both character arrays INCLUDING the null character

    int k = prefix(x, n, t->key, t->len);

    // if there's nothing in common, attempt to insert the node to be inserted in the "next" node of the current node

    if (k == 0) t->next = insert(t->next, x, n);

    // otherwise, if part of the current node is a prefix of the key to be added...
    // observe the following examples of current nodes for key ABCF-null:
    // 1. current node: ABCDE-null,    ... k < n (3 < 5)
    // 2. current node: ABC-null,      ... k < n (3 < 5)
    // 3. current node: ABCF,          ... k < n (4 < 5)
    // 4. current node: ABCF-null,     ... k = n (5 = 5) (not in this else if branch)

    else if (k < n)
    {
        // if the node is larger, split it
        // -- this applies to example 1, where "ABCDE-null" would be split into "ABC|DE-null".
        // -- this also applies to example 2, where "ABC-null" would actually be split into "ABC|null"
        // -- however in case of example 3, the current node size is actually equal to the prefix, so no split

        if (k < t->len) split(t, k);

        // at this point, we insert what remains after the prefix (aka the "F")

        t->link = insert(t->link, x + k, n - k);

        // for example 1, this gives us "ABC-DE-null" and "ABC-F-null" as two new children, but no "ABC-null"
        // for example 2, this gives us "ABC-null" and "ABC-F-null" as the two children.
        // for example 3, we only get "ABCF-null"
    }

    // otherwise, the node to be added is the current node itself (k != 0 and >= n), so just return it! (example 4)
    return t;

}

void RadixTree::join(Node* t) {

    // Let's use the following example to explain this function:

    // Join the following node (with its link): [parent ---- "ABCD" ---- "EF null" ---- child]
    // Required Result:                         [parent ---- "ABCDEF null" ---- child]

    // Point towards the link node of the current node

    Node* p = t->link; // In our example, this means: p = "EF null"

    // Create a character array to store the current node as well as the link node's characters

    char* a = new char[t->len + p->len];

    // Copy the contents of the current node into the temporary array

    for (int i = 0; i < t->len; i++) a[i] = t->key[i]; // a = "ABCD"

    // Copy the contents of the link node afterwards

    for (int i = 0; i < p->len; i++) a[t->len + i] = p->key[i]; // a = "ABCDEF null"

    // Delete the current key and replace it with the temporary character array just created

    delete[] t->key;
    t->key = a;

    // Increase its size by the size of the link node

    t->len += p->len;

    // Set the link of the current node as the link of its own link node (i.e. skipping it)

    t->link = p->link; // In our example, this means: t = "ABCDEF null" ---- child

    // Finally, delete the now-duplicated node

    delete p;

}

RadixTree::Node* RadixTree::remove(Node* t, char* x, int n) {

    // if n is uninitialized, its value is set as the size of "x" (INCLUDING null character, i.e. size of "abc" is 4)
    // this is consistent with the expected value of the "len" member of each node: number of character, null included

    if (!n) while (x[n++]);

    // if provided tree node "t" is null, then there is nothing to be removed

    if (!t) return 0;

    // otherwise, if node has value, find the common prefix between it and the key being searched for, "x"
    // the prefix function is provided the size of both character arrays INCLUDING the null character

    int k = prefix(x, n, t->key, t->len);

    // if all of "x" is prefix, this means the current node IS "x" itself, so remove it (by replacing it with its next)

    if (k == n)
    {
        Node* p = t->next;
        delete t;
        return p;
    }

    // if there's nothing in common, repeat the process for the next node in this tree level

    if (k == 0) t->next = remove(t->next, x, n);

    // otherwise if the current node is a prefix itself... for example...
    // key: ABCDE-null
    // current node: ABC

    // then: k = 3, t->len = 3, n = 6

    else if (k == t->len)
    {
        // proceed to the link(s) of this node with the rest of the key (in this case, DE-null), attempting to remove again
        // let's say the link was "DE-null". If we go upwards, we find that this is the (k == n) case, where the link gets...
        // ...replaced by the link after it (connected to it by "next").

        t->link = remove(t->link, x + k, n - k);

        // accordingly, if "t" ends up with only one link (which we can find by seeing if its link has a next or not)...
        // ...merge it with that link so as to make it one node.

        if (t->link && !t->link->next) join(t);
    }

    // otherwise, the node to be removed was not found, so return this node as it is (i.e. no changes)

    return t;

}

int RadixTree::countStringsAux(Node* t) {

    if (!t) return 0; // if provided tree node "t" is null, then there is no strings in it

    return (!t->link) // if it has no children, then it is the end of a string so add 1
        + countStringsAux(t->next) // then recursively visit all siblings
        + countStringsAux(t->link) // then recursively visit all children
        ;
}

int RadixTree::countNodesAux(Node* t) {

    if (!t) return 0; // if provided tree node "t" is null, then there is no Nodes in it

    return 1 // Always add 1. Doesn't matter if it's a leaf node or not
        + countNodesAux(t->next) // then recursively visit all siblings 
        + countNodesAux(t->link) // then recursively visit all children 
        ;
}

RadixTree::Node* RadixTree::sortRadixTreeAux(Node* head) {

    // A rather long example summarizing the steps and code below can be found at the end of this file

    // ---------------------------------------------------------------------------------------------------------------
    // Step 0: A head must have at least one sibling; otherwise...
    //         If it were an invalid pointer altogether, return it directly
    //         If it has no siblings, recursively iterate over its children, then return it
    //         Additionally, prepare all the node pointers that will be used throughout this function
    // ---------------------------------------------------------------------------------------------------------------

    if (!head) return head;
    else if (!head->next) { if (head->link) head->link = sortRadixTreeAux(head->link); return head; }

    Node* mid = head, * last = head, * t2 = 0, * t1 = 0, * newHead = 0, * temp = 0;

    // ---------------------------------------------------------------------------------------------------------------
    // Step 1: Splitting into sub-lists recursively
    // ---------------------------------------------------------------------------------------------------------------

    // Initial call sets "head" as root node, and "mid" & "last" always need to point at it initially
    // The pointer "mid" will be used to traverse all nodes till it reaches the sibling at the middle
    // The pointer "last" will be used to traverse all nodes till it reaches the (pre-)last sibling
    // Traverse all siblings of the current level until "mid" & "last" point to their targets
    while (last->next && last->next->next) { mid = mid->next; last = last->next->next; }

    // Recursively split the sub-list that starts with the sibling to the node at the middle (i.e. the second half)
    t2 = sortRadixTreeAux(mid->next);

    // Nullify the sibling pointer of the middle node (i.e. unlink these siblings)
    mid->next = NULL;

    // Recursively merge the sub-list that starts with the head node (i.e. the first half)
    t1 = sortRadixTreeAux(head);

    // ---------------------------------------------------------------------------------------------------------------
    // Step 2: New Head Determination
    // ---------------------------------------------------------------------------------------------------------------

    // It is now time to find the new head of this sub-list
    // At the top of the stack each sub-list will be just one node
    // We compare them to see who will be the sibling to become the new head
    // Recursively, this leads to generating sorted sub-lists that undergo the same operation

    // First, we check for the possible scenario that one of the sub-lists is actually empty (head = 0)
    if (!t1) return t2; if (!t2) return t1; // In that case, the new head will be the other sub-list's directly

    // Now is time to use the "newHead" pointer in the task of determination of the new head of the merged sub-list
    // Loop over the keys of the two nodes as long as neither of them ends while characters are equal (indicated by...
    // ..."newhead" pointing at a null value), where once characters differ "newHead" points to either "t1" or "t2"...
    // ...and otherwise it stays null... It is crucial to remember that "t1" and "t2" are ***sibling nodes***
    //
    for (int i = 0; i < t1->len && i < t2->len && !newHead; i++)
        newHead = (t1->key[i] < t2->key[i]) ? t1 : (t2->key[i] < t1->key[i]) ? t2 : 0;

    // When loop breaks, "newHead" is guaranteed to point to the correct node, in doubt? Here are the possible faults:

    // - "ABC-null" vs. "ABC-null": Invalid case; sibling leaf nodes cannot be equal (means duplicate string exists)
    // - "ABC" vs. "ABC-null": Invalid case; if "ABC" exists (i.e. there's "ABC | something"), it will be a prefix
    // - "ABC" vs. "AB-null": Invalid case; sibling nodes have "AB" in common, it will be a prefix to "C" & "null"
    // - "ABC" vs. "ABCD": Invalid case; if "ABC" exists, "ABCD" cannot, as it would be split into "ABC" | "D"
    // - "ABC" vs. "ABC": Invalid case; duplicate sibling nodes cannot exist in the Radix Tree

    // At this point we increment either pointer "t1" or "t2" based on what "newHead" points to, so that we proceed...
    // ...with the sibling of the head and not the head itself in the sorting and merging step of this function
    //
    if (newHead == t1) t1 = t1->next; else t2 = t2->next;

    // ---------------------------------------------------------------------------------------------------------------
    // Step 3: Merging and Sorting (mergesorting) the sub-lists
    // ---------------------------------------------------------------------------------------------------------------

    temp = newHead; // Node pointer to be used in merging and sorting the sub-tree's nodes, if they do exist
    // The same logic used above is used here again to determine which node is smaller out of the two nodes
    // It starts with the "newHead" since all it does is keep setting the sibling of every node

    // Keep merging (sorting) for as long as both nodes exist
    while (t1 && t2) {

        for (int i = 0; i < t1->len && i < t2->len; i++) {

            // If "t1" is smaller than "t2", then the next node in our sorted list should be "t1"
            // Next, we need "temp" to point at the latest addition to the list, which is "t1"
            // Finally, we need to increment "t1" so it gets evaluated in the next iteration
            // The opposite happens in case of "t2" being the smaller node of the two
            //
            if (t1->key[i] < t2->key[i]) { temp->next = t1; temp = t1; t1 = t1->next; break; }
            if (t1->key[i] > t2->key[i]) { temp->next = t2; temp = t2; t2 = t2->next; break; }

        }
    }

    // Now once we're outside the loop, we need to remember a crucial piece of information:
    // "t1" and "t2" point to *sorted* sub-lists. Hence, if the loop took two sub-lists...
    // ...of lengths "4", and all nodes after "t1" were smaller than "t2" (i.e. no node...
    // ...from list starting with "t2" has been added yet), simply assigning the "temp"...
    // ...to "t2" would add all of these nodes to the list, successfully merging + sorting
    //
    temp->next = (t1 ? t1 : t2);

    // With this, we have now merged all sub-lists, with "newHead" pointing at the...
    // ...resulting merged (and sorted) list, so we return it.
    return newHead;

}

char** RadixTree::fetchStringsAux(Node* t, bool echo, bool sort) {

    // First, we prepare a pointer that would point to a clone of the current Radix Tree to sort it if required before fetching strings
    static RadixTree* clone;

    static int offset, stringNum, numOfStrings; // Offset in string's prefix, string's number, and number of strings
    static char** strings; // Array (P2P) to store all of the strings that get fetched from the current Radix Tree
    static char* prefix; // Dynamically allocated prefix for sibling nodes, doesn't need explicit initialization

    // If start of first call to function (indicated by "t" pointing at the root node), begin initialization
    // There is a special case here where the root node of the original tree may reappear later on in the...
    // ...sorted tree, hence we need an extra check and that is the clone value, where if it is null then...
    // ...it hasn't been created yet (i.e. we didn't enter this function before), but afterwards it'll exist
    if (t == root && !clone) {

        if (sort) { // If sorting is required

            clone = new RadixTree(this); // Create a clone of the current tree

            // Then sort its nodes via merge sort (modifies the tree; hence the cloning)...
            // ...and then, set "t" to point at the clone tree's root instead of this one's
            clone->sortRadixTree();
            t = clone->root;

        } // Otherwise, it is already initialized as the root from the original call

        offset = stringNum = 0;
        numOfStrings = countStrings();
        strings = (char**) calloc(numOfStrings, sizeof(char*));

    }

    // For prefixes, dynamically resize such that it contains:
    // 1. Itself (length determined by its current offset value)
    // 2. Node "t"'s key (length determined by node variable "len")
    // For leaf nodes, "len" takes into consideration the null terminator, so we don't need to explicitly add "1"
    prefix = (char*) realloc(prefix, sizeof(char) * (offset + t->len));

    // For all characters of the node's key, including NULL character if exists, copy into current string, incrementing the offset simultaneously
    for (int i = 0; i < t->len; i++) prefix[offset++] = t->key[i];

    if (t->link) fetchStringsAux(t->link, echo, sort); // If a child exists, traverse to it, which recursively means traverse until the leaf node

    // If we reach a leaf (i.e end of a string) the prefix would have a null terminator (copied from it), which is our signal to finally store it
    if (prefix[offset - 1] == 0) {

        // Re-allocate the current string's size and fill it with what's been added to the prefix thus far
        strings[stringNum] = (char*) realloc(strings[stringNum], offset * sizeof(char));
        for (int i = 0; i < offset; i++) strings[stringNum][i] = prefix[i];

    }

    // At this point in the code, we've finished processing the first recursive call
    // This means its time to visit a sibling node (or - if none exist - terminate this function and proceed to parent function - if exists)
    // In all cases, this calls for shrinking the prefix via removing the current node's key from it - done by simply changing the offset value
    offset -= t->len;

    // If a sibling exists, traverse to it, which recursively means traverse over all sibling nodes, which also implies we're having a new string
    if (t->next) { stringNum++; fetchStringsAux(t->next, echo, sort); }

    // If end of first call to function (indicated by the the current node now being the clone's root node - if we're sorting - or the root node)
    if (t == (sort ? clone->root : root)) {

        // Print all of the generated and stored strings if required
        if (echo) for (int i = 0; i < numOfStrings; i++) cout << i << ". " << strings[i] << endl;

        // De-allocate memory occupied by the clone tree and the prefix, then reset it to being a null pointer for potential future re-usage
        if (sort) delete clone; clone = 0;
        free(prefix); prefix = 0;

        // Finally, return the array of strings (expecting whoever uses it to de-allocate it themselves afterwards)
        return strings;

    }

    // If not the first call, and there's no child or sibling, then terminate this function and return nothin; nobody is awaiting its return value
    return 0;
}

void RadixTree::heapifyNodes(Node** arr, int n, int i) {

    int largest = i; // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && arr[l]->key[0] > arr[largest]->key[0])
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r]->key[0] > arr[largest]->key[0])
        largest = r;

    // If largest is not root
    if (largest != i) {

        swap(arr[i], arr[largest]);

        // Recursively heapify the affected sub-tree
        heapifyNodes(arr, n, largest);

    }
}

void RadixTree::heapSortNodes(Node** arr, int size) {

    // Build heap (rearrange array)
    for (int i = size / 2 - 1; i >= 0; i--)
        heapifyNodes(arr, size, i);

    // One by one extract an element from heap
    for (int i = size - 1; i > 0; i--) {

        // Move current root to end
        Node* temp = arr[i];
        arr[i] = arr[0];
        arr[0] = temp;

        // call max heapify on the reduced heap
        heapifyNodes(arr, i, 0);

    }
}

void RadixTree::sortAndPrintStringsAux(Node* t, bool echo, char* p, int pLen) {

    if (!t) return; // if provided tree node "t" is null, then there is nothing to do.

    int itr = 0;  // iterator to loop over all siblings of the current node.

    // Array of nodes to hold all nodes in the current level.
    // Size equal the number of characters in ASCII table.
    Node** arr = new Node * [128];
    Node* temp = t;

    // Store all siblings in the array.
    while (temp != 0)
    {
        arr[itr] = temp;
        temp = temp->next;
        itr++;
    }

    // Sort using heap sort algorithm.
    heapSortNodes(arr, itr);
    Node* n;

    // Looping over the sorted array of nodes.
    for (int i = 0; i < itr; i++)
    {
        n = arr[i];

        // If the node has no children, print it.
        if (!n->link) {

            // Printing the prefix of the node.
            for (int i = 0; i < pLen; i++) { if (segmentsFile) segmentsFile << p[i]; if (echo) cout << p[i]; }

            // Printing the the node itself.
            for (int i = 0; i < n->len; i++) { if (segmentsFile) segmentsFile << n->key[i]; if (echo) cout << n->key[i]; }

            if (segmentsFile) segmentsFile << endl; if (echo) cout << endl;

        } else {

            // If the node has children, create a new prefix that contains the previous prefix and the current node.
            char* newPrefix = new char[n->len + pLen];

            // Inserting the characters of the node's prefix.
            for (int i = 0; i < pLen; i++)
                newPrefix[i] = p[i];

            // Inserting the characters of the node itself.
            for (int i = 0; i < n->len; i++)
                newPrefix[i + pLen] = n->key[i];

            // Visit current node's children and pass the newly created prefix.
            sortAndPrintStringsAux(n->link, echo, newPrefix, n->len + pLen);

            // Then deallocate its memory when done.
            delete[] newPrefix;

        }
    }

    // De-allocate array of nodes.
    delete[] arr;
}

void RadixTree::printNodeWithPrefix(Node* n, char* p, int pLen, bool echo) {

    // First we check if node has a prefix, because if so, we print it, followed by a separator
    if (pLen) {

        for (int i = 0; i < pLen; i++) { if (nodesFile) nodesFile << p[i]; if (echo) cout << p[i]; }
        if (nodesFile) nodesFile << " | "; if (echo) cout << " | ";

    }

    // Then we check if node is empty (key is only a null character; means end of a string) and if so we print (NULL) and terminate function
    if (n->key[0] == 0) { if (nodesFile) nodesFile << "(NULL)\n"; if (echo) cout << "(NULL)\n"; return; }

    // Otherwise if it is not empty, we print its key, character by character, since some nodes may not have a null terminator
    for (int i = 0; i < n->len; i++) { if (nodesFile) nodesFile << n->key[i]; if (echo) cout << n->key[i]; }

    // And finally a line break
    if (nodesFile) nodesFile << endl;
    if (echo) cout << endl;

}

void RadixTree::printNodesAux(Node* t, bool echo) {

    static char* p; // Character array to contain node prefix
    static int v, pLen; // "v": The number of nodes already visited, "pLen": The length of prefix

    if (t == root) v = 0; // If commencing a new test, which can be concluded if current node is the root, re-initialize "v"

    if (nodesFile) nodesFile << "Node #" << v << " - "; // Printing into output file
    if (echo) cout << "Node #" << v << " - "; // Printing into console, is very slow

    v++; // Incrementing the visited nodes counter as we've already printed the number

    printNodeWithPrefix(t, p, pLen, echo); // Printing the node along with its respective prefix

    // And now, we want to add the current node's key to the prefix so that when we visit its child its prefix is full

    p = (char*) realloc(p, sizeof(char) * (pLen + t->len)); // But before addition the character array needs re-allocation

    // The loop below adds the key of the current node "t" to the prefix, while maintaining the value it has at this moment
    // It does that by simultaneously incrementing the variable representing prefix length, "pLen", while using it as index

    for (int i = 0; i < t->len; i++) p[pLen++] = t->key[i];

    if (t->link != 0) printNodesAux(t->link, echo); // Recursive call to visit all the children of the node "t" (if exists)

    pLen -= t->len; // Decrementing "pLen" to remove the key of the node "t" from the prefix since we're visiting a sibling

    if (t->next != 0) printNodesAux(t->next, echo); // Recursive call to visit all the siblings of the node "t" (if exists)

    if (t == root) { // If the pointers match then "t" is indeed the root node, meaning that recursion is now complete

        free(p); // Finally, as prefix/node printing is now over, free the memory allocated for the static array "p"
        p = 0; // Then re-initialize it to a null pointer for potential future re-usage of the static array "p"

    }

}

void RadixTree::printTreeAux(const Node* t, int pLen, bool echo) {

    // Allocate two boolean arrays, "p" and "temp", which will be used as follows:
    // "p" will be used in the tree visualization process to determine which character set to use for prefixes.
    // "temp" will be used in dynamically resizing "p", mechanism explained further down in this function's code.

    static bool* p, * temp;

    {	// This part is in a scope of its own since it is only concerned with printing and does not affect "p" or "temp".

        // This loop is responsible for printing out the character set based on the value of "p". It always starts printing its content
        // prior to other operations; hence why "p" is also referred to as "The Prefix". The usage and choosing of these character sets
        // will also be explained further down in this function's code, for now we're concerned with the printing operation(s) only.
        //
        for (int i = 0; i < pLen; i++) {

            // Prints to the tree file.		
            if (treeFile) treeFile << (p[i] ? "│   " : "    ");

            // Prints to the console, given echo is true.
            if (echo) cout << (p[i] ? "│   " : "    ");
        }

        // This line prints a special set of characters that represent whether the node has siblings or it is the last sibling.
        // It should be noted that the special characters chosen do not appear as intended on the Windows "cmd" console, due to
        // being part of the extended ASCII character set which is not supported on it. Anyway, some IDEs have their own custom
        // output consoles, so it is advised to switch to them if this issue were to occur for correct tree visualization.
        //
        // In any case though, the text file should provide the correct visualization, provided the right encoding.
        //
        // Example of the different evaluations for the following conditionals:
        // ...
        // ├───TGGTTGA
        // ├───CTT
        // └───GCCCC
        //        
        if (treeFile) treeFile << ((t->next != 0) ? "├───" : "└───");

        // Prints to the console, given echo is true.
        if (echo) cout << ((t->next != 0) ? "├───" : "└───");

        // Loop responsible for printing out the key of a node.
        // First, check if the node key is null. If it is, prints (NULL) to prevent empty tree branches.
        if (t->key[0] == 0) {

            // Prints to the tree file.
            if (treeFile) treeFile << "(NULL)\n";

            // Prints to the console, given echo is true.
            if (echo) cout << "(NULL)\n";

        // Otherwise, proceed with printing the node's key character by character.
        // We don't simply pass the pointer to the output stream(s), since not all nodes have a null terminator.
        } else {

            for (int i = 0; i < t->len; i++) {

                // Prints the content of the node, including the final null character if it exists, given first was not null.
                if (treeFile) treeFile << t->key[i];

                // Prints to the console, given echo is true
                if (echo) cout << t->key[i];

            }

            // Prints a new line
            if (treeFile) treeFile << endl;
            if (echo) cout << endl;

        }
    }

    // The following part chooses whether to append the special character set "│   " or four spaces "    " to branches of the tree.
    //
    // Example of the final output:
    // 1- In case of choosing "│   ":
    //
    // ...
    // │   ├───T
    // │   │   ├───CTT
    // ...
    //
    // 2- In case of choosing "    ":
    //
    // ...
    // │       │       └───A
    // │       └───G
    // ...
    //
    // Of course, the size of the boolean array needs to be increased by one unit to be able to store the previous choices made.
    // The eight following lines of code cover the procedure of increasing the array's size while maintaining its data.
    // An example explaining how they operate in this recursive environment can be found at the end of this file.

    // Allocate memory for the temporary boolean array for "pLen" boolean values for the prefix choices.
    temp = new bool[pLen];

    // Copy the content that currently exists in the prefix into the temporary array.
    for (int i = 0; i < pLen; i++) temp[i] = p[i];

    // Delete whatever the prefix is pointing at currently.
    delete[] p;

    // Now expand the prefix by one unit (the new choice to be added).
    p = new bool[pLen + 1];

    // Now fill it with its original data (first pLen iterations) then in the last iteration,
    // add the new choice to be stored, which is made based on whether a sibling exists or not.
    // The choice "true" means we've chosen "│   " because "next" was not null (i.e. sibling exists),
    // while the choice "false" means we've chosen "    " because "next" was null (i.e. sibling doesn't).
    // The printing loop visited earlier knows what "true" and "false" are associated with and prints accordingly.
    for (int i = 0; i < pLen + 1; i++) p[i] = (i < pLen) ? temp[i] : (t->next != 0);

    // Finally, de-allocate the temporary array, as it is no longer useful to us, and will be re-initialized the next iteration.
    delete[] temp;

    // Now, make a recursive call to the function to traverse on to the first child using the new content of "p", provided that one exists.
    // It should be noted that the size is sent as "pLen + 1" rather than increment "pLen" so "p" would be as it is now after returning,
    // while the child is sent the full prefix (i.e. old prefix + what we just appended to it) as it is in the next level of the tree.
    if (t->link != 0) printTreeAux(t->link, pLen + 1, echo);

    // Once done with all children (and their siblings), we return to this function with the prefix length as it was before leaving.
    // Now, we make a recursive call to the next sibling of the node to keep iterating through them with the same prefix.
    // This is provided that a sibling does exist. If not, resume function execution.
    if (t->next != 0) printTreeAux(t->next, pLen, echo);

    // Finally, check if we're at the very first function call (via checking if "t" is root). If so, nullify the prefix.
    if (t == root) { delete[] p; p = 0; }

    // Afterwards the function terminates and the stack is popped (i.e. parent function may now resume execution).
    // An example of the final output of the function can be found in the function declaration in RadixTree.h

}

// Addition function, updates root with a new root containing the string to be added
void RadixTree::addString(char* str) {
    root = insert(root, str);
}

// Deletion function, updates root with a new root that does not contain the string to be deleted
void RadixTree::deleteString(char* str) {
    root = remove(root, str);
}

// Searching function returns, boolean value based on the result of the finder function
bool RadixTree::searchString(char* str) {
    return find(root, str) != 0;
}

// String counting function, returns the total number of string in the current Radix Tree
int RadixTree::countStrings() {
    return countStringsAux(root);
}

// Node counting function, returns the total number of nodes in the current Radix Tree
int RadixTree::countNodes() {
    return countNodesAux(root);
}

// Tree sorting function, sorts the nodes of the current Radix Tree alphabetically in ascending order
void RadixTree::sortRadixTree() {
    root = sortRadixTreeAux(root);
}

// String fetching function, returns all strings that can be found in current Radix Tree, has the option to sort them or not
char** RadixTree::fetchStrings(bool echo, bool sort) {

    if (echo) cout << "String Count: " << countStrings() << "\nNote: Duplicate strings are prohibited in the Radix Tree.\n\n";

    char** strings = fetchStringsAux(root, echo, sort);

    return strings;

}

// String printing function, prints strings in tree sorted in alphabetical order
void RadixTree::sortAndPrintStrings(const char* address, bool echo) {

    if (*address) segmentsFile.open(address);

    int stringsNum = countStrings();

    if (segmentsFile) segmentsFile << "String Count: " << stringsNum << "\nNote: Duplicate strings are prohibited in the Radix Tree.\n\n";
    if (echo) cout << "String Count: " << stringsNum << "\nNote: Duplicate strings are prohibited in the Radix Tree.\n\n";

    sortAndPrintStringsAux(root, echo);

    if (*address) segmentsFile.close();

}

// Node printing function, prints node count, individual nodes, with their respective prefixes
void RadixTree::printNodes(const char* address, bool echo) {

    if (*address) nodesFile.open(address);

    int nodesNum = countNodes();

    if (nodesFile) nodesFile << "Node Count: " << nodesNum << "\n";
    if (echo) cout << "Node Count: " << nodesNum << "\n";

    if (nodesFile) nodesFile << "Note: For nodes with prefixes, the prefix is printed before the node key and they are separated by the \"|\" character.\n\n";
    if (echo) cout << "Note: For nodes with prefixes, the prefix is printed before the node key and they are separated by the \"|\" character.\n\n";

    printNodesAux(root, echo);

    if (*address) nodesFile.close();

}

// Tree visualization function, prints a visualization of the entire radix tree
void RadixTree::printTree(const char* address, bool echo) {

    if (*address) treeFile.open(address);

    // The auxiliary tree printing function takes different inputs:
    // The root node of the current Radix Tree and the prefix data length (explained in detail inside the function)
    printTreeAux(root, 0, echo);

    if (*address) treeFile.close();

}

/*  ┌ Example on Recursive Tree Merge Sorting ┐
┌───┴─────────────────────────────────────────┴─────────────────────────────────────────┐
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 0, Step 0 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Everything initialized, head has more than one sibling, proceed to step 1             │
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 0, Step 1 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
├─── The three "^"s used next stand for the three pointers "head", "mid", and "last" ───┤
│                                                                                       │
│ [271][612][312][103][212][256][861][273][982]                                         │
│  ^^^                                                                                  │
│                                                                                       │
│ [271][612][312][103][212][256][861][273][982]                                         │
│   ^    ^    ^                                                                         │
│                                                                                       │
│ [271][612][312][103][212][256][861][273][982]                                         │
│   ^         ^         ^                                                               │
│                                                                                       │
│ [271][612][312][103][212][256][861][273][982]                                         │
│   ^              ^              ^                                                     │
│                                                                                       │
│ [271][612][312][103][212][256][861][273][982]                                         │
│   ^                   ^                   ^                                           │
│                                                                                       │
├───────────────── mid is now [212], t2 = sortRadixTreeAux(mid─>next) ──────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 1, Step 0 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Everything initialized, head has more than one sibling, proceed to step 1             │
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 1, Step 1 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ [256][861][273][982]                                                                  │
│  ^^^                                                                                  │
│                                                                                       │
│ [256][861][273][982]                                                                  │
│   ^    ^    ^                                                                         │
│                                                                                       │
├───────────────── mid is now [861], t2 = sortRadixTreeAux(mid─>next) ──────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 2, Step 0 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Everything initialized, head has more than one sibling, proceed to step 1             │
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 2, Step 1 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ [273][982]                                                                            │
│  ^^^                                                                                  │
│                                                                                       │
├───────────────── mid is now [273], t2 = sortRadixTreeAux(mid─>next) ──────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 3, Step 0 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Head ([982]) does not have a sibling, therefore we evaluate its children              │
│ Let's assume that it does have a child that points to the list: [281][882][821][481]  │
│                                                                                       │
├──────────────────────────── sortRadixTreeAux(head->link); ────────────────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 4, Step 0 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Everything initialized, head has more than one sibling, proceed to step 1             │
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 4, Step 1 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│                                                                                       │
│ [281][882][821][481]                                                                  │
│  ^^^                                                                                  │
│                                                                                       │
│ [281][882][821][481]                                                                  │
│   ^    ^    ^                                                                         │
│                                                                                       │
├───────────────── mid is now [882], t2 = sortRadixTreeAux(mid─>next) ──────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 5, Step 0 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Everything initialized, head has more than one sibling, proceed to step 1             │
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 5, Step 1 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ [821][481]                                                                            │
│  ^^^                                                                                  │
├───────────────── mid is now [821], t2 = sortRadixTreeAux(mid─>next) ──────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 6, Step 0 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Head ([481]) does not have a sibling, therefore we evaluate its children              │
│ Let's assume that it does not have any children either                                │
│                                                                                       │
├───────────────────────────────────── return head; ────────────────────────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────── RESUME: Stack Level 5, Step 1 ────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
├──── Now "t2" points towards [481], mid─>next = NULL; t1 = sortRadixTreeAux(head); ────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 6, Step 0 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Head ([821]) does not have a sibling, therefore we evaluate its children              │
│ Let's assume that it does not have any children either                                │
│                                                                                       │
├───────────────────────────────────── return head; ────────────────────────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────── RESUME: Stack Level 5, Step 1 ────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
├─────────────── Now "t1" points towards [821], we now proceed to step 2 ───────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 5, Step 2 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
├───── Now the function proceeds to find the new head of the to-be-merged sub-list ─────┤
│                                                                                       │
│ Let's assume the two nodes were not [821] and [481], but rather ["abcd"] and ["abed"] │
│ for the sake of having a more accurate demonstration, then afterwards we'll step back │
│ to the original example as it is simpler to demonstrate                               │
│                                                                                       │
│ First we check for null pointers, and neither of them are                             │
│ Both of them have length 4 (3 characters + null terminator), so we have 4 iterations  │
│ 1st iteration: a = a, do nothing and continue iterating                               │
│ 2nd iteration: b = b, do nothing and continue iterating                               │
│ 3rd iteration: c < e, "newHead" = "t1"                                                │
│ And since "newHead" now has a value, the loop does not undergo its 4th iteration      │
│                                                                                       │
│ Now back to our main example, [481] < [821], therefore the new head would be "t2"     │
│                                                                                       │
│ Now we evaluate the conditional (newHead == t1), which is false, therefore:           │
│ "t1" stays the same while "t2" is incremented, making it now point to nothing         │
│                                                                                       │
│ t1 -> [821]                                                                           │
│ t2 -> [NULL]                                                                          │
│ Meanwhile, "newHead" is pointing at what "t2" pointed at before being incremented     │
│ i.e. newHead -> [481]                                                                 │
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 5, Step 3 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Now we introduce "temp" whose purpose is to determine the rest of the sorted sub-list │
│ by setting the "next" of each node coming after "newHead" as "newHead" stays the same │
│                                                                                       │
│ At this point, "t2" is a null pointer, therefore the loop doesn't even begin          │
│ Therefore we check "temp->next = (t1 ? t1 : t2;)" which leads to: "temp->next = t1;"  │
│                                                                                       │
│ Knowing that "temp" was pointing at "newHead", then we have the sub-list: [481][821]  │
│                                                                                       │
├───────────────── Sub-list is now sorted and merged - return newHead; ─────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────── RESUME: Stack Level 4, Step 1 ────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Recap: We were awaiting "t2 = sortRadixTree(mid->next)", and it just finished above   │
│        So now we have t2 -> [481][821]                                                │
│        The full list was [281][882][821][481], now [281][882][481][821]               │
│        With [281] being the head, we proceed with the following:                      │
│                                                                                       │
├─────────────────── mid─>next = NULL;  t1 = sortRadixTreeAux(head); ───────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 5, Step 0 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Everything initialized, head has more than one sibling, proceed to step 1             │
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 5, Step 1 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ [281][882]                                                                            │
│  ^^^                                                                                  │
├───────────────── mid is now [281], t2 = sortRadixTreeAux(mid─>next) ──────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 6, Step 0 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Head ([882]) does not have a sibling, therefore we evaluate its children              │
│ Let's assume that it does not have any children either                                │
│                                                                                       │
├───────────────────────────────────── return head; ────────────────────────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────── RESUME: Stack Level 5, Step 1 ────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
├──── Now "t2" points towards [882], mid─>next = NULL; t1 = sortRadixTreeAux(head); ────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 6, Step 0 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Head ([281]) does not have a sibling, therefore we evaluate its children              │
│ Let's assume that it does not have any children either                                │
│                                                                                       │
├───────────────────────────────────── return head; ────────────────────────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────── RESUME: Stack Level 5, Step 1 ────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
├─────────────── Now "t1" points towards [281], we now proceed to step 2 ───────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 5, Step 2 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ [281] < [882], therefore the new head would be "t1", additionally...                  │
│ "t2" stays the same while "t1" is incremented, making it now point to nothing         │
│                                                                                       │
│ t1 -> [NULL]                                                                          │
│ t2 -> [882]                                                                           │
│ Meanwhile, "newHead" is pointing at what "t1" pointed at before being incremented     │
│ i.e. newHead -> [281]                                                                 │
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 5, Step 3 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ "temp->next = (t1 ? t1 : t2;)" leads to: "temp->next = t2;"                           │
│                                                                                       │
│ Knowing that "temp" was pointing at "newHead", then we have the sub-list: [281][882]  │
│                                                                                       │
├───────────────── Sub-list is now sorted and merged - return newHead; ─────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────── RESUME: Stack Level 4, Step 1 ────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Now that we have t1 -> [281][882] and t2 -> [481][821], we may proceed to step 2      │
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 4, Step 2 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ No null pointers, proceed to first node comparison for head determination             │
│ Since [281] < [481], then "newHead" will now point to "t1" i.e. [281][882]            │
│ Afterwards, "t1" gets incremented, i.e. t1 -> [882]                                   │
│ Meanwhile, "t2" is still the same, i.e. t2 -> [481][821]                              │
│                                                                                       │
│ Now that we have "newHead" we proceed to step 3                                       │
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 4, Step 3 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ This time, "t1" and "t2" are NOT null pointers, so the loop can begin                 │
│ As written in the code each node is compared and placed in "temp" based on the result │
│                                                                                       │
│ Since [481] < [882], then [481] gets placed next (i.e. Sub-list is now [281][481]...) │
│ Then "temp" points at what "t2" points at currently, i.e. [481]                       │
│ ^ This is the same as saying "temp" gets incremented as its next was just set as "t2" │
│ Then "t2" gets incremented (since we took its first node), so now it points at [821]  │
│                                                                                       │
│ Both pointers are still not null so we proceed with the loop                          │
│ [821] < [882], then [821] is put in "temp" (i.e. Sub-list is now [281][481][821]...)  │
│ "temp->next = t2; temp = t2; t2 = t2->next;", making t2 -> [NULL]                     │
│                                                                                       │
│ Now the loop stops and we proceed to evaluate "temp->next = (t1 ? t1 : t2);"          │
│ Since "t2" was the one to go null first, "temp->next" takes "t1" (i.e. [882]          │
│                                                                                       │
│ Therefore now we have "temp" setting the last element in the sub-list, which now is:  │
│ [281][481][821][882] which is also what "newHead" is point at, therefore...           │
│                                                                                       │
├───────────────── Sub-list is now sorted and merged - return newHead; ─────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────── RESUME: Stack Level 3, Step 0 ────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ We have now successfully sorted the children of head [982] back from stack level 3    │
│ We may now proceed with this step and...                                              │
│                                                                                       │
├───────────────────────────────────── return head; ────────────────────────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────── RESUME: Stack Level 2, Step 1 ────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
├──── Now "t2" points towards [982], mid─>next = NULL; t1 = sortRadixTreeAux(head); ────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────────── Stack Level 3, Step 0 ────────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Head ([273]) does not have a sibling, therefore we evaluate its children              │
│ Let's assume that it does not have any children either                                │
│                                                                                       │
├───────────────────────────────────── return head; ────────────────────────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────── RESUME: Stack Level 2, Step 1 ────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
├──────────────────────────── Now "t1" points towards [273] ────────────────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├────────────────────────────── Stack Level 2, Steps 2, 3 ──────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ To avoid redundancy in this example, I will just display the final results            │
│ newHead = t1 = [273]...                                                               │
│ t1 = t1->next = [NULL]                                                                │
│ t2 = [982]                                                                            │
│ temp = newHead = [273]...                                                             │
│ temp->next = t2 = [982]                                                               │
│                                                                                       │
│ Therefore newHead -> [273][982]                                                       │
│                                                                                       │
├───────────────── Sub-list is now sorted and merged - return newHead; ─────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────── RESUME: Stack Level 1, Step 1 ────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
├──────── Now, t2 -> [273][982], mid─>next = NULL; t1 = sortRadixTreeAux(head); ────────┤
│                                                                                       │
│ Again, to avoid redundancy, let's directly display results & proceed to the next step │
│ t1 -> [256][861]                                                                      │
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├────────────────────────────── Stack Level 1, Steps 2, 3 ──────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Same as in the previous two steps, displaying the results directly (step-by-step)     │
│                                                                                       │
│ newHead = t1 = [256]...                                                               │
│ t1 = t1->next = [861]                                                                 │
│ t2 = [273][982]                                                                       │
│ temp = newHead = [256]...                                                             │
│                                                                                       │
│ Since [273] < [861]: temp->next = t2 = [273] (i.e. now newHead = [256][273]...)       │
│                      Increment temp and t2, so temp = [273], t2 = [982]               │
│                                                                                       │
│ Since [861] < [982]: temp->next = t1 = [861] (i.e. now newHead = [256][273][861]...)  │
│                      Increment temp and t1, so temp = [861], t1 = [NULL]              │
│                                                                                       │
│ "temp->next = (t1 ? t1 : t2);" leads to temp->next = t2 = [982]                       │
│                                                                                       │
│ Therefore newHead -> [256][273][861][982]                                             │
│                                                                                       │
├───────────────── Sub-list is now sorted and merged - return newHead; ─────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├──────────────────────────── RESUME: Stack Level 0, Step 1 ────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ Reminder that the original list was [271][612][312][103] [212] [256][861][273][982]   │
│                                                          (mid)                        │
│                                                                                       │
├─── Now, t2 -> [256][273][861][982], mid->next = NULL; t1 = sortRadixTreeAux(head); ───┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├────────────────────────────── Stack Level 1 - All Steps ──────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ We're now about to repeat everything for [271][612][312][103][212]                    │
├──── Let's assume that none of them have children to avoid redundancy - once again ────┤
│                                                                                       │
│ [271][612][312][103][212]                                                             │
│   ^         ^         ^                                                               │
│                                                                                       │
│ Gets split into [271][612][312] and [103][212]                                        │
│ Which in turn each get split into [271][612], [312] and [103], [212]                  │
│ Which in turn get split further into ([271], [612]), [312] and [103], [212]           │
│ And as seen previously, each sub-list then gets sorted and simultaneously merged      │
│ So [103] and [212] become [103][212]                                                  │
│                                                                                       │
│ But what about ([271], [612]), [312] ? Let's dive into it                             │
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ As for [312], it is returned directly as it has no siblings or children               │
│ As for [271] and [612], they undergo the same process as before and become [273][612] │
│                                                                                       │
│ t2 -> [312], t1 -> [271][612]                                                         │
│ newHead = t1 = [271]... and afterwards t1 increments, becoming [612]                  │
│                                                                                       │
│ temp = newHead = [271]...                                                             │
│ [312] < [612], so temp->next = t2 = [312] and afterwards temp and t2 increment        │
│ i.e. temp = [312], t2 = [NULL] <-- a NULL breaks the loop                             │
│                                                                                       │
│ Therefore "temp->next = (t1 ? t1 : t2);" leads to temp->next = t1 = [612]             │
│                                                                                       │
│ Therefore newHead -> [271][312][612]                                                  │
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ And now [271][312][612] gets merge-sorted with [103][212]...                          │
│                                                                                       │
│ t2 -> [103][212], t1 -> [271][312][612]                                               │
│ newHead = t2 = [103]... and afterwards t2 increments, becoming [212]                  │
│                                                                                       │
│ temp = newHead = [103]...                                                             │
│ [212] < [271], so temp->next = t2 = [212] and afterwards temp and t2 increment        │
│ i.e. temp = [212], t2 = [NULL] <-- a NULL breaks the loop                             │
│                                                                                       │
│ Therefore "temp->next = (t1 ? t1 : t2);" leads to temp->next = t1 = [271][312][612]   │
│ Notice that "t1" in its entirety (3 nodes) get added in the list in this step         │
│                                                                                       │
│ Therefore newHead -> [103][212][271][312][612]                                        │
│                                                                                       │
├───────────────── Sub-list is now sorted and merged - return newHead; ─────────────────┤
│                                                                                       │
├───────────────────────────────────────────────────────────────────────────────────────┤
├────────────────────────────── Stack Level 0, Steps 2, 3 ──────────────────────────────┤
├───────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                       │
│ We now have t2 -> [256][273][861][982]                                                │
│             t1 -> [103][212][271][312][612]                                           │
│                                                                                       │
│ Since merge-sorting has now been demonstrated multiple times, I will only display...  │
│ ...the step-by-step updates of "newHead"                                              │
│                                                                                       │
│ [103] < [256] --> newHead = [103]                                                     │
│ [212] < [256] --> newHead = [103][212]                                                │
│ [256] < [271] --> newHead = [103][212][256]                                           │
│ [271] < [273] --> newHead = [103][212][256][271]                                      │
│ [273] < [312] --> newHead = [103][212][256][271][273]                                 │
│ [312] < [861] --> newHead = [103][212][256][271][273][312]                            │
│ [612] < [861] --> newHead = [103][212][256][271][273][312][612]                       │
│ t1 went NULL! --> newHead = [103][212][256][271][273][312][612]<t2>                   │
│              i.e. newHead = [103][212][256][271][273][312][612][861][982]             │
│                                                                                       │
├───────────────── Sub-list is now sorted and merged - return newHead; ─────────────────┤
│                                                                                       │
│ Sorting is now over, this is the result:                                              │
│                                                                                       │
│ [103][212][256][271][273][312][612][861][982]                                         │
│                                           │                                           │
│                                           └──[281][481][821][882]                     │
│                                                                                       │
└───────────────────────────────────────────────────────────────────────────────────────┘
*/

/*  ┌ Example on Recursive Tree Printing with Dynamic Memory Allocation ┐
┌───┴───────────────────────────────────────────────────────────────────┴────────────────────────────────────────┐
│                                                                                                                │
│  A test that matches the assumptions made in this example can be generated using the following setup:          │
│  ─ Randomization Seed: 1337                                                                                    │
│  ─ Min. Segment length: 4                                                                                      │
│  ─ Max. Segment length: 7                                                                                      │
│  ─ Number of segments: 10                                                                                      │
│                                                                                                                │
│  Upon executing "printTreeAux", first, we have pLen set at 0, and "static bool* p, * temp;" which gives us:    │
│  p ──> [], temp ──> [], pLen = 0                                                                               │
│                                                                                                                │
│  They will not be instantiated again for the lifetime of this program.                                         │
│  Now let's proceed with the function, skip the part concerning printing...                                     │
│                                                                                                                │
│  "temp" is resized to have size pLen (= 0 at this step)                                                        │
│  temp ──> [?], pLen = 0                                                                                        │
│                                                                                                                │
│  The loop sets the first "pLen" characters in "temp" as "p" (= 0 at this step, i.e. no changes), so:           │
│  temp ──> [?], pLen = 0                                                                                        │
│                                                                                                                │
│  At this point, "p" is no longer needed, so we de─allocate its memory                                          │
│  p ──> garbage, pLen = 0                                                                                       │
│                                                                                                                │
│  Now we allocate "pLen + 1 (= 1 at this step)" bool slots for p                                                │
│  p ──> [?], pLen = 0                                                                                           │
│                                                                                                                │
│  Next, we copy "pLen" bool values from "temp" into "p" then add a true or false according to the situation     │
│  p ──> [true], pLen = 0                                                                                        │
│                                                                                                                │
│  And at this point, "temp" is no longer needed, so we de─allocate its memory                                   │
│  temp ──> garbage, pLen = 0                                                                                    │
│                                                                                                                │
│  The value of pLen makes sense at this point, since for this function (the root function), there's no prefix   │
│  That is because root nodes have no prefix tree symbols before the "├───"                                      │
│  Notice how the prefix itself, however, is actually of length 1                                                │
│  Hence, upon proceeding to a child, we send it "pLen + 1"                                                      │
│                                                                                                                │
├────────────────────────────────── Assuming a child does exist, we will proceed ────────────────────────────────┤
├──────────────────── Here we recurse, sending the function the first child and pLen + 1 (= 1) ──────────────────┤
│                                                                                                                │
│  First, we now have:                                                                                           │
│  p ──> [true], temp ──> garbage, pLen = 1 (which means 1 character set got printed)                            │
│                                                                                                                │
│  We now allocate new memory for the "temp" array with size pLen (= 1 at this step)                             │
│  temp ──> [?], pLen = 1                                                                                        │
│                                                                                                                │
│  The loop sets the first "pLen" bool values in "temp" as "p", so:                                              │
│  temp ──> [true], pLen = 1                                                                                     │
│                                                                                                                │
│  At this point, "p" is no longer needed, so we de─allocate its memory                                          │
│  temp ──> garbage, pLen = 1                                                                                    │
│                                                                                                                │
│  Next we allocate "pLen + 1 (= 2 at this step)" characters for p                                               │
│  p ──> [?][?], pLen = 1                                                                                        │
│                                                                                                                │
│  Next, we copy "pLen" bool values from "temp" into "p" then add a true or false according to the situation     │
│  If next node exists, we choose "true", otherwise we choose "false". This will be further explained later      │
│  Let's assume the next node (sibling node) exists, so...                                                       │
│  p ──> [true][true], pLen = 1                                                                                  │
│                                                                                                                │
│  And at this point, "temp" is no longer needed, so we de─allocate its memory                                   │
│  temp ──> garbage, pLen = 1                                                                                    │
│                                                                                                                │
│  Again, the value of pLen for this function makes sense, as this is the root's child, i.e. 2nd tree level      │
│                                                                                                                │
│  ───────────── Assuming there is no child, the function skips the first recursive function call ───────────────┤
│  ───────────────────────── Following the assumption made above, there's a next node ───────────────────────────┤
│  ────────── Therefore we recurse again, sending the function the next node and the same pLen (= 1) ────────────┤
│                                                                                                                │
│  First, we now have:                                                                                           │
│  p ──> [true][true], temp ──> garbage, pLen = 1                                                                │
│                                                                                                                │
│  Same as before, allocate memory for "temp" of size "pLen " then copy "pLen" characters from "p"               │
│  temp ──> [?], pLen = 1                                                                                        │
│  temp ──> [true], pLen = 1                                                                                     │
│                                                                                                                │
│  Then we delete "p", allocate "pLen + 1" characters, copy all of "temp" then add our choice for this iteration │
│  p ──> garbage, pLen = 1                                                                                       │
│  p ──> [?][?], pLen = 1                                                                                        │
│  p ──> [true][true], pLen = 1                                                                                  │
│                                                                                                                │
│  And at this point, "temp" is no longer needed, so we de─allocate its memory                                   │
│  temp ──> garbage, pLen = 1                                                                                    │
│                                                                                                                │
│  ─────────── Let's assume again that there's no child, and skip the first recursive function call ─────────────┤
│  ───────────── Assuming there is a next node, we recurse again with it, and the same pLen (= 1) ───────────────┤
│                                                                                                                │
│  The same exact execution as the previous non─empty function is done here again...                             │
│  ...except for one thing: Let's assume this node has no next node, meaning that...                             │
│  ...when choosing which special character set to append to the prefix, we'll go...                             │
│  ...with the four spaces, the following example should show both choices in action                             │
│                                                                                                                │
│          .                                                                                                     │
│          .                                                                                                     │
│          .                                                                                                     │
│                                                                                                                │
│  │   │   ├───TC... <── Choosing true, i.e. "│   "                                                              │
│  │   │   └───AT... <── Choosing true, i.e. "│   "                                                              │
│  │   └───GCC                                                                                                   │
│  │       ├───AA... <── Choosing false, i.e. "    "                                                             │
│  │       └───GT... <── Choosing false, i.e. "    "                                                             │
│  ├───T                                                                                                         │
│  │   ├───ATCTGT... <── Choosing true, i.e. "│   "                                                              │
│  │   ├───CATAAC... <── Choosing true, i.e. "│   "                                                              │
│                                                                                                                │
│          .                                                                                                     │
│          .                                                                                                     │
│          .                                                                                                     │
│                                                                                                                │
│  i.e. The four spaces are for the case that there's no next node where there could potentially be children     │
│                                                                                                                │
│  After the current function is over, we will have the following values:                                        │
│                                                                                                                │
│  p ──> [true][false], temp ──> garbage, pLen = 1                                                               │
│                                                                                                                │
│  This function then terminates, popping the stack and allowing its parent function to continue execution       │
│  Its parent (@pLen = 0) was waiting for its first recursive call to finish, and now commences the second       │
│  Since we had assumed it had a sibling node, it will go through the second recursive call                      │
│                                                                                                                │
│  This recursive cycle repeats until all siblings and children have their prefixes chosen                       │
│                                                                                                                │
└────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
*/