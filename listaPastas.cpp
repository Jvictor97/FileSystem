#include <dirent.h>
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

void listFolders(){
	DIR* diretorio;	
	struct dirent* item_diretorio;
	string path;
	string command;
	cin>>command;
	
	//Se o comando desejado for dir
	if(command == "dir"){
		cin>>path; //entra com o diretorio que deseja-se listar os arquivos
		diretorio = opendir(path.c_str()); //abre o diretorio especificado
		if( !diretorio ){ // caso o diretorio especificado nao exista, exibe o erro
			perror( "Erro" );	
		}	
		item_diretorio = readdir(diretorio); //le o diretorio especificado	
		while(item_diretorio){ 
			cout<<item_diretorio->d_name <<endl; //imprime os arquivos dentro do diretorio especificado
			item_diretorio = readdir(diretorio);
		}	
		closedir(diretorio); //fecha o diretorio
	}	
}



int main(){	
	listFolders();
	return( 0 );
	}

