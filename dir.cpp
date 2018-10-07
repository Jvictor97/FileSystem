#ifndef DIR_CPP
#define DIR_CPP

#include <iostream>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#include "globals.h"
using namespace std;

int size(Inode);
char type(int);

void printInode(Inode i){
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

void createdir(){

	string name = params[0];
	int i, j;

	for(int a = 0; a < 7; a++){
		if(actualInode.blocks[a] - 1 < totalInodes
		&& inodes[actualInode.blocks[a] - 1].flag == 1 
		&& inodes[actualInode.blocks[a] - 1].type == 1 
		&& inodes[actualInode.blocks[a] - 1].name == name
		&& inodes[actualInode.blocks[a] - 1].father_inode == actualInode.number){
			cout<<RED<<"\nERRO: este caminho ja contem um diretorio com o nome \""<<YELLOW<<name<<RED<<"\".\n\n";
			return;
		}
	}

	// Encontra o proximo Inode disponivel
	for(i = 0; i < totalInodes && inodes[i].flag != 0; i++);

	// Caso todos os inodes estejam preenchidos imprime a mensagem de erro e sai da função
	if(i == totalInodes){
		cout<<RED<<"\nERRO: Numero máximo de inodes utilizado..."<<RESET<<endl;
		cout<<YELLOW<<"Dica: Apague algum arquivo/diretorio ou formate seu HD para liberar espaco!\n\n"<<RESET;
		return;
	}

	// Encontra bloco disponivel do Inode pai
	for(j = 0; actualInode.blocks[j] != 0; j++){
		if(j > 6){
			cout<<RED<<"\nERRO: Numero máximo de blocos de endereco utilizado..."<<RESET<<endl;
			cout<<YELLOW<<"Dica: Apague algum arquivo/diretorio ou formate seu HD para liberar espaco!\n\n"<<RESET;
			return;
		}
	}

	inodes[i].flag = 1;
	inodes[i].type = 1;
	strncpy(inodes[i].name, name.c_str(), sizeof(Inode::name));
	inodes[i].father_inode = actualInode.number;
	inodes[actualInode.number - 1].blocks[j] = actualInode.blocks[j] = inodes[i].number;
}

void cd(){
	string dirname = params[0];
	int i,j;

	if(dirname == ".."){
		if(actualInode.number == 1){
			cout<<RED<<"\nERRO: voce ja esta no root!!!\n\n"<<RESET;
			return;
		}
		if(inodes[actualInode.father_inode - 1].number == 1)
			location = location.substr(0, location.find(actualInode.name));
		else
			location = location.substr(0, location.find(actualInode.name) - 1);
		
		actualInode = inodes[actualInode.father_inode - 1];
		return;
	}

	for(i = 0; i < 7; i++){
		if(actualInode.blocks[i] - 1 < totalInodes
			&& inodes[actualInode.blocks[i] - 1].flag == 1 
			&& inodes[actualInode.blocks[i] - 1].type == 1 
			&& inodes[actualInode.blocks[i] - 1].name == dirname
			&& inodes[actualInode.blocks[i] - 1].father_inode == actualInode.number)
		{
			Inode prevInode = actualInode;
			actualInode = inodes[actualInode.blocks[i] - 1];
			
			if(prevInode.number == 1)
				location += actualInode.name;
			else 
				location = location + "/" + actualInode.name;
			return;
		}
	}
	cout<<RED<<"\nERRO: o diretorio nao existe...\n\n"<<RESET;
}

void dir(){
	printf(YELLOW "\n%-26s %-5s %-4s\n" RESET, "Nome", "Tipo", "Tamanho");
	//printInode(actualInode);
	for(int i = 0; i < 7; i++){
		if(actualInode.blocks[i] != 0){
			Inode child = inodes[actualInode.blocks[i] - 1];
			//printInode(child);
			printf("%-26s %2c   %4d bytes\n", child.name, type(child.type), size(child));
		}
	}
}

int size(Inode child){
	// Se for um inode de diretório retorna o tamanho do Inode
	if(child.type == 1)
		return sizeof(Inode);
	// Senão, é um inode de arquivo, neste caso percorre os blocks 
	// do child que sejam diferentes de zero e soma o tamanho do datablock
	else{
		int size = 0;
		for(int i = 0; i < 7; i++){
			if(child.blocks[i] != 0)
				size += sizeBlock;
		}
		return size;
	}
}

char type(int i_type){
	if(i_type == 1)
		return 'D';
	else 
		return 'A';
}

#endif