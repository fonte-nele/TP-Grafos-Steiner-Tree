#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdlib>
using namespace std;

int main(int argc, char *argv[])
{
	string nome_arq;
    if(argc != 2)
    {
        //Analisando erro do formato de entrada do programa!
        cerr << "\nFormato de entrada via linha de comando invalido!" << endl;
        cerr << "Formato esperado: <executavel> <nome_arquivo_instancia>" << endl << endl;
        
        cout << "Por favor, informe o nome do arquivo da instancia a ser tratada: " << endl;
        cout << "Nome: ";
        getline(cin,nome_arq);
    }
    else
    {   //Atribuindo o nome do arquivo digitado na linha de comando para a string!
        nome_arq = argv[1];
    }

    ifstream entrada;
    string linha;
    int v1, v2, peso, tam, cont;
    //Colocando o formato do arquivo de entrada!
    if(nome_arq.find(".stp") > nome_arq.size())
    {
        //Caso não encontre o .stp entrará aqui!	
    	nome_arq += ".stp";
    }
    
    entrada.open(nome_arq.c_str());
    while(!entrada)
    {
        //Analisando erro do nome do arquivo a ser aberto!
        cerr << "\nERRO NA ABERTURA DO ARQUIVO " << nome_arq << " !!!" << endl << endl;
        cerr << "Favor informar um nome valido para o arquivo da instancia" << endl;
        cerr << "OBS.: O NOME DO ARQUIVO DEVE SER DIGITADO SEM SEU RESPECTIVO FORMATO" << endl;
        cerr << "Nome : ";
        getline(cin,nome_arq);
        if(nome_arq.find(".stp") > nome_arq.size())
	    {
	        //Caso não encontre o .stp entrará aqui!	
	    	nome_arq += ".stp";
	    }
        entrada.open(nome_arq.c_str());
    }
    //Leitura da parte inicial do arquivo, com a quantidade de vértices e arestas!
    while(entrada >> linha)
    {
    	if(linha == "Nodes")
    	{
    		entrada >> v1;
    		entrada >> linha >> v2;
    		break;
    	}
    }

    //getline(entrada,linha);
    //entrada >> linha >> v1;
    //entrada >> linha >> v2;
    
    cout << "V1 : " << v1 << " Arestas: " << v2 << " Linha: " << linha;
    cont = v2;
    //Construção do grafo!
    //Grafo grafo(v1,v2,&cont,&tam);
    int *vetTerminais, i=0;
    while(cont != 0)
    {
        entrada >> linha >> v1 >> v2 >> peso;
        //Construção Lista Adjacência!
        //cout << "Vertices: " << v1 << endl;
        //cout << "Arestas: " << v2 << endl;
        //grafo.addAresta(v1,v2,peso);
        cont--;
    }

    while(entrada >> linha)
    {
    	if(linha == "Terminals")
    	{
    		entrada >> linha >> cont;
    		cout << "TErmias: " << cont;
    	}
    }
    //getline(entrada,linha);
    //getline(entrada,linha);
    //getline(entrada,linha);
    //getline(entrada,linha);
    //entrada >> linha >> cont;
    vetTerminais = new int[cont];

    while(cont != 0)
    {
        //Construção do vetor de vértices terminais!
        entrada >> linha >> vetTerminais[i];
        i++;
        cont--;
    }
    entrada.close();
	return 0;
}