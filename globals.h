#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>

using namespace std;

extern string params[10];
extern string cmd;
extern int numBlocks;
extern int sizeBlock;
extern int inodeBlocks;
extern int bitmapBlocks;
extern int numDataBlocks;
extern int inodesPerBlock;
extern int totalInodes;
extern string numBlocks_s;
extern string sizeBlock_s;
extern string nomeHD;
extern FILE * configFile; // Ponteiro para o arquivo de configuração de HDs
extern FILE * hd;    // Ponteiro para o HD que será criado
extern SuperBlock superBlock;
extern BitMapDataBlocks bitmapDataBlocks;
extern Inode genericInode;
extern Inode * inodes;
extern char ** datablocks;
extern Config config;
extern Config hdList[20];
extern Inode actualInode;
extern string location;


#endif