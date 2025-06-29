#pragma once
#include <iostream>
using namespace std;
string pad_string(string str, size_t length) { return string(length - str.length(), '0') + str; }
string tab(int h) {
    string res = "";
    for (int i = 0; i < h; i++) {
        res += "\t";
    }
    return res;
}
