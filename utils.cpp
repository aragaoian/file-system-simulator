#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

string pad_string(string str, size_t length) { return string(length - str.length(), '0') + str; }

string tab(int h) {
    string res = "";
    for (int i = 0; i < h; i++) {
        res += "\t";
    }
    return res;
}

vector<string> split(const string &input, const string &delimiter) {
    vector<string> tokens;
    string str_copy = input;
    size_t pos = 0;
    string token;
    while ((pos = str_copy.find(delimiter)) != string::npos) {
        token = str_copy.substr(0, pos);
        tokens.push_back(token);
        str_copy.erase(0, (pos + delimiter.length()));
    }
    tokens.push_back(str_copy);

    return tokens;
}

void load_vector(vector<string> &v, fstream &file){
    string line;
    while(getline(file, line)){
        v.push_back(line);
    }
}

int get_tab_size(string s){
    int size = 0;
    for(int i = 0; i < s.length(); i++){
        if(s[i] != '\t') break;
        size++;
    }
    return size;
}