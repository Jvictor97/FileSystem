#include <iostream>
#include <string>
#include <stdio.h>
#include <map>
#include <sstream>
#include <vector>
#include "hd.cpp"
#include <bitset>

using namespace std;


int main() {
  	//bitset<8> mb = bitset<8>(16);
	
	FILE * hd = createhd("myHD",1024, 32);

	writeToBlock(30, "DeuCerto", hd, 32);
	fclose(hd);
	return 0;
}


vector<int> getclause()
{
    char c;
    vector<int> cl;
    string line;
    getline(cin, line);
    istringstream iss(line);
    while (iss >> c)
    {
        cout << c << " ";
    }
    return cl;
}