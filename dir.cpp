#include <sys/stat.h>
#include <string.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;


void createdir(){
	string folder_name;
	cin>>folder_name;
	#ifdef __linux__
	const int dir_err = mkdir(folder_name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (dir_err == -1) {
		perror("Erro");
		exit(1);
	}

	
	#elif defined WIN32
	if(mkdir(folder_name.c_str()) != 0){
			perror("Erro");
		}
		else{
			puts("Diretorio Criado com Sucesso.");
		}
	
	#elif defined WIN64 
	if(mkdir(folder_name.c_str()) != 0){
			perror("Erro");
		}
		else{
			puts("Diretorio Criado com Sucesso.");
		}

	#else
	#error Plataforma não suportada
	#endif

}

int main(){
	createdir();
	return 0;
}
