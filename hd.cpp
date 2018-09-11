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
#include <fstream>

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
Config hdList[20];

typedef void (*functions)(void); // function pointer type

// Funções principais:
void createhd();
void exit();
void selecionaHD();
void dirhd();
void typehd();

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
        fflush(hd);
        writeSuperBlock();
        fflush(hd);
        fseek(hd, 0, SEEK_END);
        writeInodeBlocks();
        fflush(hd);
        fseek(hd, 0, SEEK_END);
        //writeBitmapBlocks();

        numDataBlocks = 5; //numBlocks - 1 - inodeBlocks - bitmapBlocks;
        fflush(hd);
        fseek(hd, 0, SEEK_END);

        // for (int block = 0; block < numDataBlocks; block++)
        // {
        //     for (int byte = 0; byte < sizeBlock; byte++)
        //     {
        //         fputc(0, hd);
        //         if(ferror(hd)) 
        //             perror("Erro na escrita do arquivo");
        //     }
        // }
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

    for(int i = 0; i < numDataBlocks; i++){
        cout<<bitmapDataBlocks.bitMapArray[i]<<" ";
    }

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

    inodeBlocks = floor(0.3 * numBlocks);
    
    superBlock.numInodeBlocks = inodeBlocks;

    for(bitmapBlocks = 1; numBlocks - (1 + inodeBlocks + bitmapBlocks) > sizeBlock * bitmapBlocks; bitmapBlocks++);
    
    superBlock.firstDataBlock = inodeBlocks + bitmapBlocks + 1;

    fflush(stdin);
    fwrite(&(superBlock), sizeof(SuperBlock), 1, hd);

    int space = sizeBlock - sizeof(SuperBlock);

    while(space--){
        fputc(0, hd);
    }

    SuperBlock sp;
    fseek(hd, 0, SEEK_SET);

    fread(&sp, sizeof(SuperBlock), 1, hd);
    printSuperBlock(sp);
}

void writeInodeBlocks(){
    Inode root;
    int space;

    root.flag = true;
    root.type = 0;
    root.number = 1;
    root.father_inode = 1;

    //printf("%x ", )

    strncpy(root.name, "/", sizeof(Inode::name));

    fwrite(&root, sizeof(Inode), 1, hd);

    space = sizeBlock - sizeof(Inode);

    genericInode.flag = false;
    genericInode.type = 2;
    genericInode.father_inode = 0;

    for(int i = 2; i <= inodeBlocks; i++){
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
    fflush(hd);
    fflush(stdin);

    for(int i = 0; i < inodeBlocks; i++){
        if(space < sizeof(Inode)){
            fseek(hd, space, SEEK_CUR);
            space = sizeBlock;
        }
        fread(&inodes[i], sizeof(Inode), 1, hd);
        cout<<endl;
        printInodes(inodes[i]);
        space-=sizeof(Inode);
    }

    cout << inodeBlocks << endl;
    cout << numBlocks << endl;

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
    //cout << sizeof(Inode) << endl;
}

void printSuperBlock(SuperBlock sp){
    printf("Magic: %s\n", sp.magicNumber);
    printf("Nome: %s\n", sp.hdName);
    printf("BlockSize: %d\n", sp.blockSize);
    printf("Num Blocks: %d\n", sp.numBlocks);
    printf("Free Blocks: %d\n", sp.numFreeBlocks);
    printf("First Inode: %d\n", sp.firstInodeBlock);
    printf("First Data: %d\n", sp.firstDataBlock);
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
    configFile = fopen(".config", "r");

    fread(hdList, sizeof(Config), 20, configFile);

    for(int i = 0; i < 20 && hdList[i].nomeHD[0] != 0; i++){
        cout<<"\nNome do HD: "<<hdList[i].nomeHD<<endl
            <<"Tamanho do Bloco: "<<hdList[i].blockSize<<" bytes"<<endl
            <<"Numero de Blocos: "<<hdList[i].numBlocks<<" blocos"<<endl;
    }
    
    fclose(configFile);
}

void formathd(){
    nomeHD = params[0]+".mvpfs";

    hd = fopen(nomeHD.c_str(), "r");
    fread(&superBlock, sizeof(SuperBlock), 1, hd);
    fclose(hd);
    remove(nomeHD.c_str());


    sizeBlock_s = to_string(superBlock.blockSize);
    params[1] = sizeBlock_s;

    sizeBlock_s = to_string(superBlock.numBlocks);
    params[2] = sizeBlock_s;

    createhd();
}

void typehd(){

    nomeHD = params[0] + ".mvpfs";
    configFile = fopen(".config", "r");
    fflush(hd);
    fflush(stdin);
    fseek(configFile, 0, SEEK_SET);
    fread(hdList, sizeof(Config), 20, configFile);

    int x;

    // for(int i = 0; i < 20; i++){
    //     cout<<"Nome: "<<hdList[i].nomeHD<<endl
    //         <<"Size: "<<hdList[i].blockSize<<endl
    //         <<"Blocks: "<<hdList[i].numBlocks<<endl;
    // }

    // for(x = 0; hdList[x].nomeHD != params[0]; x++);

    // sizeBlock = hdList[x].blockSize;
    // numBlocks = hdList[x].numBlocks;
    sizeBlock = 64;
    numBlocks = 10;

    //cout<<"SizeBlock: "<<sizeBlock<<"\nnumBlocks: "<<numBlocks<<endl;

    char * buffer = (char *) malloc(sizeof(char) * sizeBlock * numBlocks);

    hd = fopen(nomeHD.c_str(), "r");
    fflush(stdin);
    fflush(hd);
    fseek(hd, 0, SEEK_SET);
    //fscanf(hd, "%d", buffer);

    int id = 0;
    while(id < sizeBlock * numBlocks){
        fread(&buffer[id], sizeof(char), 1, hd);
        id++;
    }

    for(int i = 0; i < sizeBlock * numBlocks; i++){
        if(buffer[i] != 0)
            //if(buffer[i] > 32 && buffer[i] < 126)
                printf("%x ", buffer[i]);
            //else
            //    printf("? ");
    }
    cout<<endl;

    for(int i = 0; i < sizeBlock * numBlocks; i++){
        if(buffer[i] != 0)
            if(buffer[i] > 32 && buffer[i] < 126)
                printf("%c ", buffer[i]);
            else
                printf("? ");
    }

    cout<<endl;

    free(buffer);
}