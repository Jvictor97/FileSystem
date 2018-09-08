#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

void printHelp(){
  cout<<" Comandos e Funcionalidades\n\n\n"<<endl;
			cout<<"ARQUIVO - create [arquivo] \nCriar struct do I-Node de arquivo.\n"<<endl;

  		cout<<"ARQUIVO - copy [nome] [novo nome]\nCopia um arquivo do diretório atual cujo nome seja [nome] para o path especificado por [novo nome]\n"<<endl;

      cout<<"ARQUIVO - copyfrom [nome] [novo nome]\nCopia um arquivo do HD virtual para o HD real\n"<<endl;

      cout<<"ARQUIVO - copyto [nome] [novo nome]\nCopia um arquivo do HD real para o HD virtual\n"<<endl;

      cout<<"ARQUIVO - move [origem] [destino]\nMove o arquivo especificado pelo path [origem] para o path [destino]\n"<<endl;

      cout<<"ARQUIVO - rename [nome] [novo nome \nRenomeia o arquivo especificado pelo nome [nome] com o conteúdo de [novo nome]\n"<<endl;

      cout<<"ARQUIVO - remove [arquivo]\nDeleta um arquivo do HD selecionado\n"<<endl;
       
      cout<<"ARQUIVO - type [arquivo]\nExibe o conteúdo do arquivo na linha de comando (semelhante ao comando 'cat' do linux)\n"<<endl;

      cout<<"HD - [nome do HD]:\nSeleciona um HD e copia todo o seu conteúdo para a memória permitindo manipulação\n"<<endl;

      cout<<"HD - createhd [nome do hd] [tamanho do bloco] [qtde de blocos]\nCria um novo HD já formatado para uso\n"<<endl;

      cout<<"HD - dirhd\nExibe a lista de todos os HDs já criados\n"<<endl;

      cout<<"HD - formathd [nome do hd]\nLimpa todo o conteúdo do HD e reinicializa o superblock, o header de bitmap, seus inodes e datablocks\n"<<endl;

      cout<<"HD - removehd [nome do hd]\nExclui um HD do sistema\n"<<endl;

      cout<<"HD - statushd [nome do hd]\nExibe propriedades do filesystem como espaço utilizado e disponível\n"<<endl;
      
      cout<<"HD - typehd [nome do hd]\nColeta informações dos blocos de dados válidos no HD e exibe na tela o conteúdo em codificação char e byte\n"<<endl;

      cout<<"PASTAS - cd [nome da pasta]\nSeleciona um diretório específico e carrega na memória todos os metadados do mesmo\n"<<endl;

      cout<<"PASTAS - copydir [origem] [destino]\nCopia um diretório e todos os seus sub-itens recursivamente de [origem] para [destino]\n"<<endl;

      cout<<"PASTAS - createdir [nome da pasta]\nCria um novo diretório no sistema de arquivos\n"<<endl;

      cout<<"PASTAS - dir [nome da pasta]\nExibe todo o conteúdo (arquivos e subdirs) de um diretório bem como seus metadados\n"<<endl;

      cout<<"PASTAS - movedir [origem] [destino]\nEste comando não permite mover uma pasta para o mesmo diretório\n"<<endl;

      cout<<"PASTAS - removedir [nome da pasta]\nCria um novo diretório no sistema de arquivos\n"<<endl;

      cout<<"PASTAS - renamedir [nome] [novo nome]\nRenomeia o diretório especificado por [nome] utilizando o conteúdo de [novo nome]\n"<<endl;
      
	  cout<<"help ou ?\n Ajuda\n"<<endl;
	  
	  cout<<"exit\nSair\n\n\n\n"<<endl;
}



int main() {
  printHelp();
}
