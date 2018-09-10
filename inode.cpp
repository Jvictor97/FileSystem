struct Inode{

//public:
    // Inode(){
    //     // for(int i = 0; i < 7; i++){
    //     //     blocks[i] = -1;
    //     // }

    //     father_inode = -1;
    //     name[0] = '\0';
    //     type = 2;
    // }

    bool flag; // 0 - inativo(deletado) | 1 - ativo
    char type; // 0 - diretório | 1 - arquivo | 2 - undefined
    char number[4];
    char father_inode[4]; // -1 = inativo
    char name[24];
    char blocks[7*4];
};

// int main(){

//     Inode myInode[2];
//     myInode[0].flag = 1;
//     FILE *myfile = fopen("myFile", "w");
//     fwrite(&myInode[0], sizeof(Inode), 1, myfile);


//     printf("%c\n", myInode[0].flag);
    
//     fclose(myfile);
// }

// void create(){

//     Inode myInode;
//     myInode.flag = '0';
//     myInode.type = '0';
//     myInode.number = 0;
//     myInode.father_inode = 0; //INODE DO DIRETORIO PAI
//     strncpy(myInode.name, "nome do arquivo", sizeof(Inode::name));

// }