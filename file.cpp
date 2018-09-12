#include <iostream>
#include "globals.h"

using namespace std;

void create(){

    string fileName = params[0];

    //char buffer[blockSize * 7];
    
    string content;
    getline(cin, content);
    
    if(content.size() > (sizeBlock * 7)){
        cout<<RED<<"O arquivo é muito grande" << endl;
        return;
    }

    int i;

    // Encontra o proximo Inode disponivel
    for(i = 0; i < totalInodes && inodes[i].flag != 0; i++);

    int amtBlocks = ceil(content.size() / sizeBlock);

    inodes[i].flag = 1;
    inodes[i].type = 2;
    strncpy(inodes[i].name, fileName.c_str(), sizeof(Inode::name));
    inodes[i].father_inode = actualInode.number;    
    
    int j, k;

    // Encontra o proximo DataBlock disponivel
    for(j = 0; j < amtBlocks; j++){
        for(k = 0; bitmapDataBlocks.bitMapArray[k] && k < numDataBlocks; k++);
        bitmapDataBlocks.bitMapArray[k] = 1;
        inodes[i].blocks[j] = k + superBlock.firstDataBlock;
   }

    for(j = 0; j < amtBlocks; j++){
        strncpy(datablocks[inodes[i].blocks[j] - superBlock.firstDataBlock], content.substr(0,sizeBlock).c_str(), sizeBlock);
        content = content.substr(sizeBlock, content.size());
    }
}

