#include "BTree.cpp"
#include "structs.h"
#include "utils.cpp"
#include <bitset>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

class FileSystem {
    Directory *root;
    fstream disk_image;
    vector<string> disk_image_v;

  public:
    FileSystem(string imgFile) {
        root = new Directory();
        root->tree = new BTree(3);

        disk_image.open("fs.img", ios::in | ios::out | ios::binary | ios::app);
        if (!disk_image.is_open()) {
            cerr << "Erro ao abrir o arquivo fs.img" << endl;
        }
        load_vector(disk_image_v, disk_image);
        mount();
    }
    ~FileSystem() {
        disk_image.close();
        disk_image.open("fs.img", ios::out | ios::trunc);
        go_to_root();

        root->tree->traverse([](TreeNodeType *node) { save_img(node, 0); });
        delete root;
        disk_image.close();
    }

    void mkdir(string name) {
        TreeNodeType *node = root->tree->search(name);
        if (node != NULL) {
            cout << "Pasta com nome já existente" << "\n";
            return;
        }
        TreeNodeType *dir = create_directory(name);
        root->tree->insert(dir);
    }

    void touch(string name, string content) {
        TreeNodeType *node = root->tree->search(name);
        if (node != NULL) {
            cout << "Arquivo com nome já existente" << "\n";
            return;
        }

        if (content.length() > 1000 * 1000) {
            cout << "O arquivo não pode passar de 1MB" << "\n";
            return;
        }

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

    void mount() {
        int i = 0;
        load_img(i, 0, root);
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

    void rm(string name) {
        TreeNodeType *node = root->tree->search(name);
        if (node == NULL) {
            cout << "Arquivo ou pasta não encontrado!" << "\n";
            return;
        }
        if (node->type == DIRECTORY_TYPE) {
            if (!node->data.directory->tree->isEmpty()) {
                cout << "Não é possível remover " << node->name << ", a pasta não está vazia!" << "\n";
                return;
            }
            delete_directory(node);
        } else {
            delete_file(node);
        }
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

    TreeNodeType *create_directory(string name, Directory *parent_dir = nullptr) {
        BTree *tree = new BTree(3);
        Directory *dir = new Directory();
        dir->tree = tree;
        dir->parent = parent_dir == nullptr ? root : parent_dir;

        TreeNodeType *node = new TreeNodeType();
        node->name = name;
        node->data.directory = dir;
        node->type = DIRECTORY_TYPE;

        return node;
    }

    void go_to_root() {
        if (root->parent != NULL) {
            root = root->parent;
            go_to_root();
        }
    }

    void delete_file(TreeNodeType *file) { root->tree->deletion(file); }

    void delete_directory(TreeNodeType *dir) {
        dir->data.directory->parent->tree->deletion(dir);
        delete dir;
    }

    static void save_img(TreeNodeType *node, int h) {
        fstream disk_image("fs.img", ios::app);
        if (node->type == FILE_TYPE) {
            disk_image << tab(h) << "F:" << pad_string(to_string(node->name.length()), 10) << node->name
                       << pad_string(to_string(node->data.file->content.length()), 32) << node->data.file->content << "\n";
            disk_image.flush();
        } else {
            disk_image << tab(h) << "D:" << pad_string(to_string(node->name.length()), 10) << node->name << "\n";
            disk_image.flush();
            node->data.directory->tree->traverse([h](TreeNodeType *childNode) { save_img(childNode, h + 1); });
        }
    }

    void load_img(int &i, int curr_depth, Directory *curr_dir) {
        while (i < disk_image_v.size()) {
            int tabs_size = get_tab_size(disk_image_v[i]);
            if (tabs_size <= curr_depth && tabs_size != 0) return; // chegou no fim do folder

            string line = disk_image_v[i].substr(tabs_size);
            vector<string> node_info = split(line, ":");
            string type = node_info[0];
            int name_size = stoi(node_info[1].substr(0, 10));
            string name = node_info[1].substr(10, name_size);

            if (type == "F") {
                int content_size = stoi(node_info[1].substr(10 + name_size, 32));
                string content = node_info[1].substr(10 + name_size + 32, content_size);
                TreeNodeType *node = create_txt_file(name, content);
                curr_dir->tree->insert(node);
                i++;

            } else {
                TreeNodeType *node = create_directory(name, curr_dir);
                curr_dir->tree->insert(node);
                i++;
                load_img(i, tabs_size, node->data.directory);
            }
        }
    }
};
