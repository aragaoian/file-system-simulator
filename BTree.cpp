// Searching a key on a B-tree in childs++
#include <functional>

#include "structs.h"
#include <iostream>
using namespace std;

class TreeNode {
    TreeNodeType **keys;
    int t;
    TreeNode **childs;
    int n;
    bool leaf;

  public:
    TreeNode(int t1, bool leaf1) {
        t = t1;
        leaf = leaf1;

        keys = new TreeNodeType *[2 * t - 1];
        childs = new TreeNode *[2 * t];

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

            if (childs[i + 1]->n == 2 * t - 1) {
                splitchildshild(i + 1, childs[i + 1]);

                if (keys[i + 1]->name < k->name) i++;
            }
            childs[i + 1]->insertNonFull(k);
        }
    }

    int findKey(TreeNodeType *k) {
        int idx = 0;
        while (idx < n && keys[idx] < k)
            ++idx;
        return idx;
    }

    void deletion(TreeNodeType *k) {
        int idx = findKey(k);

        if (idx < n && keys[idx] == k) {
            if (leaf) {
                removeFromLeaf(idx);
            } else {
                removeFromNonLeaf(idx);
            }
        } else {
            if (leaf) {
                cout << "The key " << k << " is does not exist in the tree\n";
                return;
            }

            bool flag = ((idx == n) ? true : false);

            if (childs[idx]->n < t) fill(idx);

            if (flag && idx > n) {
                childs[idx - 1]->deletion(k);
            } else {
                childs[idx]->deletion(k);
            }
        }
        return;
    }

    // Remove from the leaf
    void removeFromLeaf(int idx) {
        for (int i = idx + 1; i < n; ++i)
            keys[i - 1] = keys[i];

        n--;

        return;
    }

    // Delete from non leaf node
    void removeFromNonLeaf(int idx) {
        TreeNodeType *k = keys[idx];

        if (childs[idx]->n >= t) {
            TreeNodeType *pred = getPredecessor(idx);
            keys[idx] = pred;
            childs[idx]->deletion(pred);
        }

        else if (childs[idx + 1]->n >= t) {
            TreeNodeType *succ = getSuccessor(idx);
            keys[idx] = succ;
            childs[idx + 1]->deletion(succ);
        }

        else {
            merge(idx);
            childs[idx]->deletion(k);
        }
        return;
    }

    TreeNodeType *getPredecessor(int idx) {
        TreeNode *cur = childs[idx];
        while (!cur->leaf)
            cur = cur->childs[cur->n];

        return cur->keys[cur->n - 1];
    }

    TreeNodeType *getSuccessor(int idx) {
        TreeNode *cur = childs[idx + 1];
        while (!cur->leaf)
            cur = cur->childs[0];

        return cur->keys[0];
    }

    void fill(int idx) {
        if (idx != 0 && childs[idx - 1]->n >= t)
            borrowFromPrev(idx);

        else if (idx != n && childs[idx + 1]->n >= t)
            borrowFromNext(idx);

        else {
            if (idx != n)
                merge(idx);
            else
                merge(idx - 1);
        }
        return;
    }

    // Borrow from previous
    void borrowFromPrev(int idx) {
        TreeNode *child = childs[idx];
        TreeNode *sibling = childs[idx - 1];

        for (int i = child->n - 1; i >= 0; --i)
            child->keys[i + 1] = child->keys[i];

        if (!child->leaf) {
            for (int i = child->n; i >= 0; --i)
                child->childs[i + 1] = child->childs[i];
        }

        child->keys[0] = keys[idx - 1];

        if (!child->leaf) child->childs[0] = sibling->childs[sibling->n];

        keys[idx - 1] = sibling->keys[sibling->n - 1];

        child->n += 1;
        sibling->n -= 1;

        return;
    }

    // Borrow from the next
    void borrowFromNext(int idx) {
        TreeNode *child = childs[idx];
        TreeNode *sibling = childs[idx + 1];

        child->keys[(child->n)] = keys[idx];

        if (!(child->leaf)) child->childs[(child->n) + 1] = sibling->childs[0];

        keys[idx] = sibling->keys[0];

        for (int i = 1; i < sibling->n; ++i)
            sibling->keys[i - 1] = sibling->keys[i];

        if (!sibling->leaf) {
            for (int i = 1; i <= sibling->n; ++i)
                sibling->childs[i - 1] = sibling->childs[i];
        }

        child->n += 1;
        sibling->n -= 1;

        return;
    }

    void merge(int idx) {
        TreeNode *child = childs[idx];
        TreeNode *sibling = childs[idx + 1];

        child->keys[t - 1] = keys[idx];

        for (int i = 0; i < sibling->n; ++i)
            child->keys[i + t] = sibling->keys[i];

        if (!child->leaf) {
            for (int i = 0; i <= sibling->n; ++i)
                child->childs[i + t] = sibling->childs[i];
        }

        for (int i = idx + 1; i < n; ++i)
            keys[i - 1] = keys[i];

        for (int i = idx + 2; i <= n; ++i)
            childs[i - 1] = childs[i];

        child->n += sibling->n + 1;
        n--;

        delete (sibling);
        return;
    }

    void splitchildshild(int i, TreeNode *y) {
        TreeNode *z = new TreeNode(y->t, y->leaf);
        z->n = t - 1;

        for (int j = 0; j < t - 1; j++)
            z->keys[j] = y->keys[j + t];

        if (y->leaf == false) {
            for (int j = 0; j < t; j++)
                z->childs[j] = y->childs[j + t];
        }

        y->n = t - 1;
        for (int j = n; j >= i + 1; j--)
            childs[j + 1] = childs[j];

        childs[i + 1] = z;

        for (int j = n - 1; j >= i; j--)
            keys[j + 1] = keys[j];

        keys[i] = y->keys[t - 1];
        n = n + 1;
    }

    void traverse(function<void(TreeNodeType *)> func) {
        int i;
        for (i = 0; i < n; i++) {
            if (leaf == false) childs[i]->traverse(func);
            func(keys[i]);
        }

        if (leaf == false) childs[i]->traverse(func);
    }

    TreeNodeType *search(string k) {
        int i = 0;
        while (i < n && k > keys[i]->name)
            i++;

        if (i < n && keys[i]->name == k) return keys[i];

        if (leaf == true) return NULL;

        return childs[i]->search(k);
    }

    int height(TreeNode *node) {
        if (leaf) return 0;
        int i;
        int maxHeight = 0;
        for (i = 0; i < n; i++) {
            maxHeight = max(maxHeight, height(childs[i]));
        }
        return 1 + maxHeight;
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

    void traverse(function<void(TreeNodeType *)> func) {
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

                s->childs[0] = root;

                s->splitchildshild(0, root);

                int i = 0;
                if (s->keys[0]->name < k->name) i++;
                s->childs[i]->insertNonFull(k);

                root = s;
            } else
                root->insertNonFull(k);
        }
    }

    void deletion(TreeNodeType *k) {
        if (!root) {
            cout << "The tree is empty\n";
            return;
        }

        root->deletion(k);

        if (root->n == 0) {
            TreeNode *tmp = root;
            if (root->leaf)
                root = NULL;
            else
                root = root->childs[0];

            delete tmp;
        }
        return;
    }
};

struct Directory {
    Directory *parent;
    BTree *tree;
};