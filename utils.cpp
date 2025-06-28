#pragma once
#include <iostream>
using namespace std;

string tab(int h){
    string res = "";
    for(int i = 0; i < h; i++){
        res += "\t";
    }
    return res;
}
