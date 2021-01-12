//
// Created by Ahmed Bakry on 1/11/2021.
//

#include "RadixTree.h"

int RadixTree::prefix(char* x, int n, char* key, int m)
{
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

RadixTree::Node* RadixTree::find(Node* t, char* x, int n)
{
    // if n is uninitialized, its value is set as the size of "x" (INCLUDING null character, i.e. size of "abc" is 4)
    // this is consistent with the expected value of the "len" member of each node: number of character, null included
    if (!n) while (x[n++]);

    // if provided tree node "t" is null, then there is nothing to be found, we have reached the end of this branch
    if (!t) return 0;

    // otherwise, if node has value, find the common prefix between it and the key being searched for, "x"
    // the prefix function is provided the size of both character arrays INCLUDING the null character
    int k = prefix(x, n, t->key, t->len);

    // if there's nothing in common, repeat the process for the next child
    if (k == 0) return find(t->next, x, n);

    // if the entirety of "x" is a prefix of the current node, return it, this is the node we're looking for
    if (k == n) return t;

    // if the entirety of the current node is a prefix itself...
    // this means that the string we're looking for could cover more than one node...
    // therefore, continue searching for the remaining part of the searched-for value... how?
    // "x + k" makes us skip ahead the prefix found and search for the rest of the string, meanwhile...
    // "n - k" is the size of the rest of the string.
    if (k == t->len) return find(t->link, x + k, n - k);
    return 0;
}

void RadixTree::split(Node* t, int k) // dividing t node according to k key symbol 
{
    Node* p = new Node(t->key + k, t->len - k);
    p->link = t->link;
    t->link = p;

    char* a = new char[k];
    for (int i = 0; i < k; i++) a[i] = t->key[i];

    delete[] t->key;
    t->key = a;
    t->len = k;
}

RadixTree::Node* RadixTree::insert(Node* t, char* x, int n) // inserting x key in t tree 
{
    if (!n) while (x[n++]);
    if (!t) return new Node(x, n);
    int k = prefix(x, n, t->key, t->len);
    if (k == 0) t->next = insert(t->next, x, n);
    else if (k < n)
    {
        if (k < t->len) // cut or not to cut?
            split(t, k);
        t->link = insert(t->link, x + k, n - k);
    }
    return t;
}

void RadixTree::join(Node* t) // t and t->link nodes merge
{
    Node* p = t->link;
    char* a = new char[t->len + p->len];
    //strncpy(a, t->key, t->len);
    for (int i = 0; i < t->len; i++) a[i] = t->key[i];


    //strncpy(a + t->len, p->key, p->len);
    for (int i = 0; i < p->len; i++) (a + t->len)[i] = p->key[i];

    delete[] t->key;
    t->key = a;
    t->len += p->len;
    t->link = p->link;
    delete p;
}

RadixTree::Node* RadixTree::remove(Node* t, char* x, int n) // deleting x key from t tree 
{
    if (!n) while (x[n++]);
    if (!t) return 0;
    int k = prefix(x, n, t->key, t->len);
    if (k == n) // deleting a leaf
    {
        Node* p = t->next;
        delete t;
        return p;
    }
    if (k == 0) t->next = remove(t->next, x, n);
    else if (k == t->len)
    {
        t->link = remove(t->link, x + k, n - k);
        if (t->link && !t->link->next) // does t node have just one sister node?
            join(t);
    }
    return t;
}

// Addition function updates root with a new root containing the string to be added
void RadixTree::addString(char* str) {
    root = insert(root, str);
}

// Deletion function updates root with a new root that does not contain the string to be deleted
void RadixTree::deleteString(char* str) {
    root = remove(root, str);
}

// Searching function returns boolean value based on the result of the finder function
bool RadixTree::searchString(char* str) {
    return find(root, str) != 0;
}
