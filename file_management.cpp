#include "BTree.cpp"
#include "structs.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

class FileSystem {
    Directory *root;
    vector<TreeNodeType> diskImg;

  public:
    FileSystem(string imgFile) {
        root = new Directory();
        root->tree = new BTree(3);

        fstream diskImage("fs.img");
    }
    ~FileSystem() { delete root; }

    void mkdir(string name) {
        TreeNodeType *dir = create_directory(name);
        root->tree->insert(dir);
    }

    void new_file(string name, string content) {
        TreeNodeType *file = create_txt_file(name, content);
        root->tree->insert(file);
    }

    void cat(string name) {
        TreeNodeType *node = root->tree->search(name);
        if (node == NULL) {
            cout << name << " não foi encontrado" << "\n";
            return;
        }
        if (node->type == DIRECTORY_TYPE) {
            cout << name << " é uma pasta" << "\n";
        } else {
            cout << node->data.file->content << "\n";
        }
    }

    static void printNode(TreeNodeType *node) { cout << (node->type == DIRECTORY_TYPE ? "D" : "F") << " - " << node->name << "\n"; }

    void ls() { root->tree->traverse(printNode); }

    void cd(string name) {
        if (name == "..") {
            if (root->parent == NULL) {
                cout << "Você já está no root!\n";
            } else {
                root = root->parent;
            }
            return;
        }
        TreeNodeType *node = root->tree->search(name);
        if (node == NULL) {
            cout << name << " não foi encontrado" << "\n";
            return;
        }
        if (node->type == FILE_TYPE) {
            cout << name << " é um arquivo" << "\n";
        } else {
            root = node->data.directory;
        }
    }

    void delete_file(string name) {}
    bool delete_directory(string name) { return false; }

  private:
    TreeNodeType *create_txt_file(string name, string content) {
        File *file = new File();
        file->name = name;
        file->content = content;

        TreeNodeType *node = new TreeNodeType();
        node->type = FILE_TYPE;
        node->name = name;
        node->data.file = file;

        return node;
    }

    TreeNodeType *create_directory(string name) {
        BTree *tree = new BTree(3);
        Directory *dir = new Directory();
        dir->tree = tree;
        dir->parent = root;

        TreeNodeType *node = new TreeNodeType();
        node->name = name;
        node->data.directory = dir;
        node->type = DIRECTORY_TYPE;

        return node;
    }

    static void save_img() {}
};
