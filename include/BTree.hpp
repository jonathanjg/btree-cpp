#pragma once
#include <iostream>
#include <vector>

template <typename T>
class BTreeNode {
public:
    bool isLeaf;
    std::vector<T> keys;
    std::vector<BTreeNode*> children;
    int t; // minimum degree

    BTreeNode(int _t, bool _isLeaf) : t(_t), isLeaf(_isLeaf) {}

    void traverse() {
        int i;
        for (i = 0; i < keys.size(); i++) {
            if (!isLeaf)
                children[i]->traverse();
            std::cout << keys[i] << " ";
        }
        if (!isLeaf)
            children[i]->traverse();
    }

    BTreeNode* search(T k) {
        int i = 0;
        while (i < keys.size() && k > keys[i])
            i++;
        if (i < keys.size() && keys[i] == k)
            return this;
        if (isLeaf)
            return nullptr;
        return children[i]->search(k);
    }

    void insertNonFull(T k) {
        int i = keys.size() - 1;

        if (isLeaf) {
            keys.push_back(0); // dummy space
            while (i >= 0 && keys[i] > k) {
                keys[i+1] = keys[i];
                i--;
            }
            keys[i+1] = k;
        } else {
            while (i >= 0 && keys[i] > k)
                i--;
            i++;
            if (children[i]->keys.size() == 2*t - 1) {
                splitChild(i, children[i]);
                if (keys[i] < k)
                    i++;
            }
            children[i]->insertNonFull(k);
        }
    }

    void splitChild(int i, BTreeNode* y) {
        BTreeNode* z = new BTreeNode(y->t, y->isLeaf);
        int t_minus_1 = t - 1;

        for (int j = 0; j < t_minus_1; j++)
            z->keys.push_back(y->keys[j+t]);
        if (!y->isLeaf) {
            for (int j = 0; j < t; j++)
                z->children.push_back(y->children[j+t]);
        }

        y->keys.resize(t_minus_1);
        y->children.resize(y->isLeaf ? 0 : t);

        children.insert(children.begin()+i+1, z);
        keys.insert(keys.begin()+i, y->keys[t-1]);
    }
};

template <typename T>
class BTree {
public:
    BTreeNode<T>* root;
    int t;

    BTree(int _t) : root(nullptr), t(_t) {}

    void traverse() {
        if (root) root->traverse();
        std::cout << "\n";
    }

    BTreeNode<T>* search(T k) {
        return root ? root->search(k) : nullptr;
    }

    void insert(T k) {
        if (!root) {
            root = new BTreeNode<T>(t, true);
            root->keys.push_back(k);
        } else {
            if (root->keys.size() == 2*t - 1) {
                BTreeNode<T>* s = new BTreeNode<T>(t, false);
                s->children.push_back(root);
                s->splitChild(0, root);
                int i = (s->keys[0] < k) ? 1 : 0;
                s->children[i]->insertNonFull(k);
                root = s;
            } else {
                root->insertNonFull(k);
            }
        }
    }
};