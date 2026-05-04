#include <iostream>
#include "BTree.hpp"

int main() {
    BTree<int> tree(3); // minimum degree t = 3

    std::vector<int> values = {10, 20, 5, 6, 12, 30, 7, 17};

    for (int value : values) {
        tree.insert(value);
    }

    std::cout << "Sorted traversal: ";
    tree.traverse();

    std::cout << "\nTree structure:\n";
    tree.printStructure();

    int key = 6;
    std::cout << "\nSearch for " << key << ": ";
    if (tree.search(key)) {
        std::cout << "found\n";
    } else {
        std::cout << "not found\n";
    }
    tree.exportToDot("btree.dot");

    return 0;
}