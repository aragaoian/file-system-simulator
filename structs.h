#pragma once
#include <iostream>
using namespace std;

typedef enum { FILE_TYPE, DIRECTORY_TYPE } NodeType;
struct File {
    string name;
    string content;
};
struct Directory;
struct TreeNodeType {
    string name;
    NodeType type;
    union {
        File *file;
        Directory *directory;
    } data;
};
