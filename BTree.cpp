// Searching a key on a B-tree in C++

#include "structs.h"
#include <iostream>
using namespace std;

class TreeNode {
    TreeNodeType **keys;
    int t;
    TreeNode **C;
    int n;
    bool leaf;

  public:
    TreeNode(int t1, bool leaf1) {
        t = t1;
        leaf = leaf1;

        keys = new TreeNodeType *[2 * t - 1];
        C = new TreeNode *[2 * t];

        n = 0;
    }

    void insertNonFull(TreeNodeType *k) {
        int i = n - 1;

        if (leaf == true) {
            while (i >= 0 && keys[i]->name > k->name) {
                keys[i + 1] = keys[i];
                i--;
            }

            keys[i + 1] = k;
            n = n + 1;
        } else {
            while (i >= 0 && keys[i]->name > k->name)
                i--;

            if (C[i + 1]->n == 2 * t - 1) {
                splitChild(i + 1, C[i + 1]);

                if (keys[i + 1]->name < k->name) i++;
            }
            C[i + 1]->insertNonFull(k);
        }
    }

    void splitChild(int i, TreeNode *y) {
        TreeNode *z = new TreeNode(y->t, y->leaf);
        z->n = t - 1;

        for (int j = 0; j < t - 1; j++)
            z->keys[j] = y->keys[j + t];

        if (y->leaf == false) {
            for (int j = 0; j < t; j++)
                z->C[j] = y->C[j + t];
        }

        y->n = t - 1;
        for (int j = n; j >= i + 1; j--)
            C[j + 1] = C[j];

        C[i + 1] = z;

        for (int j = n - 1; j >= i; j--)
            keys[j + 1] = keys[j];

        keys[i] = y->keys[t - 1];
        n = n + 1;
    }

    void traverse(void (*func)(TreeNodeType *)) {
        int i;
        for (i = 0; i < n; i++) {
            if (leaf == false) C[i]->traverse(func);
            func(keys[i]);
        }

        if (leaf == false) C[i]->traverse(func);
    }

    TreeNodeType *search(string k) {
        int i = 0;
        while (i < n && k > keys[i]->name)
            i++;

        if (i < n && keys[i]->name == k) return keys[i];

        if (leaf == true) return NULL;

        return C[i]->search(k);
    }

    friend class BTree;
};

class BTree {
    TreeNode *root;
    int t;

  public:
    BTree(int temp) {
        root = NULL;
        t = temp;
    }

    void traverse(void (*func)(TreeNodeType *)) {
        if (root != NULL) root->traverse(func);
    }

    TreeNodeType *search(string k) { return (root == NULL) ? NULL : root->search(k); }

    void insert(TreeNodeType *k) {
        if (root == NULL) {
            root = new TreeNode(t, true);
            root->keys[0] = k;
            root->n = 1;
        } else {
            if (root->n == 2 * t - 1) {
                TreeNode *s = new TreeNode(t, false);

                s->C[0] = root;

                s->splitChild(0, root);

                int i = 0;
                if (s->keys[0]->name < k->name) i++;
                s->C[i]->insertNonFull(k);

                root = s;
            } else
                root->insertNonFull(k);
        }
    }
};

struct Directory {
    Directory *parent;
    BTree *tree;
};