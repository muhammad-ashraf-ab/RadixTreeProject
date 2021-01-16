# Radix Tree Linked Implementation
This project was created for CSE_331 Data Structures And Algorithms course offered in Ain Shams University - Faculty of Engineering under the supervision of Dr. Ashraf Abdel Raouf

This project uses a linked implementation of radix trees and has been greatly influenced by the implementation found in the following link:
https://kukuruku.co/post/radix-trees/

# Problem Statement
## Radix Tree
In a regular radix tree each edge is assigned with some symbol. As for root nodes, they store some service(user) information. Thus, any route from a tree root to one of its leaves defines precisely only one string. This string is considered to be stored in the specified tree.

Implement a radix tree to store DNA segments. Assume that DNA encoding consists only of letters A, C, G, T. Here is an example of a DNA fragment:

**> CTGCACGTGTCCCTGAAGGCTTCCAGAGGAAGCTTTACA**

Segments can be between 10 and 100 characters long. You can use randomly generated strings for testing, although in reality there are dependencies in DNA sequences. No symbols other than A, C, G, T may appear in the data. Note that a valid fragment may also be a prefix of another valid fragment. Your implementation must have:

* A method to add a string.
* A method to delete a string.
* A method to search for a string.
* A method to find out how many strings are in the tree.
* A method to find out how many nodes are in the tree.
* A method to print out all of the strings in alphabetical order.
* A method to print all nodes in the tree and what prefixes they correspond to.
