/*
ID: dtopn1
PROG: gift1
LANG: C++
*/
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ofstream fout ("gift1.out");
    ifstream fin ("gift1.in");
    char name[10][10];//,group[7];
    int num;
    fin >> num;
    for (int i = 0; i < num; i++){
    	fin >> name[i];
    }
    for (int i = 0; i < 6 && group[i] != 0; ++i)
    {
    	pdt_gr *= 1 + group[i] - 'A';
    }
    if (pdt_gr % 47 - pdt_cm % 47 == 0)
    {
    	fout << "GO" << endl;
    }
    else{
    	fout << "STAY" << endl;
    }
    return 0;
}
