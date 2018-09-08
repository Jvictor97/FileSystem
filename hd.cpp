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

using namespace std;

string params[10];
string cmd;
int numBlocks;
int sizeBlock;
int inodeBlocks;
int bitmapBlocks;
string numBlocks_s;
string sizeBlock_s;
string nomeHD;
FILE * config; // Ponteiro para o arquivo de configuração de HDs
FILE * hd;    // Ponteiro para o HD que será criado
string content;
int blockNo;

typedef void (*functions)(void); // function pointer type

void createhd();
int strToInt(const string);
void writeSuperBlock();
void writeFixedBytesString(int, string);
void writeFixedBytesInt(int, string);
void exit();

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
        for (int block = 0; block < numBlocks - 1; block++)
        {
            for (int byte = 0; byte < sizeBlock; byte++)
            {
                fputc('0', hd);
                if (ferror(hd)) 
                    perror("Erro na escrita do arquivo");
            }
        }
        fclose(hd); 

        config = fopen(".config", "a"); // Abre ou cria o arquivo config

        nomeHD = nomeHD.substr(0, nomeHD.find(".mvpfs"));
        fputs(nomeHD.c_str(), config);
        fputc('|', config);
        fputs(sizeBlock_s.c_str(), config);
        fputc('|', config);
        fputs(numBlocks_s.c_str(), config);
        fputc(';', config);
        nomeHD+=".mvpfs";
        fclose(config);

        return;
    }
}

void exit(){
	if(hd != NULL)
		fclose(hd);
	return;
}

void writeSuperBlock(){
    fputs("bXZwZnM=", hd);              // Magic Number
    writeFixedBytesString(32, nomeHD.substr(0, nomeHD.find(".mvpfs")));  // Nome do HD
    writeFixedBytesInt(4, sizeBlock_s); // Tamanho de cada bloco
    writeFixedBytesInt(4, numBlocks_s); // Número de blocos
    writeFixedBytesInt(4, numBlocks_s); // Número de blocos livres
    writeFixedBytesInt(4, "1");

    inodeBlocks = 0.03 * numBlocks;
    string inodeIndex = to_string(1 + inodeBlocks);
    
    writeFixedBytesInt(4, inodeIndex);
    
    for(bitmapBlocks = 1; numBlocks - (1 + inodeBlocks + bitmapBlocks) > sizeBlock * 8 * bitmapBlocks; bitmapBlocks++);
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

void writeFixedBytesString(int bytes, string str){
    int numZeros = bytes - strlen(str.c_str());
    fputs(str.c_str(), hd);

    for(int i = 0; i < numZeros; i++){
        fputc('0', hd);
    }

}

void writeFixedBytesInt(int bytes, string str){
    int numZeros = bytes - strlen(str.c_str());
    for(int i = 0; i < numZeros; i++){
        fputc('0', hd);
    }
    fputs(str.c_str(), hd);
}