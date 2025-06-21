#pragma once
#include <iostream>
using namespace std;

typedef enum
{
    FILE_TYPE,
    DIRECTORY_TYPE
} NodeType;

typedef struct File
{
    char *name;
    char *content;
    size_t size;
} File;

typedef struct Directory Directory;
