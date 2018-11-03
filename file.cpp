#ifndef FILE_CPP
#define FILE_CPP

#include <iostream>
#include <string>
#include "globals.h"
#include <string.h>

using namespace std;

bool movingFile = false;
int definedCreateInode;

void printInod(Inode i){
    printf("Flag: %d\n", i.flag);
    printf("Type: %d\n", i.type);
    printf("Number: %d\n", i.number);
    printf("Father: %d\n", i.father_inode);
    printf("Name: %s\n", i.name);

    for(int j = 0; j < 7; j++){
        printf("Block[%d]: %d\n", j, i.blocks[j]);
    }
    cout<<endl;
}

void create(){

    string fileName = params[0];

    int aux;
    for(aux = 0; aux < totalInodes; aux++){
        if(inodes[aux].type == 2
            && inodes[aux].name == fileName 
            && inodes[aux].father_inode == actualInode.number)
        {
            cout<<RED<<"\nERRO: O arquivo \""<<YELLOW<<fileName<<RED<<"\""<<" ja existe neste diretorio\n\n";
            return;
        }
    }

    

    string content;
    getline(cin, content);
    
    if(content.size() > (sizeBlock * 7)){
        cout<<RED<<"O arquivo é muito grande" << endl;
        return;
    }


    int i;

    // Encontra o proximo Inode disponivel
    for(i = 0; i < totalInodes && inodes[i].flag != 0; i++);

    //TODO: diminuir a quantia de blocos livres no superblock em amtBlocks
    int amtBlocks = ceil((double)content.size() / (double)sizeBlock);

    inodes[i].flag = 1;
    inodes[i].type = 2;
    strncpy(inodes[i].name, fileName.c_str(), sizeof(Inode::name));
    inodes[i].father_inode = actualInode.number;

    int actual;
	for(actual = 0; actualInode.blocks[actual] != 0; actual++){
		if(actual > 6){
			cout<<RED<<"\nERRO: Numero máximo de blocos de endereco utilizado...\n"<<RESET;
			cout<<YELLOW<<"Dica: Apague algum arquivo/diretorio ou formate seu HD para liberar espaco!\n\n"<<RESET;
            inodes[i].initialize();
			return;
		}
	}

    inodes[actualInode.number - 1].blocks[actual] = actualInode.blocks[actual] = inodes[i].number;

    int j, k;

    // Encontra o proximo DataBlock disponivel
    for(j = 0; j < amtBlocks; j++){
        for(k = 0; bitmapDataBlocks.bitMapArray[k] && k < numDataBlocks; k++);
        bitmapDataBlocks.bitMapArray[k] = 1;
        inodes[i].blocks[j] = k + superBlock.firstDataBlock;
    }

    for(j = 0; j < amtBlocks; j++){
        strncpy(datablocks[inodes[i].blocks[j] - superBlock.firstDataBlock], content.substr(0,sizeBlock).c_str(), sizeBlock);
        if(content.size() > sizeBlock)
            content = content.substr(sizeBlock);
    }
    // Atualiza o superblock com o número de blocos livres
    superBlock.numFreeBlocks -= amtBlocks;
}


void removeFile(){
	string filename = params[0];
    int amtBlocks = 0;

	for(int i = 0; i < 7; i++){
		if(actualInode.blocks[i] != 0){
			Inode child = inodes[actualInode.blocks[i] - 1];
			// Se for um inode de arquivo
			if(child.type == 2
			&& child.flag == 1 
			&& child.name == filename){
				for(int j = 0; j < 7; j++){
					if(child.blocks[j] != 0){
                        amtBlocks++;
                        for(int b = 0; b < sizeBlock; b++){
                            datablocks[child.blocks[j] - superBlock.firstDataBlock][b] = 0;
                        }
						bitmapDataBlocks.bitMapArray[child.blocks[j] - superBlock.firstDataBlock] = 0;
                        inodes[actualInode.number - 1].blocks[i] = actualInode.blocks[i] = 0;
                        //printInod(actualInode);
					}
				}
                superBlock.numFreeBlocks += amtBlocks;
				inodes[child.number - 1].initialize();
                if(!movingFile && !movingDir)
			    	cout<<GREEN<<"\nArquivo \""<<YELLOW<<filename<<GREEN<<"\" removido com sucesso!\n\n";
				return;
			}
		}
	}

	cout<<RED<<"\nERRO: nenhum arquivo com o nome \""<<YELLOW<<filename<<RED<<"\" neste caminho.\n\n";
}                                                   


//TODO: Validar o conteúdo de arquivos de mesmo nome em outro nível de diretorio
void type(){
	string filename = params[0];
    fflush(stdin);

	for(int i = 0; i < 7; i++){
		if(actualInode.blocks[i] != 0){
			Inode child = inodes[actualInode.blocks[i] - 1];
			
			if(child.type == 2 // Se for um inode de arquivo
			&& child.flag == 1 // Ativo
			&& child.name == filename // Cujo nome seja o filename
            && child.father_inode == actualInode.number // E esta no diretorio atual
            ){ 
				for(int j = 0; j < 7; j++){
					if(child.blocks[j] != 0){
                        for(int p = 0; p < sizeBlock; p++){
						    printf(MAGENTA "%c" RESET, datablocks[child.blocks[j] - superBlock.firstDataBlock][p]);
                        }
                        printf("\n");
					}
				}
				return;
			}
		}
	}

	cout<<RED<<"\nERRO: nenhum arquivo com o nome \""<<YELLOW<<filename<<RED<<"\" neste caminho.\n\n";
}

void rename(){
    string currentName = params[0];
    string newName = params[1];

    int fileInode = 0;
    bool newNameExists = false;
    
    for(int i = 0; i < 7; i++){
        if(actualInode.blocks[i] != 0){
			Inode child = inodes[actualInode.blocks[i] - 1];
			
			if(child.type == 2 // Se for um inode de arquivo
			&& child.flag == 1 // Ativo
            && child.father_inode == actualInode.number // E esta no diretorio atual
            ){ 
			    // Cujo nome seja o currentName
                if(child.name == currentName){
                    fileInode = child.number;
                }
                if(child.name == newName)
                    newNameExists = true;
            }
        }
    }

    if(fileInode && !newNameExists){
        // Renomeia o arquivo 
        strncpy(inodes[fileInode - 1].name, newName.c_str(), sizeof(Inode::name));
        // actualInode = inodes[actualInode.number - 1];
    }
    else   
        if(!fileInode)
        	cout<<RED<<"\nERRO: nenhum arquivo com o nome \""<<YELLOW<<currentName<<RED<<"\" neste caminho.\n\n";
        else
            cout<<RED<<"\nERRO: ja existe um arquivo com o nome \""<<YELLOW<<newName<<RED<<"\" neste caminho.\n\n";
}

void copy(){
    string curName = params[0];
    string newName;
    char * stringPath = (char *) malloc(sizeof(char) * params[1].length());
    Inode searchInode;
    int fileInode = 0;

    for(int i = 0; i < 7; i++){
        if(actualInode.blocks[i] != 0){
            Inode child = inodes[actualInode.blocks[i] - 1];

            if(child.flag == 1 // Se o inode estiver ativo
            && child.type == 2 // For de tipo arquivo
            && strcmp(child.name, params[0].c_str()) == 0 // Tiver o nome dado por params[0]
            && child.father_inode == actualInode.number) // E que seja filho de actualInode
            {
                fileInode = child.number;
            }
        }
    }

    if(!fileInode){
        cout<<RED<<"\nERRO: nenhum arquivo com o nome \""<<YELLOW<<params[0]<<RED"\" no caminho atual.\n\n";
        return;
    }

    Inode createInode; // declarado createInode

    if(definedCreateInode == -1){

        if(params[1].back() == '/')
            newName = curName;
        else{
            int lastFolderEnd = params[1].rfind("/");
            int currFolderSign = params[1].rfind("./");

            if(lastFolderEnd == -1)
                newName = params[1];
            else{
                    newName = params[1].substr(lastFolderEnd + 1);
                    params[1].erase(lastFolderEnd + 1, params[1].length() - 1);
                    //cout<<"\nPath: "<<params[1]<<endl;
                    //cout<<"\nNome: "<<newName<<endl;
            }
        }

        // INICIO REMOVER

        // cout<<"Nome Atual: "<<curName<<endl;
        // cout<<"Novo Nome: "<<newName<<endl;
        // cout<<"Caminho: "<<params[1]<<endl;
        // exit(0);

        // FIM REMOVER

        if(params[1][0] == '/'){
            // Copia para o path baseado no root
            strcpy(stringPath, params[1].substr(1).c_str());
            searchInode = inodes[0];
            //printf("Baseado no ROOT\n");
        }
        else if(params[1][0] == '.' && params[1][1] == '/'){
            // Copia para o path baseado no inode atual
            strcpy(stringPath, params[1].substr(2).c_str());
            searchInode = actualInode;
            //printf("Baseado no ATUAL com ponto\n");
        }else{
            // Copia para o path baseado no inode atual    
            strcpy(stringPath, params[1].c_str());
            searchInode = actualInode;
            //printf("Baseado no ATUAL\n");
        }

        //printf("StringPath: \"%s\"\n", stringPath);

        char * path;
        string fullPath; 

        fullPath = stringPath;
        path = strtok(stringPath, "/");


        while(path != NULL && fullPath.find("/") != -1 && definedCreateInode == -1){
            //cout<<"\nEntrou no while"<<endl;
            //cout<<path<<endl;
            //cout<<"Path: "<<path<<endl;
            int i;
            for(i = 0; i < 7; i++){
                if(searchInode.blocks[i] != 0 && searchInode.blocks[i] - 1 < totalInodes){
                    Inode child = inodes[searchInode.blocks[i] - 1];

                    if(child.type == 1 // Se for um inode de diretorio
                    && child.flag == 1 // Ativo
                    && strcmp(child.name, path) == 0 // Que possui o nome de path
                    && child.father_inode == searchInode.number // E esta no diretorio atual
                    ){ 
                        searchInode = child;
                        break;
                    }
                }
            }
            if(i == 7){
                cout<<RED<<"\nERRO: o caminho \""<<YELLOW<<params[1]<<RED<<"\" nao foi localizado.\n\n";
                return;
            }
            path = strtok(NULL, "/");
        }

        createInode = searchInode;
    }    
    else{
        createInode = inodes[definedCreateInode - 1];
        newName = curName;
    }

    if(actualInode.number == createInode.number && curName == newName) {
        cout<<RED<<"\nERRO: o arquivo não pode ser copiado com o mesmo nome para o mesmo caminho.\n\n";
        return;
    }

    // printf("Nome do arquivo: %s\n", inodes[fileInode - 1].name);
    // printf("Nome pasta: %s\n", searchInode.name);

    // INICIO DO CREATE

    int blankBlock = -1;

    for(int aux = 0; aux < 7; aux++){
        if(createInode.blocks[aux] != 0){
            Inode child = inodes[createInode.blocks[aux] - 1];
            if(child.type == 2 // Se o tipo for arquivo
            && child.name == newName) // Com o mesmo nome do novo arquivo
            {
                cout<<RED<<"\nERRO: Um arquivo com nome \""<<YELLOW<<newName<<RED<<"\""<<" ja existe neste diretorio\n\n";
                return;
            }
        }
        else
            if(blankBlock == -1)
                blankBlock = aux; // Seleciona o bloco do inode Pasta em que o inode de arquivo será gravado        
    }

    if(blankBlock == -1){
        cout<<RED<<"\nERRO: Numero máximo de blocos de endereco utilizado...\n"<<RESET;
        cout<<YELLOW<<"Dica: Apague algum arquivo/diretorio ou formate seu HD para liberar espaco!\n\n"<<RESET;
        return;
    }

    // Concatena todo o conteúdo do arquivo original
    string content;
    Inode file = inodes[fileInode - 1];
    //cout<<"Nome destino: "<<createInode.name<<endl;
    //cout<<"Nome arquivo: "<<file.name<<endl;
    for(int n = 0; n < 7; n++){
        if(file.blocks[n] != 0){
            //cout<<datablocks[file.blocks[n] - superBlock.firstDataBlock];
            content += datablocks[file.blocks[n] - superBlock.firstDataBlock];
            //strcat(content, datablocks[file.blocks[n]]);
        }
    }

    int node;
    // Encontra o proximo Inode disponivel
    for(node = 0; node < totalInodes && inodes[node].flag != 0; node++);

    int amtBlocks = ceil((double) content.length() / (double)sizeBlock);

    // Inicia a criação do inode de arquivo
    inodes[node].flag = 1;
    inodes[node].type = 2;
    strncpy(inodes[node].name, newName.c_str(), sizeof(Inode::name));
    inodes[node].father_inode = createInode.number;

    int k;
    // Encontra o proximo DataBlock disponivel
    for(int j = 0; j < amtBlocks; j++){
        for(k = 0; bitmapDataBlocks.bitMapArray[k] && k < numDataBlocks; k++);
        bitmapDataBlocks.bitMapArray[k] = 1;
        // Insere no inode arquivo as referências de datablocks utilizados
        inodes[node].blocks[j] = k + superBlock.firstDataBlock;
    }

    // Insere no inode Pasta o número do filho (inode arquivo) no bloco dado por 'node'
    inodes[createInode.number - 1].blocks[blankBlock] = inodes[node].number;

    for(int j = 0; j < amtBlocks; j++){
        strncpy(datablocks[inodes[node].blocks[j] - superBlock.firstDataBlock], content.substr(0,sizeBlock).c_str(), sizeBlock);
        if(content.size() > sizeBlock)
            content = content.substr(sizeBlock);
    }
    // Atualiza o superblock com o número de blocos livres
    superBlock.numFreeBlocks -= amtBlocks;
    // Atualiza o inode atual
    actualInode = inodes[actualInode.number - 1];

    // FIM DO CREATE

    if(!movingFile && definedCreateInode == -1)
        cout<<GREEN<<"\nArquivo \""<<YELLOW<<curName<<GREEN"\" copiado com sucesso!\n\n"<<RESET;

    definedCreateInode = -1; // Seta novamente para -1 para permitir chamadas normais

    free(stringPath);
}

void move(){
    movingFile = true;

    copy();
    removeFile();

    cout<<GREEN<<"\nArquivo \""<<YELLOW<<params[0]<<GREEN"\" movido com sucesso!\n\n"<<RESET;
    movingFile = false;
}

void copyfrom(){
    string fileName = params[0];
    string realHdPath = params[1];
    int fileInode;

    for (int i = 0; i < 7; i++)
    {
        if(actualInode.blocks[i] != 0)
        {
            Inode child = inodes[actualInode.blocks[i] - 1];

            if(child.flag == 1 
            && child.type == 2 
            && strcmp(child.name, fileName.c_str()) == 0 
            && child.father_inode == actualInode.number)
            { 
                // Se for uma imagem
                if(params[1].rfind(".jpg") != -1 
                || params[1].rfind(".jpeg") != -1 
                || params[1].rfind(".png") != -1 
                || params[1].rfind(".bmp") != -1 )
                {
                    int con;
                    con = datablocks[child.blocks[0] - superBlock.firstDataBlock][i] - '0';
                    for(int i = 1; i < 24; i++){
                        con = (datablocks[child.blocks[0] - superBlock.firstDataBlock][i] - '0') + (con * 10);
                    }
                    int fileSize = con;
                    int amtRed = 24;
                    int it = 0;
                    char * imgContent = (char*) malloc(sizeof(char) * fileSize);
                   
                    for (int n = 0; n < 7; n++)
                    {
                        if(child.blocks[n] != 0)
                        {
                            while(amtRed < sizeBlock && it < fileSize){
                                imgContent[it] = datablocks[child.blocks[n] - superBlock.firstDataBlock][amtRed];
                                amtRed++;
                                it++;
                            }
                            amtRed = 0;
                        }
                    }

                    FILE * realFile = fopen(realHdPath.c_str(), "wb");
                    fwrite(imgContent, 1, fileSize, realFile);

                    fclose(realFile);
                    free(imgContent);
                }
                else{ // Caso seja um arquivo de texto
                    // Concatena todo o conteúdo do arquivo original
                    string content;
                    for (int n = 0; n < 7; n++)
                    {
                        if(child.blocks[n] != 0)
                        {
                            //cout<<datablocks[child.blocks[n] - superBlock.firstDataBlock];
                            content += datablocks[child.blocks[n] - superBlock.firstDataBlock];
                        }
                    }
                    FILE * realFile = fopen(realHdPath.c_str(), "w+b");

                    char * contentC = (char*)malloc(sizeof(char) * strlen(content.c_str()));

                    strcpy(contentC, content.c_str());

                    fwrite(contentC, 1, strlen(contentC), realFile);

                    fclose(realFile);
                    free(contentC);
                }
                
                cout<<GREEN<<"\nArquivo \""<<YELLOW<<fileName<<GREEN<<"\" transferido com sucesso para: \""<<YELLOW<<realHdPath<<GREEN"\".\n\n";
                return;
            }
        }
    }

    cout<<RED<<"\nERRO: o arquivo \""<<YELLOW<<params[0]<<RED<<"\" nao foi localizado.\n\n";
}

void copyto(){

    string hdPath = params[0];
    string newName = params[1];

    FILE* fileFromHD = fopen(hdPath.c_str(), "r+b");
    fflush(fileFromHD);
    fflush(stdin);
    size_t result;
    int fileSize = 0;

    fseek(fileFromHD, 0, SEEK_END); // Coloca a o ponteiro dentro do arquivo na ultima posição.
    fileSize = ftell(fileFromHD);   // Encontra o tamanho do arquivo
    rewind(fileFromHD);             // Volta o ponteiro para a posicao inicial

    //TODO: Validar se o arquivo cabe no HD (total do tamanho do HD)
    double myFileSize = ceil((double)fileSize / (double)sizeBlock);

    if(superBlock.numFreeBlocks < myFileSize ){
        cout << "ERRO O ARQUIVO NAO CABE NO HD!!" << endl;
        return;
    }

    //printf("%d\n", fileSize);
    //printf("%d\n", fileSize / sizeBlock);

    //TODO: Validar se o arquivo cabe em um inode
    if( (fileSize / sizeBlock) > 7){
        cout << "ERRO O ARQUIVO NAO CABE NO INODE" << endl;
        return;
    }

    int i;
    // Encontra o proximo Inode disponivel
    for(i = 0; i < totalInodes && inodes[i].flag != 0; i++);

    if(i == totalInodes){
        cout << "ERRO TODOS INODES UTILIZADOS" << endl;
        return;
    }

    // Cria um buffer com o tamanho do arquivo
    char *contentChar = (char*)malloc(sizeof(char) * fileSize);
    //char contentChar[20000];

    if(fileFromHD == NULL){
        cout<< "ERRO NAO ABRIU" << endl;
        return;
    }

    result = fread(contentChar, 1, fileSize, fileFromHD);
    fclose(fileFromHD);

    if(result != fileSize){
        cout << "ERRO NO TAMANHO DO ARQUIVO" << endl; 
        return;
    }

    //string path= "c:/users/joao.souza/desktop/P/amazon.png";
    // FILE * fout = fopen(path.c_str(), "wb");
    
    // fwrite(contentChar, 1, fileSize, fout);
    // fclose(fout);
    // return;

    //string content = contentChar;

    //cout <<"CONTEUDO: " << content << endl;
    //cout<<"Red Bytes: " <<result<<endl;
    //cout<<"Size do arq: "<<fileSize<<endl;

    int amtBlocks = ceil((double)(fileSize + 24)/ (double)superBlock.blockSize);

    int actual;
	for(actual = 0; actualInode.blocks[actual] != 0; actual++){
		if(actual > 6){
			cout<<RED<<"\nERRO: Numero máximo de blocos de endereco utilizado...\n"<<RESET;
			cout<<YELLOW<<"Dica: Apague algum arquivo/diretorio ou formate seu HD para liberar espaco!\n\n"<<RESET;
            inodes[i].initialize();
			return;
		}
	}

    inodes[actualInode.number - 1].blocks[actual] = actualInode.blocks[actual] = inodes[i].number;

    int j, k;

    // Encontra o proximo DataBlock disponivel
    for(j = 0; j < amtBlocks; j++){
        for(k = 0; bitmapDataBlocks.bitMapArray[k] && k < numDataBlocks; k++);
            bitmapDataBlocks.bitMapArray[k] = 1;
            inodes[i].blocks[j] = k + superBlock.firstDataBlock;
    }

    int it = 0;
    int amtWrote = 0;

    if(params[0].rfind(".jpg") != -1 
    || params[0].rfind(".jpeg") != -1 
    || params[0].rfind(".png") != -1 
    || params[0].rfind(".bmp") != -1 )
    {
        string strFileSize = to_string(fileSize);
        size_t len = strFileSize.size();
        while(len < 24){
            strFileSize.insert(0, "0");
            len++;
        }
        while(amtWrote < 24){
            datablocks[inodes[i].blocks[0] - superBlock.firstDataBlock][amtWrote] = strFileSize[amtWrote];
            amtWrote++;
        }
    }

    for(j = 0; j < amtBlocks; j++){      
        while(amtWrote < sizeBlock && it < fileSize){
            datablocks[inodes[i].blocks[j] - superBlock.firstDataBlock][amtWrote] = contentChar[it];
            amtWrote++;
            it++;
        }
        amtWrote = 0;
        // strncpy(datablocks[inodes[i].blocks[j] - superBlock.firstDataBlock], content.substr(0,sizeBlock).c_str(), sizeBlock);
        // if(content.size() > sizeBlock)
        //     content = content.substr(sizeBlock);
    } 

    //cout<<"\nIterador: "<<it<<endl;

    // Seta o inode com suas flags essenciais
    inodes[i].flag = 1;
    inodes[i].type = 2;
    inodes[i].father_inode = actualInode.number;
    
    strcpy(inodes[i].name, newName.c_str());  

    superBlock.numFreeBlocks -= amtBlocks;

    // free(contentChar);
    cout<<GREEN<<"\nArquivo \""<<YELLOW<<params[0].substr(params[0].rfind('/') + 1)<<GREEN<<"\" criado com sucesso no HD virtual como \""<<YELLOW<<params[1]<<GREEN"\".\n\n";
}
#endif