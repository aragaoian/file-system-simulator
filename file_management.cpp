#include <iostream>
#include "structs.h"
using namespace std;

class FileManagerNode{
    public:
        char *name;
        NodeType type;
        union
        {
            File *file;
            Directory *directory;
        } data;
    private:
        
      
    friend class FileManager;
};

class FileManager{
    FileManagerNode *root;
    public:
        FileManager () {
            root = NULL;
        }
        ~FileManager () {
            delete root;
        }
    private:
        
};
