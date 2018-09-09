class SuperBlock{

public:

    SuperBlock(){
        magicNumber[0] = '\0';
        hdName[0] = '\0';
    }
    char magicNumber[8];
    char hdName[32];
    uint32_t blockSize;
    uint32_t numBlocks;
    uint32_t numFreeBlocks;
    uint32_t firstInodeBlock;
    uint32_t numInodeBlocks;
    uint32_t firstDataBlock;

};

// int main(){

//     FILE *file = fopen("meuTestaSuperBlock","w");
//     SuperBlock mySB;
//     SuperBlock mySB2;

//     strncpy(mySB.magicNumber, "bXZwZnM=", sizeof(SuperBlock::magicNumber));
//     mySB.blockSize = 256;
//     mySB.nBlocks = 1024;
//     mySB.nFreeBlocks = 1023;
//     mySB.firstInode = 2;
//     mySB.firstDataBlock = 30;  
//     strncpy(mySB.hdName, "meuHD", sizeof(SuperBlock::hdName));

//     fwrite(mySB.magicNumber, sizeof(SuperBlock::magicNumber), 1, file);
//     fwrite(mySB.hdName, sizeof(SuperBlock::hdName), 1, file);

//     fclose(file);

//     FILE* file2 = fopen("meuTestaSuperBlock", "r");

//     fread(mySB2.magicNumber, sizeof(SuperBlock::magicNumber), 1, file2);
//     fread(mySB2.hdName, sizeof(SuperBlock::hdName), 1, file2);

//     printf("%s\n", mySB2.magicNumber);
//     printf("%s\n", mySB2.hdName);
    
//     fclose(file2);
// }