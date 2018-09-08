#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

void printHelp(){
  cout<<" Comandos e Funcionalidades\n\n\n"<<endl;
			cout<<"ARQUIVO - create [arquivo] \nCriar struct do I-Node de arquivo.\n"<<endl;

  		cout<<"ARQUIVO - copy [nome] [novo nome]\nCopia um arquivo do diret�rio atual cujo nome seja [nome] para o path especificado por [novo nome]\n"<<endl;

      cout<<"ARQUIVO - copyfrom [nome] [novo nome]\nCopia um arquivo do HD virtual para o HD real\n"<<endl;

      cout<<"ARQUIVO - copyto [nome] [novo nome]\nCopia um arquivo do HD real para o HD virtual\n"<<endl;

      cout<<"ARQUIVO - move [origem] [destino]\nMove o arquivo especificado pelo path [origem] para o path [destino]\n"<<endl;

      cout<<"ARQUIVO - rename [nome] [novo nome \nRenomeia o arquivo especificado pelo nome [nome] com o conte�do de [novo nome]\n"<<endl;

      cout<<"ARQUIVO - remove [arquivo]\nDeleta um arquivo do HD selecionado\n"<<endl;
       
      cout<<"ARQUIVO - type [arquivo]\nExibe o conte�do do arquivo na linha de comando (semelhante ao comando 'cat' do linux)\n"<<endl;

      cout<<"HD - [nome do HD]:\nSeleciona um HD e copia todo o seu conte�do para a mem�ria permitindo manipula��o\n"<<endl;

      cout<<"HD - createhd [nome do hd] [tamanho do bloco] [qtde de blocos]\nCria um novo HD j� formatado para uso\n"<<endl;

      cout<<"HD - dirhd\nExibe a lista de todos os HDs j� criados\n"<<endl;

      cout<<"HD - formathd [nome do hd]\nLimpa todo o conte�do do HD e reinicializa o superblock, o header de bitmap, seus inodes e datablocks\n"<<endl;

      cout<<"HD - removehd [nome do hd]\nExclui um HD do sistema\n"<<endl;

      cout<<"HD - statushd [nome do hd]\nExibe propriedades do filesystem como espa�o utilizado e dispon�vel\n"<<endl;
      
      cout<<"HD - typehd [nome do hd]\nColeta informa��es dos blocos de dados v�lidos no HD e exibe na tela o conte�do em codifica��o char e byte\n"<<endl;

      cout<<"PASTAS - cd [nome da pasta]\nSeleciona um diret�rio espec�fico e carrega na mem�ria todos os metadados do mesmo\n"<<endl;

      cout<<"PASTAS - copydir [origem] [destino]\nCopia um diret�rio e todos os seus sub-itens recursivamente de [origem] para [destino]\n"<<endl;

      cout<<"PASTAS - createdir [nome da pasta]\nCria um novo diret�rio no sistema de arquivos\n"<<endl;

      cout<<"PASTAS - dir [nome da pasta]\nExibe todo o conte�do (arquivos e subdirs) de um diret�rio bem como seus metadados\n"<<endl;

      cout<<"PASTAS - movedir [origem] [destino]\nEste comando n�o permite mover uma pasta para o mesmo diret�rio\n"<<endl;

      cout<<"PASTAS - removedir [nome da pasta]\nCria um novo diret�rio no sistema de arquivos\n"<<endl;

      cout<<"PASTAS - renamedir [nome] [novo nome]\nRenomeia o diret�rio especificado por [nome] utilizando o conte�do de [novo nome]\n"<<endl;
      
	  cout<<"help ou ?\n Ajuda\n"<<endl;
	  
	  cout<<"exit\nSair\n\n\n\n"<<endl;
}



int main() {
  printHelp();
}
