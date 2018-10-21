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
#include <stack>
using namespace std;

int size(Inode);
char type(int);
void copyDirInode(Inode dirInode, Inode createInode);
Inode createdDirInode;
void removeDirInode(Inode);
void removeFileInode(Inode);

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
	createdDirInode = inodes[i];
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

void renamedir(){

	string dirName = params[0];
	string newDirName = params[1];

	for(int i = 0; i < 7; i++){
		if(actualInode.blocks[i] - 1 < totalInodes
			&& inodes[actualInode.blocks[i] - 1].flag == 1 
			&& inodes[actualInode.blocks[i] - 1].type == 1 
			&& inodes[actualInode.blocks[i] - 1].name == dirName
			&& inodes[actualInode.blocks[i] - 1].father_inode == actualInode.number)
		{

			strncpy(inodes[actualInode.blocks[i] - 1].name, newDirName.c_str(), sizeof(Inode::name));
			
			return;
		}		
	}
	cout << RED << "\nERRO: O diretorio nao existe\n" << RESET;
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

void removedir(){
	string dirname = params[0];

	for(int i = 0; i < 7; i++){
		if(actualInode.blocks[i] != 0 
			&& inodes[actualInode.blocks[i] - 1].flag == 1 
			&& inodes[actualInode.blocks[i] - 1].type == 1 
			&& inodes[actualInode.blocks[i] - 1].name == dirname
			&& inodes[actualInode.blocks[i] - 1].father_inode == actualInode.number)
		{
			Inode curInode = actualInode;
			removeDirInode(inodes[actualInode.blocks[i] - 1]);
			actualInode = curInode;
			return;
		}
	}
	
	cout<<RED<<"\nERRO: nenhum diretorio com o nome \""<<YELLOW<<dirname<<RED<<"\" no caminho atual.\n\n";
}

void removeDirInode(Inode dirInode){
	for(int i = 0; i < 7; i++){
		if(dirInode.blocks[i] != 0 && inodes[dirInode.blocks[i] - 1].flag == 1)
		{
			if(inodes[dirInode.blocks[i] - 1].type == 1) // Diretório
				removeDirInode(inodes[dirInode.blocks[i] - 1]);
			else
				if(inodes[dirInode.blocks[i] - 1].type == 2) // Arquivo
					removeFileInode(inodes[dirInode.blocks[i] - 1]);
		}
	}
	inodes[dirInode.number - 1].initialize();
}

void removeFileInode(Inode fileInode){

	actualInode = inodes[fileInode.father_inode - 1];
	params[0] = fileInode.name;
	removeFile();

	// Se for um inode de arquivo
	// Inode fileInode = inodes[inodeNum - 1];
	// for(int j = 0; j < 7; j++){
	// 	if(fileInode.blocks[j] != 0){
	// 		amtBlocks++;
	// 		for(int b = 0; b < sizeBlock; b++){
	// 			datablocks[fileInode.blocks[j] - superBlock.firstDataBlock][b] = 0;
	// 		}
	// 		bitmapDataBlocks.bitMapArray[fileInode.blocks[j] - superBlock.firstDataBlock] = 0;
	// 		inodes[actualInode.number - 1].blocks[i] = actualInode.blocks[i] = 0;
	// 		//printInod(actualInode);
	// 	}
	// }
	// superBlock.numFreeBlocks += amtBlocks;
	// inodes[fileInode.number - 1].initialize();
}

void movedir(){
	string orig = params[0];
	string dest = params[1];
	char * stringPath = (char *) malloc(sizeof(char) * params[1].length());

	for(int i = 0; i < 7; i++){
		if(actualInode.blocks[i] - 1 < totalInodes
			&& inodes[actualInode.blocks[i] - 1].flag == 1 
			&& inodes[actualInode.blocks[i] - 1].type == 1
			&& inodes[actualInode.blocks[i] - 1].name == orig
			&& inodes[actualInode.blocks[i] - 1].father_inode == actualInode.number)
		{
			
						
			return;
		}		
	}
}

void copydir(){

	string curName = params[0];
    string newName;
    char * stringPath = (char *) malloc(sizeof(char) * params[1].length());
    Inode searchInode;
    int fileInode = 0;

    for(int i = 0; i < 7; i++){
        if(actualInode.blocks[i] != 0){
            Inode child = inodes[actualInode.blocks[i] - 1];

            if(child.flag == 1 // Se o inode estiver ativo
            && child.type == 1 // For de tipo diretorio
            && strcmp(child.name, params[0].c_str()) == 0 // Tiver o nome dado por params[0]
            && child.father_inode == actualInode.number) // E que seja filho de actualInode
            {
                fileInode = child.number;
            }
        }
    }

    if(!fileInode){
        cout<<RED<<"\nERRO: nenhum arquivo com o nome \""<<YELLOW<<params[0]<<RED"\" no caminho atual.\n\n";
        return;
    }

	// USAGE: copydir dir /dir/subdir/
    if(params[1].back() == '/')
        newName = curName;
    else{
        int lastFolderEnd = params[1].rfind("/");
		cout << "Last: " << lastFolderEnd << endl;
		// USAGE: copydir dir dir2
        if(lastFolderEnd == -1){
			cout << "ENTROU NO LAST" << endl;
            newName = params[1];
		}
		// USAGE: copydir dir /dir/subdir/dirName
        else{
            newName = params[1].substr(lastFolderEnd + 1);
            params[1].erase(lastFolderEnd + 1, params[1].length() - 1);
		}
    }

    // INICIO REMOVER

    // cout<<"Nome Atual: "<<curName<<endl;
    // cout<<"Novo Nome: "<<newName<<endl;
    // cout<<"Caminho: "<<params[1]<<endl;
    // exit(0);

    // FIM REMOVER

    if(params[1][0] == '/'){
        // Copia para o path baseado no root
        strcpy(stringPath, params[1].substr(1).c_str());
        searchInode = inodes[0];
        //printf("Baseado no ROOT\n");
    }
    else if(params[1][0] == '.' && params[1][1] == '/'){
        // Copia para o path baseado no inode atual
        strcpy(stringPath, params[1].substr(2).c_str());
        searchInode = actualInode;
        //printf("Baseado no ATUAL\n");

    }else{
        // Copia para o path baseado no inode atual    
        strcpy(stringPath, params[1].c_str());
        searchInode = actualInode;
        printf("Baseado no ATUAL\n");

    }

    //printf("StringPath: \"%s\"\n", stringPath);

	char * path;
    string fullPath; 

    fullPath = stringPath;
    path = strtok(stringPath, "/");


    while(path != NULL && fullPath.find("/") != -1){
		//cout<<path<<endl;
        //cout<<"Path: "<<path<<endl;
        int i;
		for(i = 0; i < 7; i++){
            if(searchInode.blocks[i] != 0 && searchInode.blocks[i] - 1 < totalInodes){
                Inode child = inodes[searchInode.blocks[i] - 1];

                if(child.type == 1 // Se for um inode de diretorio
                && child.flag == 1 // Ativo
                && strcmp(child.name, path) == 0 // Que possui o nome de path
                && child.father_inode == searchInode.number // E esta no diretorio atual
                ){ 
                    searchInode = child;
                    break;
                }
            }
        }
        if(i == 7){
            cout<<RED<<"\nERRO: o caminho \""<<YELLOW<<params[1]<<RED<<"\" nao foi localizado.\n\n";
            return;
        }
        path = strtok(NULL, "/");
	}

	    Inode createInode = searchInode;

		if(actualInode.number == createInode.number && curName == newName) {
        cout<<RED<<"\nERRO: o arquivo não pode ser copiado com o mesmo nome para o mesmo caminho.\n\n";
        return;
    }

    // printf("Nome do arquivo: %s\n", inodes[fileInode - 1].name);
    // printf("Nome pasta: %s\n", searchInode.name);

    // INICIO DO CREATE

    int blankBlock = -1;

    for(int aux = 0; aux < 7; aux++){
        if(createInode.blocks[aux] != 0){
            Inode child = inodes[createInode.blocks[aux] - 1];
            if(child.type == 1 // Se o tipo for diretorio
            && child.name == newName) // Com o mesmo nome do novo diretorio
            {
                cout<<RED<<"\nERRO: Um diretorio com nome \""<<YELLOW<<newName<<RED<<"\""<<" ja existe neste diretorio\n\n";
                return;
            }
        }
        else
            if(blankBlock == -1)
                blankBlock = aux; // Seleciona o bloco do inode Pasta em que o inode de arquivo será gravado        
    }  

    if(blankBlock == -1){
        cout<<RED<<"\nERRO: Numero máximo de blocos de endereco utilizado...\n"<<RESET;
        cout<<YELLOW<<"Dica: Apague algum arquivo/diretorio ou formate seu HD para liberar espaco!\n\n"<<RESET;
        return;
    }

    // Guarda conteúdo os Inodes que são filhos de diretorio
    Inode dirInode = inodes[fileInode - 1];
    cout<<"Nome arquivo: "<<dirInode.name<<endl;

	Inode saveActual = actualInode;

	cout << "AQUI : \n\n" << endl;

	copyDirInode(dirInode, createInode);

	actualInode = saveActual;
	return;
	/*Sequencia da logica:
	1) Se o iNode filho for de diretorio, percorrer pelos seus descendentes até que se encontre o ultimo
	  descendente diretorio,  achar um iNode livre no vetor de iNode, e copiar o inode.
	2) Se o iNode filho for de arquivo, achar um iNode livre no vetor de iNode, guardar o conteudo dos datablocks do inode filho
	
	*/



	// for(int n = 0; n < nKids; n++){
	// 	//Inode de diretorio
	// 	if(kids[n].type == 1){
	// 		for(int localNode = 0; localNode; ++){
				
	// 		}
	// 	}
	// }

/*
    int node;
    // Encontra o proximo Inode disponivel
    for(node = 0; node < totalInodes && inodes[node].flag != 0; node++);

    int amtBlocks = ceil((double) content.length() / (double)sizeBlock);

    // Inicia a criação do inode de arquivo
    inodes[node].flag = 1;
    inodes[node].type = 2;
    strncpy(inodes[node].name, newName.c_str(), sizeof(Inode::name));
    inodes[node].father_inode = createInode.number;

    int k;
    // Encontra o proximo DataBlock disponivel
    for(int j = 0; j < amtBlocks; j++){
        for(k = 0; bitmapDataBlocks.bitMapArray[k] && k < numDataBlocks; k++);
        bitmapDataBlocks.bitMapArray[k] = 1;
        // Insere no inode arquivo as referências de datablocks utilizados
        inodes[node].blocks[j] = k + superBlock.firstDataBlock;
    }

    // Insere no inode Pasta o número do filho (inode arquivo) no bloco dado por 'node'
    inodes[createInode.number - 1].blocks[blankBlock] = inodes[node].number;

    for(int j = 0; j < amtBlocks; j++){
        strncpy(datablocks[inodes[node].blocks[j] - superBlock.firstDataBlock], content.substr(0,sizeBlock).c_str(), sizeBlock);
        if(content.size() > sizeBlock)
            content = content.substr(sizeBlock);
    }
    // Atualiza o superblock com o número de blocos livres
    superBlock.numFreeBlocks -= amtBlocks;

    // FIM DO CREATE

    if(!movingFile)
        cout<<GREEN<<"\nArquivo \""<<YELLOW<<curName<<GREEN"\" copiado com sucesso!\n\n"<<RESET;

    free(stringPath);
	*/
}


void copyDirInode(Inode dirInode, Inode createInode){

	cout << "dirInode:" << dirInode.name << endl;
	cout << "createInode: " << createInode.name << endl;

	params[0] = dirInode.name;
	actualInode = createInode;
	createdir();
	Inode actualDirInode = createdDirInode;

	for(int i = 0; i < 7; i++){
		if(dirInode.blocks[i] != 0){
			if(inodes[dirInode.blocks[i] - 1].type == 1){
				copyDirInode(inodes[dirInode.blocks[i] -1],  createdDirInode);
				actualInode = createInode;
			}
			else{
				
				params[0] = inodes[dirInode.blocks[i] - 1].name;
				copy();
				cout << "FAZ O ARQUIVO" << endl;
			}
		}
	}
	cout << "PAREI AQUI!! \n\n"<< endl;
}
#endif