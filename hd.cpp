/*
This file is related to the functions and attributes of the virtual HD
*/
#include <iostream>
#include <string>
#include <stdio.h>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

FILE * createhd(char const *name, int nBlocks, int sBlock);
void writeHeader(FILE * hd, int sBlock, int nBlocks);
void writeToBlock(int blockNo, string content, FILE * hd, int sBlock);


//Create a file with a "name" in which will be the virtual HD
FILE * createhd(char const *name, int nBlocks, int sBlock)
{
    FILE *pHd;
    pHd = fopen(name, "w+");
    if (pHd == NULL) { 
        perror("Error opening file!\n");
        return NULL; 
    }
    else
    {
        writeHeader(pHd, sBlock, (nBlocks/(sBlock * 8)));
        for (int block = 0; block < nBlocks; block++)
        {
            for (int byte = 0; byte < sBlock; byte++)
            {
                fputc('#', pHd);
                if (ferror(pHd)) { perror("Error writing in to the file\n"); }
            }
            //fputc('\n', pHd); // For visualization proporse
        }
        return pHd;
    }
}

void writeHeader(FILE * hd, int sBlock, int nBlocks){
	fputc('$', hd);
	for(int bloco = 0; bloco < nBlocks; bloco++){
		for(int size = 0; size < sBlock; size++){
			fputc('0', hd);
		}
		//fputc('\n', hd);		
	}
	fputc('*', hd);
}


void writeToBlock(int blockNo, string content, FILE * hd, int sBlock){
	if(content.length() < sBlock){
		fseek(hd, blockNo * sBlock, SEEK_SET);
		fputs(content.c_str(), hd);
	}else{
		cout<<"Too big";
	}
}