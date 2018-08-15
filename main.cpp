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

//Create a file with a "name" in which will be the virtual HD
void createhd(char *name, int blocks, int bsize)
{
    FILE *pHd;
    pHd = fopen(name, "a");
    if (pHd != NULL) {perror("Error opening file!\n");}
    else
    {
        for (int i = 0; i < blocks; i++)
        {
            for (int j = 0; j < bsize; j++)
            {
                fputc('/', pHd);
                if (ferror(pHd)) { perror("Error writing in to the file\n"); }
            }
            fputc('\n', pHd);
        }
        fclose(pHd);
    }
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