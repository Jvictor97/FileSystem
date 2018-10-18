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
	// Entrega 1:
	funcs["createhd"] = createhd;
	funcs["formathd"] = formathd;
	funcs["dirhd"] = dirhd;
	funcs["typehd"] = typehd;
	funcs["exit"] = exit;
	funcs["help"] = help;
	funcs["?"] = help;

	// Entrega 2:
	funcs["removehd"] = removehd;
	funcs["statushd"] = statushd;

	system("clear");

	cout<<"\n* Seja bem vindo ao MVPFS *";
	cout<<"\nDigite a funcao que deseja executar:\n\n";

	while(cmd != "exit"){
		printf("# ");
		getclause();

		if(cmd.back() == ':'){
			cmd.back() = '.';
			cmd += "mvpfs";
			nomeHD = cmd;
			cmd = "";
			selecionaHD();
		}
		else
			if(cmd == "")
				printf("");
			else if(funcs.find(cmd.c_str()) == funcs.end())
				cout<<RED<<"\nERRO: Esta funcao nao existe...\n\n"<<RESET;
			else{
				funcs[cmd]();
				cmd = "";
			}
	}


	return 0;
}