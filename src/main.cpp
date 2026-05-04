#include "../include/BTree.hpp"

int main() {
    BTree<int> tree(3); // minimum degree = 3

    tree.insert(10);
    tree.insert(20);
    tree.insert(5);
    tree.insert(6);
    tree.insert(12);

    std::cout << "Traversal: ";
    tree.traverse();

    int key = 1;
    if (tree.search(key))
        std::cout << key << " found\n";
    else
        std::cout << key << " not found\n";

    return 0;
}