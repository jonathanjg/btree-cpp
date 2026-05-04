#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

template <typename T>
class BTreeNode {
public:
    bool isLeaf;
    int t; // minimum degree
    std::vector<T> keys;
    std::vector<BTreeNode<T>*> children;

    BTreeNode(int minDegree, bool leaf)
        : isLeaf(leaf), t(minDegree) {}

    ~BTreeNode() {
        for (auto* child : children) {
            delete child;
        }
    }

    void traverse() const {
        size_t i;
        for (i = 0; i < keys.size(); i++) {
            if (!isLeaf) {
                children[i]->traverse();
            }
            std::cout << keys[i] << " ";
        }

        if (!isLeaf) {
            children[i]->traverse();
        }
    }

    BTreeNode<T>* search(const T& k) {
        int i = 0;

        while (i < static_cast<int>(keys.size()) && k > keys[i]) {
            i++;
        }

        if (i < static_cast<int>(keys.size()) && keys[i] == k) {
            return this;
        }

        if (isLeaf) {
            return nullptr;
        }

        return children[i]->search(k);
    }

    void insertNonFull(const T& k) {
        int i = static_cast<int>(keys.size()) - 1;

        if (isLeaf) {
            // Make space for the new key
            keys.push_back(k);

            while (i >= 0 && keys[i] > k) {
                keys[i + 1] = keys[i];
                i--;
            }

            keys[i + 1] = k;
        } else {
            while (i >= 0 && keys[i] > k) {
                i--;
            }

            i++;

            if (children[i]->keys.size() == static_cast<size_t>(2 * t - 1)) {
                splitChild(i, children[i]);

                if (k > keys[i]) {
                    i++;
                }
            }

            children[i]->insertNonFull(k);
        }
    }

    void splitChild(int i, BTreeNode<T>* y) {
        auto* z = new BTreeNode<T>(y->t, y->isLeaf);

        // Save median key before resizing
        T median = y->keys[t - 1];

        // Move last (t-1) keys of y into z
        for (int j = t; j < 2 * t - 1; j++) {
            z->keys.push_back(y->keys[j]);
        }

        // Move children if y is not a leaf
        if (!y->isLeaf) {
            for (int j = t; j < 2 * t; j++) {
                z->children.push_back(y->children[j]);
            }
        }

        // Shrink y
        y->keys.resize(t - 1);
        if (!y->isLeaf) {
            y->children.resize(t);
        }

        // Insert new child into this node
        children.insert(children.begin() + i + 1, z);

        // Insert median key into this node
        keys.insert(keys.begin() + i, median);
    }

    void printStructure(int level = 0) const {
        std::cout << std::string(level * 4, ' ') << "[";

        for (size_t i = 0; i < keys.size(); i++) {
            std::cout << keys[i];
            if (i + 1 < keys.size()) {
                std::cout << " | ";
            }
        }

        std::cout << "]";
        if (isLeaf) {
            std::cout << " (leaf)";
        }
        std::cout << "\n";

        if (!isLeaf) {
            for (const auto* child : children) {
                child->printStructure(level + 1);
            }
        }
    }

    int exportDot(std::ofstream& out, int& nodeCounter) const {
        int myId = nodeCounter++;

        out << "  node" << myId << " [label=\"";
        for (size_t i = 0; i < keys.size(); i++) {
            out << keys[i];
            if (i + 1 < keys.size()) out << " | ";
        }
        out << "\"];\n";

        if (!isLeaf) {
            for (const auto* child : children) {
                int childId = child->exportDot(out, nodeCounter);
                out << "  node" << myId << " -> node" << childId << ";\n";
            }
        }

        return myId;
    }
};

template <typename T>
class BTree {
public:
    BTreeNode<T>* root;
    int t;

    BTree(int minDegree)
        : root(nullptr), t(minDegree) {}

    ~BTree() {
        delete root;
    }

    void traverse() const {
        if (root) {
            root->traverse();
        }
        std::cout << "\n";
    }

    BTreeNode<T>* search(const T& k) {
        return root ? root->search(k) : nullptr;
    }

    void insert(const T& k) {
        if (!root) {
            root = new BTreeNode<T>(t, true);
            root->keys.push_back(k);
            return;
        }

        if (root->keys.size() == static_cast<size_t>(2 * t - 1)) {
            auto* s = new BTreeNode<T>(t, false);
            s->children.push_back(root);

            s->splitChild(0, root);

            int i = 0;
            if (k > s->keys[0]) {
                i++;
            }

            s->children[i]->insertNonFull(k);
            root = s;
        } else {
            root->insertNonFull(k);
        }
    }

    void printStructure() const {
        if (!root) {
            std::cout << "(empty tree)\n";
            return;
        }

        root->printStructure();
    }

    void exportToDot(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) {
            std::cerr << "Failed to open file: " << filename << "\n";
            return;
        }

        out << "digraph BTree {\n";
        out << "  node [shape=record];\n";
        out << "  rankdir=TB;\n";

        if (root) {
            int nodeCounter = 0;
            root->exportDot(out, nodeCounter);
        }

        out << "}\n";
    }
};