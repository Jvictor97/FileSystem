#include <iostream>


#include "globals.h"
using namespace std;


void createdir(){

	string name = params[0];
	int i, j;

	// Encontra o proximo Inode disponivel
	for(i = 0; inodes[i].flag != 0; i++);

	// Encontra 
	for(j = 0; actualInode.blocks[j] != 0; j++){
		if(j > 6){
			cout << "Numero máximo de blocos utilizado" << endl;
			return;
		}
	}

	inodes[i].flag = 1;
	strncpy(inodes[i], name.c_str(), sizeof(Inode::name));
	inodes[i].type = 1;
	inodes[i].father_inode = actualInode.number;
	actualInode.blocks[j] = inodes[i].number;
}

void cd(){

	string name = params[0];
	int i,j;
	// cd inode.name 

	for(i = 0; i < inodeBlocks; i++){
		if(inodes[actualInode.blocks[i] -1].flag == 1 
			&& inodes[actualInode.blocks[i] -1].type == 1 
			&& inodes[actualInode.blocks[i] -1].father_inode == actualInode.number)
		{
			actualInode = inodes[actualInode.blocks[i] -1];
		}else{
			cout << "O diretorio nao existe" << endl;
		}
	}
	
}
// void createdir(){
// 	string folder_name;
// 	if(cin>>createdir>>folder_name){
// 		const int dir_err = mkdir(folder_name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
// 		if (dir_err == -1) {
// 			perror("Erro");
// 			exit(1);
// 		}	
// 	}

	
// 	#elif defined WIN32
// 	if(mkdir(folder_name.c_str()) != 0){
// 			perror("Erro");
// 		}
// 		else{
// 			puts("Diretorio Criado com Sucesso.");
// 		}
	
// 	#elif defined WIN64 
// 	if(mkdir(folder_name.c_str()) != 0){
// 			perror("Erro");
// 		}
// 		else{
// 			puts("Diretorio Criado com Sucesso.");
// 		}

// 	#else
// 	#error Plataforma n�o suportada
// 	#endif

// }

// #else
// #error Plataforma n�o suportada
// #endif
