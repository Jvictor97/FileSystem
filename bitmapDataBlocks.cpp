#include <iostream>

using namespace std;

// Classe para criação do bitmap de Datablocks
class BitMapDataBlocks{

    
    public:
    void setSize(int num){
        numBlocks = num; 
        bitMapArray = (bool*)malloc(sizeof(bool) * num);
    }

    int numBlocks; // Numero de blocos de Datablock
    bool *bitMapArray; // Array booleano com a quantidade de Datablocks
};

// struct Inode{

// public:
//     Inode(){
//         for(int i = 0; i < 7; i++){
//              blocks[i] = -1;
//         }

//          father_inode = -1;
//          name[0] = '\0';
//          type = 2;
//     }

//     bool flag; // 0 - inativo(deletado) | 1 - ativo
//     char type; // 0 - diretório | 1 - arquivo | 2 - undefined
//     char number[4];
//     char father_inode[4]; // -1 = inativo
//     char name[24];
//     char blocks[7*4];
// };

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
    uint8_t type; // 0 - diretório | 1 - arquivo | 2 - undefined
    uint32_t number;
    uint32_t father_inode; // -1 = inativo
    char name[24];
    uint32_t blocks[7];
};

int main(){
  
    Inode str;

    cout << sizeof(str) << endl;

   return 0;
}