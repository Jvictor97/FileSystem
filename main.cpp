#include <iostream>
#include <string>
#include <stdio.h>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

vector<int> getclause();
void createhd(string, int, int);

int main(int argc, char const *argv[])
{
    getclause();
    system("pause");
    return 0;
}

void createhd(string name, int bsize, int blocks){
    FILE * file;
    file = fopen("teste.txt", "a");
    if (file != NULL)
        cout<<"Arquivo aberto!\n";
}

vector<int> getclause() {
  char c;
  vector<int> cl;
  std::string line;
  std::getline(cin, line);
  std::istringstream iss(line);
  while ( iss >> c) {    
    cout<<c<<" ";
  }
  return cl;
}