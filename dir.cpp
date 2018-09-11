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
    //cout << sizeof(Inode) << endl;
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
			cout<<RED<<"ERRO: este caminho ja contem um diretorio com o nome \""<<YELLOW<<name<<RED<<"\"."<<endl;
			return;
		}
	}

	// Encontra o proximo Inode disponivel
	for(i = 0; i < totalInodes && inodes[i].flag != 0; i++);

	// Caso todos os inodes estejam preenchidos imprime a mensagem de erro e sai da função
	if(i == totalInodes){
		cout<<RED<<"\nERRO: Numero máximo de inodes utilizado..."<<RESET<<endl;
		cout<<YELLOW<<"Hint: Apague algum arquivo/diretorio ou formate seu HD para liberar espaco!"<<RESET<<endl;
		return;
	}

	// Encontra 
	for(j = 0; actualInode.blocks[j] != 0; j++){
		//cout<<"\nConteudo do block["<<j<<"]: "<<actualInode.blocks[j]<<endl;
		if(j > 6){
			cout<<RED<<"\nERRO: Numero máximo de blocos de endereco utilizado..."<<RESET<<endl;
			cout<<YELLOW<<"Hint: Apague algum arquivo/diretorio ou formate seu HD para liberar espaco!"<<RESET<<endl;
			return;
		}
	}

	inodes[i].flag = 1;
	strncpy(inodes[i].name, name.c_str(), sizeof(Inode::name));
	inodes[i].type = 1;
	inodes[i].father_inode = actualInode.number;
	inodes[actualInode.number - 1].blocks[j] = actualInode.blocks[j] = inodes[i].number;
	// cout<<"PAI:\n";
	// printInode(actualInode);
	// cout<<"Filho:\n";
	// printInode(inodes[i]);

}

void cd(){
	// cout<<"Root:"<<endl;
	// printInode(inodes[0]);

	// cout<<"Estava em: "<<endl;
	// printInode(actualInode);

	string dirname = params[0];
	int i,j;
	// cd inode.name 

	if(dirname == ".."){
		if(actualInode.number == 1){
			cout<<RED<<"ERRO: voce ja esta no root!!!"<<RESET<<endl;
			return;
		}
		if(inodes[actualInode.father_inode - 1].number == 1)
			location = location.substr(0, location.find(actualInode.name));
		else
			location = location.substr(0, location.find(actualInode.name) - 1);
		
		actualInode = inodes[actualInode.father_inode - 1];
		// cout<<"Inode Atual: "<<endl;
		// printInode(actualInode);
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

			// cout<<"Inode Atual"<<endl;
			// printInode(actualInode);
			return;
		}
	}
	cout<<RED<<"ERRO: o diretorio nao existe..."<<RESET<<endl;
}

#endif