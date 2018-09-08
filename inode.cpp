#include <iostream>
#include <cstdio>
#include <cstdint>
#include <string.h>
using namespace std;


class Inode{

public:
    uint8_t flag;
    uint8_t type;
    uint32_t number;
    uint32_t father_inode;
    char name[24];
    uint32_t blocks[7];
};

int main(){

    Inode myInode[2];
    myInode[0].flag = 1;
    FILE *myfile = fopen("myFile", "w");
    fwrite(&myInode[0], sizeof(Inode), 1, myfile);


    printf("%c\n", myInode[0].flag);
    
    fclose(myfile);
}

void create(){

    Inode myInode;
    myInode.flag = '0';
    myInode.type = '0';
    myInode.number = 0;
    myInode.father_inode = 0; //INODE DO DIRETORIO PAI
    strncpy(myInode.name, "nome do arquivo", sizeof(Inode::name));

}