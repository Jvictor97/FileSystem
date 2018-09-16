#include <iostream>
#include <bitset>
#include <cstdio>
#include <string>
#include <ostream>

using namespace std;

void writeHeader(FILE * hd, int sizeBloco, int qtdeBlocos){
	fputc('$', hd);
	for(int bloco = 0; bloco < qtdeBlocos; bloco++){
		for(int size = 0; size < sizeBloco; size++){
			fputc('0', hd);
		}
		//fputc('\n', hd);		
	}
	fputc('*', hd);
}	

FILE * createhd(int qtdeBlocos, int sizeBloco){
	FILE * hd;
	hd = fopen("myHD.txt", "w+");
	if(hd == NULL){
		cout<<"Unable to create Hard Drive File"<<endl;
		return NULL;
	}
	writeHeader(hd, sizeBloco, (qtdeBlocos/(sizeBloco * 8)));
	for(int blocos = 0; blocos < qtdeBlocos; blocos++){
		for(int size = 0; size < sizeBloco; size++){
			fputc('#', hd);
		}
		//fputc('\n', hd);
	}
	return hd;
}

void writeToBlock(int blockNo, string content, FILE * hd, int sizeBloco){
	if(content.length() < sizeBloco){
		fseek(hd, blockNo * sizeBloco, SEEK_SET);
		fputs(content.c_str(), hd);
	}else{
		cout<<"Too big";
	}
}

int main() {
  	//bitset<8> mb = bitset<8>(16);
	
	FILE * hd = createhd(1024, 32);

	writeToBlock(30, "DeuCerto", hd, 32);
	fclose(hd);
	return 0;
}