#ifndef FILE_CPP
#define FILE_CPP

#include <iostream>
#include "globals.h"

using namespace std;

void printInod(Inode i){
    printf("Flag: %d\n", i.flag);
    printf("Type: %d\n", i.type);
    printf("Number: %d\n", i.number);
    printf("Father: %d\n", i.father_inode);
    printf("Name: %s\n", i.name);

    for(int j = 0; j < 7; j++){
        printf("Block[%d]: %d\n", j, i.blocks[j]);
    }
    cout<<endl;
}

void create(){

    string fileName = params[0];

    //char buffer[blockSize * 7];
    
    string content;
    getline(cin, content);
    
    cout << "AQUIII::" << content << endl;
    if(content.size() > (sizeBlock * 7)){
        cout<<RED<<"O arquivo é muito grande" << endl;
        return;
    }

    int i;

    // Encontra o proximo Inode disponivel
    for(i = 0; i < totalInodes && inodes[i].flag != 0; i++);


    cout << "Content Size: " << content.size() << endl;
    cout << "sizeBlock: " << sizeBlock << endl;
    
    int amtBlocks = ceil((double)content.size() / (double)sizeBlock);

    cout << "amountBlocks: " << amtBlocks << endl;

    inodes[i].flag = 1;
    inodes[i].type = 2;
    strncpy(inodes[i].name, fileName.c_str(), sizeof(Inode::name));
    inodes[i].father_inode = actualInode.number;    

    int actual;
	for(actual = 0; actualInode.blocks[actual] != 0; actual++){
		if(actual > 6){
			cout<<RED<<"\nERRO: Numero máximo de blocos de endereco utilizado..."<<RESET<<endl;
			cout<<YELLOW<<"Hint: Apague algum arquivo/diretorio ou formate seu HD para liberar espaco!"<<RESET<<endl;
			return;
		}
	}

    inodes[actualInode.number - 1].blocks[actual] = actualInode.blocks[actual] = inodes[i].number;

    int j, k;

    // Encontra o proximo DataBlock disponivel
    for(j = 0; j < amtBlocks; j++){
        for(k = 0; bitmapDataBlocks.bitMapArray[k] && k < numDataBlocks; k++);
        bitmapDataBlocks.bitMapArray[k] = 1;
        inodes[i].blocks[j] = k + superBlock.firstDataBlock;
   }

    for(j = 0; j < amtBlocks; j++){
        strncpy(datablocks[inodes[i].blocks[j] - superBlock.firstDataBlock], content.substr(0,sizeBlock).c_str(), sizeBlock);
        content = content.substr(sizeBlock, content.size());
    }

     printInod(inodes[i]);
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

