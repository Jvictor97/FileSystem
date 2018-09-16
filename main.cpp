#include <iostream>
#include <string>
#include <stdio.h>
#include <map>
#include <sstream>
#include <vector>
#include <bitset>
#include "hd.cpp"
#include "help.cpp"
#include "dir.cpp"

using namespace std;

void getclause();

int main() {
	map<std::string, functions> funcs;
	funcs["createhd"] = createhd;
	funcs["formathd"] = formathd;
	funcs["dirhd"] = dirhd;
	funcs["typehd"] = typehd;
	funcs["exit"] = exit;
	funcs["help"] = help;
	funcs["?"] = help;

	cout<<"\n* Seja bem vindo ao MVPFS *";
	cout<<"\nDigite a funcao que deseja executar:\n\n";

	while(cmd != "exit"){
		printf("# ");
		getclause();

		if(cmd.back() == ':'){
			cmd.back() = '.';
			cmd += "mvpfs";
			nomeHD = cmd;
			selecionaHD();
		}
		else
			if(cmd == "")
				printf("");
			else if(funcs.find(cmd.c_str()) == funcs.end())
				cout<<RED<<"ERRO: Esta funcao nao existe...\n"<<RESET;
			else{
				funcs[cmd]();
				cmd = "";
			}
	}


	return 0;
}