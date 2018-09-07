#include <sys/stat.h>
#include <string.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>



#ifdef __linux__
using namespace std;
void createFolderLinux(){
	string createdir = "createdir";
	string folder_name;
	if(cin>>createdir>>folder_name){
		const int dir_err = mkdir(folder_name.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (dir_err == -1) {
        perror("Erro");
        exit(1);
    }	
	}
}

int main(int argc, char *argv[]){
	createFolderLinux();	
	return 0;
}

#elif defined WIN32
#include <direct.h> 
using namespace std;
void createFolderWindows(){
	string folder_name;
	string createdir = "createdir";
	if(cin>>createdir>>folder_name){
		if(mkdir(folder_name.c_str()) != 0){
			perror("Erro");
		}
		else{
			puts("Diretorio Criado com Sucesso.");
		}
	}
}

int main() {
   createFolderWindows();
   return 0;
}

#elif defined WIN64
using namespace std;
void createFolderWindows(){
	string folder_name;
	string createdir = "createdir";
	string str;
	if(cin>>createdir>>folder_name){
		if(mkdir(folder_name.c_str()) != 0){
			perror("Erro");
		}
		else{
			puts("Diretorio Criado com Sucesso.");
		}
	}
}

int main() {
   createFolderWindows();
   return 0;
}

#else
#error Plataforma não suportada
#endif
