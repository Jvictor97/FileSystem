#ifndef HD_CPP
#define HD_CPP

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

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

// Classes para manipulação do HD
#include "inode.cpp"
#include "superblock.cpp"
#include "config.cpp"
#include "bitmapDataBlocks.cpp"
// Header para declaração de variaveis globais
#include "globals.h"

// Classe para manipulação de arquivos
#include "file.cpp"

// Classe para manipulação de diretórios
#include "dir.cpp"

#include "help.cpp"

using namespace std;

string params[10];
string cmd;
int numBlocks;
int sizeBlock;
int inodeBlocks;
int bitmapBlocks;
int numDataBlocks;
int inodesPerBlock;
int totalInodes;
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
Inode actualInode;
string location;
bool formatado = false;

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
void printBitmap(char *);
void updateHD();
void exitHD();
void getclause();
void printDataBlocks();

// Cria um arquivo com o nome 'nomeHD', o número de blocos 
// 'numBlocks' e cada bloco com tamanho 'sizeBlock'
void createhd()
{
    // TODO: criar uma mensagem para printar ao usuário o tamanho máximo de arquivo
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
        perror(RED "Erro na criacao do arquivo" RESET);
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

        numDataBlocks = numBlocks - 1 - inodeBlocks - bitmapBlocks;

        writeBitmapBlocks();
        
        fflush(hd);
        fseek(hd, 0, SEEK_END);

        for (int block = 0; block < numDataBlocks; block++)
        {
            for (int byte = 0; byte < sizeBlock; byte++)
            {
                fputc(0, hd);
                if(ferror(hd)) 
                    perror(RED "Erro na escrita do arquivo" RESET);
            }
        }
        fclose(hd); 


        if(!formatado){
            string shortHdName = nomeHD.substr(0, nomeHD.find(".mvpfs"));

            strncpy(config.nomeHD, shortHdName.c_str(), sizeof(Config::nomeHD));
            config.blockSize = sizeBlock;
            config.numBlocks = numBlocks;
            
            configFile = fopen(".config", "a"); // Abre ou cria o arquivo config

            fwrite(&config, sizeof(Config), 1, configFile);
            fclose(configFile);
            formatado = false;
        }
        return;
    }
}

void selecionaHD(){
    // ÁREA DE MAPEAMENTO DE FUNÇÕES DE ESCOPO DO HD
	map<std::string, functions> localMap;
    // Entrega 1:
    localMap["createdir"] = createdir;
    localMap["cd"] = cd;
    localMap["dir"] = dir;
    localMap["renamedir"] = renamedir;
    localMap["create"] = create;
    localMap["remove"] = removeFile;
    localMap["type"] = type;
	localMap["exit"] = exitHD;

    // Entrega 2:
    localMap["rename"] = rename;
    localMap["copy"] = copy;
    localMap["help"] = help;
    // FIM DA ÁREA DE MAPEAMENTO

    hd = fopen(nomeHD.c_str(), "r+");

	if(hd == NULL){
		cout<<RED<<"ERRO: Nenhum HD encontrado com este nome...\n"<<RESET;
		return;
	}

    fread(&superBlock, sizeof(SuperBlock), 1, hd);

    // REMOVER PRINT SUPERBLOCK
    //cout<<"\nSuperBlock: "<<endl;
    // printSuperBlock(superBlock);

    numBlocks = superBlock.numBlocks;
    sizeBlock = superBlock.blockSize;
    inodeBlocks = superBlock.numInodeBlocks;

    int offsetInodeBlock = superBlock.firstInodeBlock * sizeBlock;

    fseek(hd, offsetInodeBlock, SEEK_SET);

    inodesPerBlock = floor(sizeBlock / sizeof(Inode));

    // REMOVER INODES PER BLOCK
    // cout<<"\ninodesPerBlock: "<<inodesPerBlock<<endl;

    totalInodes = inodesPerBlock * inodeBlocks;
    
    inodes = (Inode *) malloc(sizeof(Inode) * inodesPerBlock * inodeBlocks);

    int space = sizeBlock;

    for(int i = 0; i < totalInodes; i++){
        if(space < sizeof(Inode)){
            fseek(hd, space, SEEK_CUR);
            space = sizeBlock;
        }
        fread(&inodes[i], sizeof(Inode), 1, hd);
        //printInodes(inodes[i]);
        space-=sizeof(Inode);
    }

    // REMOVER PRINT INODES
    // cout<<"\nInodes: "<<endl;
    // for(int i = 0; i < totalInodes; i++){
    //     printInodes(inodes[i]);
    // }

    // Seta o Inode atual como o Root
    actualInode = inodes[0];
    
    // Calcula o número de bitmap blocks 
    for(bitmapBlocks = 1; numBlocks - (1 + inodeBlocks + bitmapBlocks) > sizeBlock * bitmapBlocks; bitmapBlocks++);

    // Calcula o número de datablocks
    numDataBlocks = numBlocks - 1 - inodeBlocks - bitmapBlocks;
    // Inicializa a variável global de bitmap
    bitmapDataBlocks.setSize(numDataBlocks);

    // Lê do arquivo para a variável o bitmap
    fread(bitmapDataBlocks.bitMapArray, sizeof(bool), numDataBlocks, hd); 

    // REMOVER PRINT BITMAP:
    // cout<<"Bitmap: "<<endl;
    // printBitmap(bitmapDataBlocks.bitMapArray);

    int offsetDataBlock = superBlock.firstDataBlock * sizeBlock;

    fseek(hd, offsetDataBlock, SEEK_SET);

    datablocks = (char**) malloc(sizeof(char*) * numDataBlocks);

    for(int i = 0; i < numDataBlocks; i++){
        datablocks[i] = (char*) malloc(sizeof(char) * sizeBlock);
        fread(datablocks[i], sizeBlock, 1, hd);
    }
    
    // REMOVER PRINT DATABLOCKS:
    // cout<<"\nDataBlocks:\n";
    // printDataBlocks();

    fclose(hd);

    string simpleHdName = nomeHD.substr(0, nomeHD.find(".mvpfs"));
    location = "/";

    while(cmd != "exitHD"){
		cout<<CYAN<<simpleHdName<<RESET<<":"<<location<<"> ";
		getclause();

        if(cmd == "")
            printf("");
        else if(localMap.find(cmd.c_str()) == localMap.end())
            cout<<RED<<"\nERRO: Esta funcao nao existe...\n\n"<<RESET;
        else{
            localMap[cmd]();
            cmd = cmd != "exit" ? "" : "exitHD";
        }
	}
    
}

void exit(){
	if(hd != NULL)
		fclose(hd);
	exit(0);
}

void writeSuperBlock(){

    strncpy(superBlock.magicNumber, "bXZwZnM", sizeof(SuperBlock::magicNumber));
    strncpy(superBlock.hdName, nomeHD.c_str(), sizeof(SuperBlock::hdName));
    superBlock.blockSize = sizeBlock;
    superBlock.numBlocks = numBlocks;
    superBlock.firstInodeBlock = 1;

    inodeBlocks = floor(0.3 * numBlocks);
    
    superBlock.numInodeBlocks = inodeBlocks;

    for(bitmapBlocks = 1; numBlocks - (1 + inodeBlocks + bitmapBlocks) > sizeBlock * bitmapBlocks; bitmapBlocks++);
    
    superBlock.firstDataBlock = inodeBlocks + bitmapBlocks + 1;

    // Número de blocos livres = número de datablocks
    superBlock.numFreeBlocks = numBlocks - 1 - inodeBlocks - bitmapBlocks;

    fflush(stdin);
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
    root.type = 1;
    root.number = 1;
    root.father_inode = 1;

    //printf("%x ", )
    inodesPerBlock = floor(sizeBlock / sizeof(Inode));

    strncpy(root.name, "/", sizeof(Inode::name));

    fwrite(&root, sizeof(Inode), 1, hd);

    space = sizeBlock - sizeof(Inode);

    genericInode.flag = false;
    genericInode.type = 0;
    genericInode.father_inode = 0;

    totalInodes = inodeBlocks * inodesPerBlock;

    int i;

    for(i = 2; i <= totalInodes; i++){
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

    // cout<<"\nTotal de Inodes: "<<i - 1<<endl;

    // CÓDIGO PARA EXIBIR A ESTRUTURA DOS INODES GRAVADOS

    // inodes = (Inode *) malloc(sizeof(Inode) * inodesPerBlock * inodeBlocks);
    // fseek(hd, sizeof(SuperBlock), SEEK_SET);
    // fflush(hd);
    // fflush(stdin);

    
    // for(int i = 0; i < inodeBlocks; i++){
    //     if(space < sizeof(Inode)){
    //         fseek(hd, space, SEEK_CUR);
    //         space = sizeBlock;
    //     }
    //     fread(&inodes[i], sizeof(Inode), 1, hd);
    //     cout<<endl;
    //     printInodes(inodes[i]);
    //     space-=sizeof(Inode);
    // }

    // cout << inodeBlocks << endl;
    // cout << numBlocks << endl;
    // FIM DA EXIBIÇÃO
}

void writeBitmapBlocks(){
    bitmapDataBlocks.setSize(numDataBlocks);
    fwrite(bitmapDataBlocks.bitMapArray, bitmapDataBlocks.numBlocks, 1, hd);

    int space = (bitmapBlocks * sizeBlock) - numDataBlocks;

    //cout<<"\nSpace Bitmap: "<<space<<endl;

    for(int i = 0; i < space; i++){
        fputc(0, hd);
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
    //cout << sizeof(Inode) << endl;
}

void printSuperBlock(SuperBlock sp){
    printf("Magic: %s\n", sp.magicNumber);
    printf("Nome: %s\n", sp.hdName);
    printf("BlockSize: %d\n", sp.blockSize);
    printf("Num Blocks: %d\n", sp.numBlocks);
    printf("Free Blocks: %d\n", sp.numFreeBlocks);
    printf("First Inode: %d\n", sp.firstInodeBlock);
    printf("Total InodeBlocks: %d\n", sp.numInodeBlocks);
    printf("First Data: %d\n", sp.firstDataBlock);
}

// TODO: rever por inteiro
void updateHD(){
    hd = fopen(nomeHD.c_str(), "w");
    
    fwrite(&(superBlock), sizeof(SuperBlock), 1, hd);

    int space = sizeBlock - sizeof(SuperBlock);

    while(space--){
        fputc(0, hd);
    }

    fflush(hd);
    space = sizeBlock;
    for(int i = 0; i < totalInodes; i++){
        if(space == 0)
            space = sizeBlock;
        else
            if(sizeof(Inode) > space){
                for(int j = 0; j < space; j++){
                    fputc(0, hd);
                }
                space = sizeBlock;
            }
        fwrite(&inodes[i], sizeof(Inode), 1, hd);
        space -= sizeof(Inode);
    }

    fflush(hd);
    fflush(stdin);
    fseek(hd, 0, SEEK_CUR);

    //printBitmap(bitmapDataBlocks.bitMapArray);
    fwrite(bitmapDataBlocks.bitMapArray, sizeof(char), bitmapDataBlocks.numBlocks, hd);

    space = (bitmapBlocks * sizeBlock) - numDataBlocks;
    // cout<<"Numblocks Bitmap: "<<bitmapDataBlocks.numBlocks;

    fflush(hd);
    fflush(stdin);
    fseek(hd, 0, SEEK_CUR);
    while(space--){
        fputc(0, hd);
    }

    for(int i = 0; i < numDataBlocks; i++){
        for(int j = 0; j < sizeBlock; j++){
            fwrite(&datablocks[i][j], sizeof(char), 1, hd);
        }
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

    cout<<GREEN<<"\nO HD \""<<nomeHD<<"\" foi salvo com sucesso!"<<RESET<<endl<<endl;
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

    if(configFile == NULL){
        cout<<RED<<"\nERRO: nao foi identificado um arquivo .config"<<endl;
        cout<<YELLOW<<"Dica: crie um HD com o comando 'createhd' primeiro!\n\n"<<RESET;
        return;
    }

    fread(hdList, sizeof(Config), 20, configFile);

    printf(YELLOW "\n%-32s %-10s %-10s\n" RESET, "Nome", "Blocksize", "Numblocks");
    for(int i = 0; i < 20 && hdList[i].nomeHD[0] != 0; i++){
        printf("%-32s %-10d %-10d\n", hdList[i].nomeHD, hdList[i].blockSize, hdList[i].numBlocks);
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

    formatado = true;
    createhd();
}

void typehd(){
    int linhasPrintadas = 0;
    nomeHD = params[0] + ".mvpfs";
    configFile = fopen(".config", "r");
    fflush(hd);
    fflush(stdin);
    fseek(configFile, 0, SEEK_SET);
    fread(hdList, sizeof(Config), 20, configFile);
    fclose(configFile);

    int x;
    for(x = 0; hdList[x].nomeHD != params[0]; x++);

    sizeBlock = hdList[x].blockSize;
    numBlocks = hdList[x].numBlocks;

    hd = fopen(nomeHD.c_str(), "rb+");

    char ** buffer = (char **) malloc(sizeof(char*) * numBlocks);

    for(int i = 0; i < numBlocks; i++){
        buffer[i] = (char *) malloc(sizeof(char) * sizeBlock);
        fread(buffer[i], sizeBlock, 1, hd);
    }

    fflush(stdin);
    fflush(hd);
    fseek(hd, 0, SEEK_SET);

    //cout<<"Blocos: "<<numBlocks<<endl<<"size: "<<sizeBlock<<endl;

    bool printChar = false, printedAny = false;
    
    for(int i = 0; i < numBlocks; i++){
        if(!printChar)
            printf(YELLOW "%04d  " RESET, i);
        else
            printf("      ");
        for(int j = 0; j < sizeBlock; j++){
            if(buffer[i][j] != 0){
                printedAny = true;
                if(buffer[i][j] > 32 && buffer[i][j] < 126){
                    if(printChar){
                        printf("%-3c", buffer[i][j]);
                    }
                    else{
                        printf("%-3x", buffer[i][j]);  
                    }
                        
                }else{
                    printf("%-3c", '?');
                }    
            }
            if(j == sizeBlock - 1){
                if(printedAny){
                    linhasPrintadas++;
                    printf("\n");
                    printedAny = false;
                    if(!printChar){
                        i -= 1;
                        printChar = true;
                    }
                    else{
                        printChar = false;
                    }

                    if(linhasPrintadas % 30 == 0){
                        string auxiliar;
                        getline(cin, auxiliar);
                    }
                }
                else
                    if(i == numBlocks - 1)
                        printf("\r    ");
                    else 
                        printf("\r");
            }
        }
    }
    cout<<endl;

    fclose(hd);

    for(int i = 0; i < numBlocks; i++){
        free(buffer[i]);
    }

    free(buffer);
}

void printBitmap(char * arr){
    int i;
    for(i = 0; i < numDataBlocks; i++){
        printf("%d ", arr[i]);
    }

    cout<<"\nTotal de Datablocks: "<<i<<endl;
}

void printDataBlocks(){
    fflush(hd);
    fflush(stdin);

    //cout<<"\nNumDataBlocks: "<<numDataBlocks<<endl<<"SizeBlock: "<<sizeBlock<<endl;
    //return;

    for(int i = 0; i < numDataBlocks; i++){
        for(int j = 0; j < sizeBlock; j++){
            printf("%x ", datablocks[i][j]);
        }
        printf("\n");
    }
}

void removehd(){
    nomeHD = params[0];

    configFile = fopen(".config", "r");

    if(configFile == NULL){
        cout<<RED<<"\nERRO: nao encontrado arquivo .config, caso algum HD exista o sistema esta corrompido!\n\n"<<RESET;
        return;
    }

    fread(hdList, sizeof(Config), 20, configFile);
    fclose(configFile);

    int i;
    for(i = 0; i < 20 && hdList[i].nomeHD != nomeHD; i++);

    if(hdList[i].nomeHD != nomeHD){
        cout<<RED<<"\nERRO: nenhum HD encontrado com o nome \""<<YELLOW<<nomeHD<<RED<<"\".\n\n"<<RESET;
        return;
    }

    for(int j = i; j < 19; j++){
        hdList[j] = hdList[j + 1];
    }

    configFile = fopen(".config", "w");
    fwrite(hdList, sizeof(Config), 20, configFile);

    string simpleHDname = nomeHD;

    nomeHD += ".mvpfs";

    remove(nomeHD.c_str());

    cout<<GREEN<<"\nO HD \""<<YELLOW<<simpleHDname<<GREEN<<"\" foi removido com sucesso!\n\n"<<RESET;
}

void statushd(){
    string simpleHDname = params[0];
    nomeHD = params[0] + ".mvpfs";
    hd = fopen(nomeHD.c_str(), "r");

    fread(&superBlock, sizeof(SuperBlock), 1, hd);

    numBlocks = superBlock.numBlocks;
    sizeBlock = superBlock.blockSize;
    inodeBlocks = superBlock.numInodeBlocks;

    int offsetInodeBlock = superBlock.firstInodeBlock * sizeBlock;

    fseek(hd, offsetInodeBlock, SEEK_SET);

    inodesPerBlock = floor(sizeBlock / sizeof(Inode));

    totalInodes = inodesPerBlock * inodeBlocks;
    
    inodes = (Inode *) malloc(sizeof(Inode) * inodesPerBlock * inodeBlocks);

    int space = sizeBlock;

    for(int i = 0; i < totalInodes; i++){
        if(space < sizeof(Inode)){
            fseek(hd, space, SEEK_CUR);
            space = sizeBlock;
        }
        fread(&inodes[i], sizeof(Inode), 1, hd);
        space-=sizeof(Inode);
    }

    int numFreeInodes = totalInodes;

    for(int i = 0; i < totalInodes; i++){
        if(inodes[i].flag != 0)
            numFreeInodes--;
    }

    printf(YELLOW "\n%-32s %-6s %-7s %-6s %-13s %-13s\n" RESET, "Nome do HD", "Blocos", "Tamanho", "Inodes", "Blocos-Livres", "Inodes-Livres");
    printf("%-32s %-6d %-7d %-6d %-13d %-13d\n\n" RESET, nomeHD.c_str(), numBlocks, sizeBlock, totalInodes, superBlock.numFreeBlocks, numFreeInodes);
}

#endif