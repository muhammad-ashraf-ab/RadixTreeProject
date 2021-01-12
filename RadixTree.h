//
// Created by Ahmed Bakry on 1/11/2021.
//

#ifndef RADIXTREEPROJECT_RADIXTREE_H
#define RADIXTREEPROJECT_RADIXTREE_H

#include <string.h>

class RadixTree {
private:
    class Node {
    public:
        char* key;
        int len;
        Node* link;
        Node* next;
        Node(char* x, int n) : len(n), link(0), next(0)
        {
            key = new char[n];
            strncpy(key, x, n);
        }
        ~Node() { delete[] key; }
    };
    Node* root;

    int prefix(char* x, int n, char* key, int m);
    Node* find(Node* t, char* x, int n = 0);
    void split(Node* t, int k);
    Node* insert(Node* t, char* x, int n = 0);
    void join(Node* t);
    Node* remove(Node* t, char* x, int n = 0);

public:
    RadixTree(): root(0) {};
    void addString(char* str);
    void deleteString(char* str);
    bool searchString(char* str);

};


#endif //RADIXTREEPROJECT_RADIXTREE_H
