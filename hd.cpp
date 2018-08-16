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

//Create a file with a "name" in which will be the virtual HD
FILE * createhd(char const *name, int sblocks, int bsize)
{
    FILE *pHd;
    pHd = fopen(name, "w+");
    if (pHd == NULL) { 
        perror("Error opening file!\n");
        return NULL; 
    }
    else
    {
        writeHeader(pHd, sblocks, (bsize/(sblocks * 8)));
        for (int block = 0; block < sblocks; block++)
        {
            for (int byte = 0; byte < bsize; byte++)
            {
                fputc('#', pHd);
                if (ferror(pHd)) { perror("Error writing in to the file\n"); }
            }
            //fputc('\n', pHd); // For visualization proporse
        }
        return pHd;
    }
}

void writeHeader(FILE * hd, int sizeBloco, int qtdeBlocos){
	fputc('$', hd);
	for(int bloco = 0; bloco < qtdeBlocos; bloco++){
		for(int size = 0; size < sizeBloco; size++){
			fputc('0', hd);
		}
		//fputc('\n', hd);		
	}
	fputc('*', hd);
}


void writeToBlock(int blockNo, string content, FILE * hd, int sizeBloco){
	if(content.length() < sizeBloco){
		fseek(hd, blockNo * sizeBloco, SEEK_SET);
		fputs(content.c_str(), hd);
	}else{
		cout<<"Too big";
	}
}