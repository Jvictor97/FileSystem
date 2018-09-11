struct Inode{

public:
    Inode(){
        for(int i = 0; i < 7; i++){
            blocks[i] = 0;
        }

        father_inode = 0;
        name[0] = '\0';
        type = 2;
    }

    bool flag; // 0 - inativo(deletado) | 1 - ativo
    char name[26];
    uint8_t type; // 0 - diretório | 1 - arquivo | 2 - undefined
    uint32_t number;
    uint32_t father_inode; // 0 = inativo
    uint32_t blocks[7];
} __attribute__((packed));

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