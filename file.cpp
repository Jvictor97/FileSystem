#ifndef FILE_CPP
#define FILE_CPP

#include <iostream>
#include <string>
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

    int aux;
    for(aux = 0; aux < totalInodes; aux++){
        if(inodes[aux].type == 2
            && inodes[aux].name == fileName 
            && inodes[aux].father_inode == actualInode.number)
        {
            cout<<RED<<"\nERRO: O arquivo \""<<YELLOW<<fileName<<RED<<"\""<<" ja existe neste diretorio\n\n";
            return;
        }
    }

    

    string content;
    getline(cin, content);
    
    if(content.size() > (sizeBlock * 7)){
        cout<<RED<<"O arquivo é muito grande" << endl;
        return;
    }


    int i;

    // Encontra o proximo Inode disponivel
    for(i = 0; i < totalInodes && inodes[i].flag != 0; i++);

    //TODO: diminuir a quantia de blocos livres no superblock em amtBlocks
    int amtBlocks = ceil((double)content.size() / (double)sizeBlock);

    inodes[i].flag = 1;
    inodes[i].type = 2;
    strncpy(inodes[i].name, fileName.c_str(), sizeof(Inode::name));
    inodes[i].father_inode = actualInode.number;

    int actual;
	for(actual = 0; actualInode.blocks[actual] != 0; actual++){
		if(actual > 6){
			cout<<RED<<"\nERRO: Numero máximo de blocos de endereco utilizado...\n"<<RESET;
			cout<<YELLOW<<"Dica: Apague algum arquivo/diretorio ou formate seu HD para liberar espaco!\n\n"<<RESET;
            inodes[i].initialize();
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
        if(content.size() > sizeBlock)
            content = content.substr(sizeBlock);
    }
    // Atualiza o superblock com o número de blocos livres
    superBlock.numFreeBlocks -= amtBlocks;
}


void removeFile(){
	string filename = params[0];
    int amtBlocks = 0;

	for(int i = 0; i < 7; i++){
		if(actualInode.blocks[i] != 0){
			Inode child = inodes[actualInode.blocks[i] - 1];
			// Se for um inode de arquivo
			if(child.type == 2
			&& child.flag == 1 
			&& child.name == filename){
				for(int j = 0; j < 7; j++){
					if(child.blocks[j] != 0){
                        amtBlocks++;
                        for(int b = 0; b < sizeBlock; b++){
                            datablocks[child.blocks[j] - superBlock.firstDataBlock][b] = 0;
                        }
						bitmapDataBlocks.bitMapArray[child.blocks[j] - superBlock.firstDataBlock] = 0;
                        inodes[actualInode.number - 1].blocks[i] = actualInode.blocks[i] = 0;
                        //printInod(actualInode);
					}
				}
                superBlock.numFreeBlocks += amtBlocks;
				inodes[child.number - 1].initialize();
				cout<<GREEN<<"\nArquivo \""<<YELLOW<<filename<<GREEN<<"\" removido com sucesso!\n\n";
				return;
			}
		}
	}

	cout<<RED<<"\nERRO: nenhum arquivo com o nome \""<<YELLOW<<filename<<RED<<"\" neste caminho.\n\n";
}                                                   


//TODO: Validar o conteúdo de arquivos de mesmo nome em outro nível de diretorio
void type(){
	string filename = params[0];
    fflush(stdin);

	for(int i = 0; i < 7; i++){
		if(actualInode.blocks[i] != 0){
			Inode child = inodes[actualInode.blocks[i] - 1];
			
			if(child.type == 2 // Se for um inode de arquivo
			&& child.flag == 1 // Ativo
			&& child.name == filename // Cujo nome seja o filename
            && child.father_inode == actualInode.number // E esta no diretorio atual
            ){ 
				for(int j = 0; j < 7; j++){
					if(child.blocks[j] != 0){
                        for(int p = 0; p < sizeBlock; p++){
						    printf(MAGENTA "%c" RESET, datablocks[child.blocks[j] - superBlock.firstDataBlock][p]);
                        }
                        printf("\n");
					}
				}
				return;
			}
		}
	}

	cout<<RED<<"\nERRO: nenhum arquivo com o nome \""<<YELLOW<<filename<<RED<<"\" neste caminho.\n\n";
}

void rename(){
    string currentName = params[0];
    string newName = params[1];

    int fileInode = 0;
    bool newNameExists = false;
    
    for(int i = 0; i < 7; i++){
        if(actualInode.blocks[i] != 0){
			Inode child = inodes[actualInode.blocks[i] - 1];
			
			if(child.type == 2 // Se for um inode de arquivo
			&& child.flag == 1 // Ativo
            && child.father_inode == actualInode.number // E esta no diretorio atual
            ){ 
			    // Cujo nome seja o currentName
                if(child.name == currentName){
                    fileInode = child.number;
                }
                if(child.name == newName)
                    newNameExists = true;
            }
        }
    }

    if(fileInode && !newNameExists){
        // Renomeia o arquivo 
        strncpy(inodes[fileInode - 1].name, newName.c_str(), sizeof(Inode::name));
        // actualInode = inodes[actualInode.number - 1];
    }
    else   
        if(!fileInode)
        	cout<<RED<<"\nERRO: nenhum arquivo com o nome \""<<YELLOW<<currentName<<RED<<"\" neste caminho.\n\n";
        else
            cout<<RED<<"\nERRO: ja existe um arquivo com o nome \""<<YELLOW<<newName<<RED<<"\" neste caminho.\n\n";
}

#endif

