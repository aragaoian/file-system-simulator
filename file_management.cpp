#include "BTree.cpp"
#include "structs.h"
#include "utils.cpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <bitset>
using namespace std;

class FileSystem {
    Directory *root;
    fstream diskImage;

  public:
    FileSystem(string imgFile) {
        root = new Directory();
        root->tree = new BTree(3);

        diskImage.open("fs.img", ios::in | ios::out | ios::binary | ios::app);
        if (!diskImage.is_open()) {
            cerr << "Erro ao abrir o arquivo fs.img" << endl;
        }
    }
    ~FileSystem() { 
        delete root;
        diskImage.close();
    }

    void mkdir(string name) {
        TreeNodeType *dir = create_directory(name);
        root->tree->insert(dir);
    }

    void touch(string name, string content) {
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

    static void printNode(TreeNodeType *node, int ignoreParam = 0) { cout << (node->type == DIRECTORY_TYPE ? "D" : "F") << " - " << node->name << "\n"; }

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

    void rm(string name, bool recursive = false){
        TreeNodeType *node = root->tree->search(name);
        if(node == NULL){
            cout << (!recursive ? "Arquivo não encontrado!" : "Pasta não encontrada!") << "\n";
            return;
        }
        if(!recursive){
            delete_file(node);
            return;
        }
        delete_directory(node);
    }

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

    void delete_file(TreeNodeType *file) {
        root->tree->deletion(file);
    }

    void delete_directory(TreeNodeType *dir) {
        dir->data.directory->parent->tree->deletion(dir);
        delete dir;
    }

    static void save_img(TreeNodeType *node, int h) {
        fstream diskImage("fs.img", ios::app);
        if(node->type == FILE_TYPE){
            diskImage << "F" << node->name << "\n";
            diskImage << tab(h) << node->data.file->content.length() << " | " << node->data.file->content << "\n";
        }
    }
};
