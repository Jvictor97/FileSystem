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

// int main(){

//     BitMapDataBlock bitMapDB(100);
//     FILE* file = fopen("fileHD", "w+");
//     fwrite(bitMapDB.bitMapArray, bitMapDB.nBlocks, 1, file);

//     cout << sizeof(bitMapDB.bitMapArray) << endl;

//     fclose(file);
//     // cout << bitMap.nBlocks <<endl;
//     // int i = 0;
//     // for(i = 0; i < bitMap.nBlocks; i++){
//     //     cout << bitMap.bitMapArray[i] << endl;
//     // }
//     // cout << i << endl;
    
//     return 0;
// }