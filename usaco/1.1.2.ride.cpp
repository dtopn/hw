/*
ID: dtopn1
PROG: ride
LANG: C++
*/
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    ofstream fout ("ride.out");
    ifstream fin ("ride.in");
    char comet[7],group[7];
    fin >> comet >> group;
    int flag;
    long int pdt_cm = 1, pdt_gr = 1;
    for (int i = 0; i < 6 && comet[i] != 0; ++i)
    {
        pdt_cm *= 1 + comet[i] - 'A';
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
