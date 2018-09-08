#include <iostream>
#include <string>
#include <stdio.h>
#include <map>
#include <sstream>
#include <vector>
#include <bitset>
#include "hd.cpp"

using namespace std;

void getclause();

int main() {
	map<std::string, functions> funcs;
	funcs["createhd"] = createhd;
	funcs["exit"] = exit;

	cout<<"\n* Seja bem vindo ao MVPFS *";
	cout<<"\nDigite a funcao que deseja executar:\n\n";

	while(cmd != "exit"){
		printf(">");
		getclause();

		if(cmd.back() == ':'){
			cmd.back() = '.';
			cmd += "mvpfs";
			//nomeHD = cmd;
			//selecionaHD();
		}
		else
			if(funcs.find(cmd.c_str()) == funcs.end())
				cout<<"ERRO: Esta funcao nao existe...\n";
			else
				funcs[cmd]();
	}


	return 0;
}

void getclause(){
	string in;
	int i = 0;
	std::string line;
	std::getline(cin, line);
	std::istringstream iss(line);

	bool first = true;

	while (iss >> in){
		if(first){
			cmd = in;
			first = false;
		}else{
			params[i] = in;
			i++;
		}
	}
}