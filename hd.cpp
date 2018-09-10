/*
This file is related to the functions and attributes of the virtual HD
*/
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <map>
#include <sstream>
#include <vector>
#include <cmath>

// Classes para manipulação do HD
#include "inode.cpp"
#include "superblock.cpp"
#include "config.cpp"
#include "bitmapDataBlocks.cpp"

using namespace std;

string params[10];
string cmd;
int numBlocks;
int sizeBlock;
int inodeBlocks;
int bitmapBlocks;
int numDataBlocks;
string numBlocks_s;
string sizeBlock_s;
string nomeHD;
FILE * configFile; // Ponteiro para o arquivo de configuração de HDs
FILE * hd;    // Ponteiro para o HD que será criado
SuperBlock superBlock;
BitMapDataBlocks bitmapDataBlocks;
Inode genericInode;
Inode * inodes;
char ** datablocks;
Config config;

typedef void (*functions)(void); // function pointer type

// Funções principais:
void createhd();
void exit();
void selecionaHD();
void dirhd();

// Auxiliares:
int strToInt(const string);
void writeSuperBlock();
void writeInodeBlocks();
void writeBitmapBlocks();
void printInodes(Inode);
void printSuperBlock(SuperBlock);
void updateHD();
void exitHD();
void getclause();

// Cria um arquivo com o nome 'nomeHD', o número de blocos 
// 'numBlocks' e cada bloco com tamanho 'sizeBlock'
void createhd()
{
    //TODO: criar uma mensagem para printar ao usuário o tamanho máximo de arquivo
    // que poderá ser armazenado com um bloco do tamanho escolhido

    nomeHD = params[0];
	sizeBlock = strToInt(params[1]);
	numBlocks = strToInt(params[2]);

    nomeHD += ".mvpfs";

    // Verifica se já existe um arquivo com este nome
	hd = fopen(nomeHD.c_str(), "r"); 
	char n = '0';

    // Enquanto forem encontrados arquivos com o nome escolhido
    // incrementa o inteiro identificador 'n'
	while(hd != NULL){
		fclose(hd);
		nomeHD = n == '0' ? nomeHD.substr(0, nomeHD.find(".mvpfs")) : nomeHD.substr(0, nomeHD.find(".mvpfs") - 1);
		nomeHD+= n;
		nomeHD+= ".mvpfs";
		n++;
		hd = fopen(nomeHD.c_str(), "r");
	}

    
    sizeBlock_s = to_string(sizeBlock);
    numBlocks_s = to_string(numBlocks);

    // Cria o arquivo HD para escrita
    hd = fopen(nomeHD.c_str(), "w+");
    if (hd == NULL) { 
        perror("Erro na criacao do arquivo");
        return; 
    }
    else
    {
        writeSuperBlock();
        writeInodeBlocks();
        writeBitmapBlocks();
        for (int block = 0; block < (numBlocks - 1 - inodeBlocks - bitmapBlocks); block++)
        {
            for (int byte = 0; byte < sizeBlock; byte++)
            {
                fputc(0, hd);
                if (ferror(hd)) 
                    perror("Erro na escrita do arquivo");
            }
        }
        fclose(hd); 

        string shortHdName = nomeHD.substr(0, nomeHD.find(".mvpfs"));

        strncpy(config.nomeHD, shortHdName.c_str(), sizeof(Config::nomeHD));
        config.blockSize = sizeBlock;
        config.numBlocks = numBlocks;
        
        configFile = fopen(".config", "a"); // Abre ou cria o arquivo config

        fwrite(&config, sizeof(Config), 1, configFile);
        fclose(configFile);

        return;
    }
}

void selecionaHD(){
    string print;
	map<std::string, functions> localMap;
	localMap["exit"] = exitHD;

    hd = fopen(nomeHD.c_str(), "r+");

	if(hd == NULL){
		cout<<"ERRO: Nenhum HD encontrado com este nome...\n";
		return;
	}

    fread(&superBlock, sizeof(SuperBlock), 1, hd);

    numBlocks = superBlock.numBlocks;
    sizeBlock = superBlock.blockSize;
    inodeBlocks = superBlock.numInodeBlocks;
    
    inodes = (Inode *) malloc(sizeof(Inode) * inodeBlocks);

    int space = sizeBlock;

    for(int i = 0; i < inodeBlocks; i++){
        if(space < sizeof(Inode)){
            fseek(hd, space, SEEK_CUR);
            space = sizeBlock;
        }
        fread(&inodes[i], sizeof(Inode), 1, hd);
        space-=sizeof(Inode);
    }

    //fread(inodes, sizeof(Inode), inodeBlocks, hd);

    // Calcula o número de bitmap blocks 
    for(bitmapBlocks = 1; numBlocks - (1 + inodeBlocks + bitmapBlocks) > sizeBlock * bitmapBlocks; bitmapBlocks++);

    // Calcula o número de datablocks
    numDataBlocks = numBlocks - 1 - inodeBlocks - bitmapBlocks;
    // Inicializa a variável global de bitmap
    bitmapDataBlocks.setSize(numBlocks);

    // Lê do arquivo para a variável o bitmap
    fread(bitmapDataBlocks.bitMapArray, sizeof(bool), numDataBlocks, hd);   

    cout<<numDataBlocks;

    // for(int i = 0; i < numDataBlocks; i++){
    //     cout<<bitmapDataBlocks.bitMapArray[i]<<" ";
    // }

    datablocks = (char**) malloc(sizeof(char*) * numDataBlocks);

    for(int i = 0; i < numDataBlocks; i++){
        datablocks[i] = (char*) malloc(sizeof(char) * sizeBlock);
    }

    fread(datablocks, sizeof(sizeBlock), numDataBlocks, hd);

    fclose(hd);

    string simpleHdName = nomeHD.substr(0, nomeHD.find(".mvpfs"));

    while(cmd != "exit"){
		cout<<simpleHdName<<"# ";
		getclause();

        if(localMap.find(cmd.c_str()) == localMap.end())
            cout<<"ERRO: Esta funcao nao existe...\n";
        else
            localMap[cmd]();
	}
}

void exit(){
	if(hd != NULL)
		fclose(hd);
	return;
}

void writeSuperBlock(){

    strncpy(superBlock.magicNumber, "bXZwZnM", sizeof(SuperBlock::magicNumber));
    strncpy(superBlock.hdName, nomeHD.c_str(), sizeof(SuperBlock::hdName));
    superBlock.blockSize = sizeBlock;
    superBlock.numBlocks = numBlocks;
    superBlock.numFreeBlocks = numBlocks - 1;
    superBlock.firstInodeBlock = 1;

    inodeBlocks = 0.3 * numBlocks;
    
    superBlock.numInodeBlocks = inodeBlocks;

    for(bitmapBlocks = 1; numBlocks - (1 + inodeBlocks + bitmapBlocks) > sizeBlock * bitmapBlocks; bitmapBlocks++);
    
    superBlock.firstDataBlock = inodeBlocks + bitmapBlocks + 1;

    fwrite(&(superBlock), sizeof(SuperBlock), 1, hd);

    int space = sizeBlock - sizeof(SuperBlock);

    while(space--){
        fputc(0, hd);
    }

    // SuperBlock sp;
    // fseek(hd, 0, SEEK_SET);

    // fread(&sp, sizeof(SuperBlock), 1, hd);
    // printSuperBlock(sp);
}

void writeInodeBlocks(){
    Inode root;
    int space;

    root.flag = true;
    root.type = 0;
    root.number = 0;
    root.father_inode = 0;
    strncpy(root.name, "/", sizeof(Inode::name));

    fwrite(&root, sizeof(Inode), 1, hd);

    space = sizeBlock - sizeof(Inode);

    genericInode.flag = false;
    genericInode.type = 2;
    genericInode.father_inode = -1;

    for(int i = 1; i < inodeBlocks; i++){
        if(sizeof(Inode) > space){
            for(int j = 0; j < space; j++){
                fputc(0, hd);
            }
            space = sizeBlock;
        }
        genericInode.number = i;
        fwrite(&genericInode, sizeof(Inode), 1, hd);
        space -= sizeof(Inode);
    }

    inodes = (Inode *) malloc(sizeof(Inode) * inodeBlocks);
    fseek(hd, sizeof(SuperBlock), SEEK_SET);

    for(int i = 0; i < inodeBlocks; i++){
        fread(&inodes[i], sizeof(Inode), 1, hd);
        printInodes(inodes[i]);
    }   
}

int strToInt(const string str){
    int i;
    i = 0;
	int s = 0;
    while(str[s] != '\0')
    {
        i = i * 10 + (str[s] - '0');
        s++;
    }
    return i;
}

void printInodes(Inode i){
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

void printSuperBlock(SuperBlock sp){
    printf("%s\n", sp.magicNumber);
    printf("%s\n", sp.hdName);
    printf("%d\n", sp.blockSize);
    printf("%d\n", sp.numBlocks);
    printf("%d\n", sp.numFreeBlocks);
    printf("%d\n", sp.firstInodeBlock);
    printf("%d\n", sp.firstDataBlock);
}

void writeBitmapBlocks(){
    bitmapDataBlocks.setSize(numBlocks);
    fwrite(bitmapDataBlocks.bitMapArray, bitmapDataBlocks.numBlocks, 1, hd);
}

void updateHD(){
    hd = fopen(nomeHD.c_str(), "w+");
    
    fwrite(&(superBlock), sizeof(SuperBlock), 1, hd);

    int space = sizeBlock;

    for(int i = 0; i < inodeBlocks; i++){
        if(sizeof(Inode) > space){
            for(int j = 0; j < space; j++){
                fputc(0, hd);
            }
            space = sizeBlock;
        }
        fwrite(&inodes[i], sizeof(Inode), 1, hd);
        space -= sizeof(Inode);
    }

    //fwrite(inodes, sizeof(Inode), inodeBlocks, hd);
    fwrite(bitmapDataBlocks.bitMapArray, bitmapDataBlocks.numBlocks, 1, hd);

    for(int i = 0; i < numDataBlocks; i++){
        fputs(datablocks[i], hd);
    }
    fclose(hd);
}

void exitHD(){
    updateHD();

    free(inodes);

    for(int i = 0; i < numDataBlocks; i++){
        free(datablocks[i]);
    }

    free(datablocks);

    cout<<"\nO HD "<<nomeHD<<" foi salvo com sucesso!"<<endl;
}

void getclause(){
	string in;
	int i = 0;
	std::string line;
	std::getline(cin, line);
	std::istringstream iss(line);

	bool first = true;

	while (iss >> in){
		if(first){
			cmd = in;
			first = false;
		}else{
			params[i] = in;
			i++;
		}
	}
}

void dirhd(){


    
}

void formathd(){
    nomeHD = params[0] + ".mvpfs";

    hd = fopen(nomeHD.c_str(), "r");
    fread(&superBlock, sizeof(SuperBlock), 1, hd);
    remove(nomeHD.c_str());

    params[1] = superBlock.blockSize;
    params[2] = superBlock.numBlocks;

    createhd();
}