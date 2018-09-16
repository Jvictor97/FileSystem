#include <iostream>

using namespace std;

// Classe para criação do bitmap de Datablocks
class BitMapDataBlocks{

    
    public:
    void setSize(int num){
        numBlocks = num; 
        bitMapArray = (char*)malloc(sizeof(char) * num);
        for(int i = 0; i < num; i++){
            bitMapArray[i] = 0;
        }
    }

    int numBlocks; // Numero de blocos de Datablock
    char *bitMapArray; // Array booleano com a quantidade de Datablocks
};