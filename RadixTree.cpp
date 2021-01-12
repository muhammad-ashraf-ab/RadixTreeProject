//
// Created by Ahmed Bakry on 1/11/2021.
//

#include "RadixTree.h"

int RadixTree::prefix(char* x, int n, char* key, int m)
{
    for (int k = 0; k < n; k++)
        if (k == m || x[k] != key[k])
            return k;
    return n;
}

RadixTree::Node* RadixTree::find(Node* t, char* x, int n = 0) // x key search in t tree 
{
    if (!n) n = strlen(x) + 1;
    if (!t) return 0;
    int k = prefix(x, n, t->key, t->len);
    if (k == 0) return find(t->next, x, n); // let's look for the child's node 
    if (k == n) return t;
    if (k == t->len) return find(t->link, x + k, n - k); // let's look for at the child's node 
    return 0;
}

void RadixTree::split(Node* t, int k) // dividing t node according to k key symbol 
{
    Node* p = new Node(t->key + k, t->len - k);
    p->link = t->link;
    t->link = p;
    char* a = new char[k];
    strncpy(a, t->key, k);
    delete[] t->key;
    t->key = a;
    t->len = k;
}

RadixTree::Node* RadixTree::insert(Node* t, char* x, int n = 0) // inserting x key in t tree 
{
    if (!n) n = strlen(x) + 1;
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
    strncpy(a, t->key, t->len);
    strncpy(a + t->len, p->key, p->len);
    delete[] t->key;
    t->key = a;
    t->len += p->len;
    t->link = p->link;
    delete p;
}

RadixTree::Node* RadixTree::remove(Node* t, char* x, int n = 0) // deleting x key from t tree 
{
    if (!n) n = strlen(x) + 1;
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

void RadixTree::addString(char* str) {
    root = insert(root, str);
}

void RadixTree::deleteString(char* str) {
    root = remove(root, str);
}

bool RadixTree::searchString(char* str) {
    return find(root, str) != 0;
}
