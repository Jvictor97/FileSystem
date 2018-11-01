#ifndef HELP_CPP
#define HELP_CPP
#include <iostream>
#include <cstdlib>
#include <string>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

using namespace std;

void help(){
    printf(YELLOW "%-60s %-30s\n" RESET, "Comandos", "Funcionalidades");

    printf(RESET "%-60s %-30s\n" RESET, "create [arquivo]", "Cria struct do I-Node de arquivo.");
    printf(RESET "%-60s %-30s\n" RESET, "copy [nome] [novo nome]", "Copia um arquivo do diretorio atual cujo nome seja [nome] para o path especificado por [novo nome]");
    printf(RESET "%-60s %-30s\n" RESET, "copyfrom [nome] [novo nome]", "Copia um arquivo do HD virtual para o HD real");
    printf(RESET "%-60s %-30s\n" RESET, "copyto [nome] [novo nome]", "Copia um arquivo do HD real para o HD virtual");
    printf(RESET "%-60s %-30s\n" RESET, "move [origem] [destino]", "Move o arquivo especificado pelo path [origem] para o path [destino]");
    printf(RESET "%-60s %-30s\n" RESET, "rename [nome] [novo nome]", "Renomeia o arquivo especificado pelo nome [nome] com o conteudo de [novo nome]");
    printf(RESET "%-60s %-30s\n" RESET, "remove [arquivo]", "Deleta um arquivo do HD selecionado");
    printf(RESET "%-60s %-30s\n" RESET, "type [arquivo]", "Exibe o conteudo do arquivo na linha de comando (semelhante ao comando 'cat' do linux)");
    printf(RESET "%-60s %-30s\n" RESET, "[nome do HD]", "Seleciona um HD e copia todo o seu conteudo para a memoria permitindo manipulacao");
    printf(RESET "%-60s %-30s\n" RESET, "createhd [nome do hd] [tamanho do bloco] [qtde de blocos]", "Cria um novo HD ja formatado para uso");
    printf(RESET "%-60s %-30s\n" RESET, "dirhd", "Exibe a lista de todos os HDs ja criados");
    printf(RESET "%-60s %-30s\n" RESET, "formathd [nome do hd]", "Limpa todo o conteudo do HD e reinicializa o superblock, o header de bitmap, seus inodes e datablocks");
    printf(RESET "%-60s %-30s\n" RESET, "removehd [nome do hd]", "Exclui um HD do sistema");
    printf(RESET "%-60s %-30s\n" RESET, "statushd [nome do hd]", "Exibe propriedades do filesystem como espaco utilizado e disponivel");
    printf(RESET "%-60s %-30s\n" RESET, "typehd [nome do hd]", "Coleta informacoes dos blocos de dados validos no HD e exibe na tela o conteudo em codificacao char e byte");
    printf(RESET "%-60s %-30s\n" RESET, "cd [nome da pasta]", "Seleciona um diretorio especifico e carrega na memoria todos os metadados do mesmo");
    printf(RESET "%-60s %-30s\n" RESET, "copydir [origem] [destino]", "Copia um diretorio e todos os seus sub-itens recursivamente de [origem] para [destino]");
    printf(RESET "%-60s %-30s\n" RESET, "createdir [nome da pasta]", "Cria um novo diretorio no sistema de arquivos");
    printf(RESET "%-60s %-30s\n" RESET, "dir [nome da pasta]", "Exibe todo o conteudo (arquivos e subdirs) de um diretorio bem como seus metadados");
    printf(RESET "%-60s %-30s\n" RESET, "movedir [origem] [destino]", "Este comando nao permite mover uma pasta para o mesmo diretorio");
    printf(RESET "%-60s %-30s\n" RESET, "removedir [nome da pasta]", "Cria um novo diretorio no sistema de arquivos");
    printf(RESET "%-60s %-30s\n" RESET, "renamedir [nome] [novo nome]", "Renomeia o diretorio especificado por [nome] utilizando o conteudo de [novo nome]");
    printf(RESET "%-60s %-30s\n" RESET, "help ou ?", "Mostra os comandos do sistema e suas funcionalidades");
    printf(RESET "%-60s %-30s\n" RESET, "exit", "sai do sistema");
		  
}

#endif