#ifndef FILE_CPP
#define FILE_CPP

#include <iostream>
#include "globals.h"

using namespace std;

void testandoGlobal ()
{
    
}

void removeFile(){
	string filename = params[0];

	for(int i = 0; i < 7; i++){
		if(actualInode.blocks[i] != 0){
			Inode child = inodes[actualInode.blocks[i] - 1];
			// Se for um inode de arquivo
			if(child.type == 2
			&& child.flag == 1 
			&& child.name == filename){
				for(int j = 0; j < 7; j++){
					if(child.blocks[j] != 0){
						strncpy(datablocks[child.blocks[j] - superBlock.firstDataBlock], 0, sizeBlock);
						bitmapDataBlocks.bitMapArray[child.blocks[j] - superBlock.firstDataBlock] = 0;
					}
				}
				inodes[child.number - 1].initialize();
				cout<<GREEN<<"\nArquivo \""<<YELLOW<<filename<<GREEN<<"\" removido com sucesso!\n";
				return;
			}
		}
	}

	cout<<RED<<"ERRO: nenhum arquivo com o nome \""<<YELLOW<<filename<<RED<<"\" neste caminho.\n";
}

void type(){
	string filename = params[0];

	for(int i = 0; i < 7; i++){
		if(actualInode.blocks[i] != 0){
			Inode child = inodes[actualInode.blocks[i] - 1];
			
			if(child.type == 2 // Se for um inode de arquivo
			&& child.flag == 1 // Ativo
			&& child.name == filename){ // Cujo nome seja o filename
				for(int j = 0; j < 7; j++){
					if(child.blocks[j] != 0){
						printf(MAGENTA "%s\n" RESET, datablocks[j]);
					}
				}
				cout<<endl;
				return;
			}
		}
	}

	cout<<RED<<"ERRO: nenhum arquivo com o nome \""<<YELLOW<<filename<<RED<<"\" neste caminho.\n";
}

#endif

