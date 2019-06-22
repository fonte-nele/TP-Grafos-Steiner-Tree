/*
    Feito por:                                  Matrícula:
        Felipe Fontenele de Ávila Magalhães         15.1.4331
        Mateus Vitor Pereira Lana                   15.1.4340
    Disciplina: BCC 204 - Teoria dos Grafos
    Prof.: Marco Antonio M. Carvalho
*/
#include <iostream>
#include <string>
#include <vector>
#include <utility>  //Para usar pair, make_pair
#include <fstream>
#include <chrono>
 
using namespace std;
using namespace std::chrono;

typedef struct aresta   //Para usar na Heap Binária!
{
    int v1;
    int v2;
    int peso;
}Aresta;

class Grafo
{
    public:
        Grafo(int vertices, int arestas) //Construtor
        {
            this->vertices = vertices;
            this->arestas = arestas;

            listaAdj = new vector< pair< int,int> > [vertices+1];
            arvPrim = new vector< pair< int,int> > [vertices+1];
        }
        ~Grafo()    //Destrutor
        {
            delete [] listaAdj;
            delete [] arvPrim;
        }
        void addAresta(int, int, int);
        void arvorePrim();
        void arvoreBFS(int*, int*, int*);
        void arvoreSteiner(int*, int*, int*, int, int*, bool*);
        void imprimirGrafo();
        void imprimirArvorePrim();
        void imprimirArvSteiner(bool*, int*);
    private:
        int vertices;   //Quantidade de vertices do grafo!
        int arestas;    //Quantidade de arestas do grafo!
        
        //Outra forma: vector< list< pair<int, int> > > listaAdj(vertices + 1);
        vector< pair<int,int> > *listaAdj;  //Vetor de vector de pares, <vert. destino, peso>
        vector< pair<int,int> > *arvPrim;   //Idem
};

int menu();
void addHeap(Aresta*, int, Aresta);
void reconstruirHeap(Aresta*, int, int);
void excluirV_Heap(Aresta*, int, int);
Aresta menorHeap(Aresta*, int);

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

    //Construção do grafo!
    Grafo grafo(v1,v2);
    tam = v1;	//quantidade de vertices!
    cont = v2;	//quantidade de arestas!
    int *vetTerminais, i=0;
    while(cont != 0)
    {
        entrada >> linha >> v1 >> v2 >> peso;
        //Construção Lista Adjacência!
        grafo.addAresta(v1,v2,peso);
        cont--;
    }
    while(entrada >> linha)
    {
    	if(linha == "Terminals")
    	{
    		entrada >> linha >> cont;
    		break;
    	}
    }

    vetTerminais = new int[cont];
    while(cont != 0)
    {
        //Construção do vetor de vértices terminais!
        entrada >> linha >> vetTerminais[i];
        i++;
        cont--;
    }
    entrada.close();
    //grafo.imprimirGrafo();

    duration<double> time_span;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    //Geração Árvore Geradora Mínima com base no algoritmo de Prim!
    grafo.arvorePrim();
    //grafo.imprimirArvorePrim();

    //Geração Arvore de Profundidade - BFS
    int pai[tam + 1], nivel[tam + 1], vPeso[tam + 1];
    bool marcado[tam + 1];
    grafo.arvoreBFS(pai, nivel, vPeso);

    //Remoção dos vértices folhas do grafo a partir da BFS!
    grafo.arvoreSteiner(vetTerminais, pai, nivel, i, vPeso, marcado);

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    time_span = duration_cast<duration<double> >(t2-t1);


    grafo.imprimirArvSteiner(marcado, vPeso);
    cout << "Tempo de execucao: " << time_span.count() << " segundos." << endl;
    //Menu final, caso o usuario queira visualizar mais informacoes sobre o grafo!
    int op;
    do{
    	cout << endl;
        op = menu();
        switch(op)
        {
            case 1:
                grafo.imprimirGrafo();
                break;
            case 2:
                grafo.imprimirArvorePrim();
                break;
            case 3:
                cout << "\n--Arvore de Profundidade BFS--" << endl;
                for(int j = 1; j <= tam; j++) 
                {
                    cout << "Vertice: " << j << " Nivel -> " << nivel[j] << "\t Pai -> " << pai[j] << "\t Peso aresta -> " << vPeso[j] << endl;
                }
                break;
            case 4:
                grafo.imprimirArvSteiner(marcado,vPeso);
                break;
            case 5:
                cout << "\nPROGRAMA FINALIZADO COM SUCESSO..." << endl << endl;
                break;
            default:
                cout << "\nOPCAO INVALIDA. FAVOR TENTAR NOVAMENTE..." << endl << endl;
                break;
        }
    }while(op != 5);

    delete [] vetTerminais;     
    return 0;
}

void Grafo::addAresta(int v1, int v2, int peso)
{
    //Adiciona vértice v2 à lista de vértices adjacentes de v1
    listaAdj[v1].push_back(make_pair(v2, peso));
    //Adiciona vértice v1 à lista de vértices adjacentes de v2
    listaAdj[v2].push_back(make_pair(v1, peso));
}

void Grafo::imprimirGrafo()
{
    cout << "\t-------GRAFO-------" << endl;
    //Impressao da lista de adjacencia do grafo! 
    for(int i = 1; i <= vertices; i++)
    {
        cout << "\nVertice " << i << " tem uma aresta com o vertice: " << endl;

        vector< pair<int,int > >::iterator it = listaAdj[i].begin();
        while(it != listaAdj[i].end())
        {
            cout << "\t" << (*it).first << " com peso = " << (*it).second << endl;
            it++;
        }
        cout << endl;
    }
}

void Grafo::imprimirArvorePrim()
{
    cout << "\nArvore Geradora Minima - Prim" << endl;
    //Impressao da lista de adjacencia da Arvore Geradora Minima!
    for(int i = 1; i <= vertices; i++) 
    {
        cout << "\tAGM["<< i << "]";
          
        vector< pair<int, int> >::iterator it = arvPrim[i].begin();
        while(it != arvPrim[i].end())
        {
            cout << " -> " << (*it).first << "[" << (*it).second << "]";
            it++;
        }
        cout << endl;
    }
    cout << endl << "\t\tLEGENDA:\n\t\tLista Adjacencia Vertice -> Vertice Adjacente[Peso da aresta entre os vertices]" << endl << endl;
}

void Grafo::arvorePrim()
{
    int atual = 1, tamFila = 0, proxVert, i;
    bool marcados[vertices + 1];
    Aresta aux, filaPrioridade[arestas];
   
    for(i = 0; i <= vertices; i++) //Inicializando todos os vertices como nao-visitados! 
    {
        marcados[i] = false;
    }
    i = 0;
    while(i < vertices)
    {
        if(!marcados[atual])        //Vértice atual não marcado!
        {            
            marcados[atual] = true; //Marca o vértice atual!
              
            //Olha a lista de adjacencia do vertice atual!
            vector< pair<int, int> >::iterator it = listaAdj[atual].begin();

            while(it != listaAdj[atual].end())
            {                
                if(!marcados[(*it).first]) //Vertice destino não marcado!
                {
                    //Adicionar o vertice ao Heap
                    aux.v1 = atual;
                    aux.v2 = (*it).first;
                    aux.peso = (*it).second;
                  
                    addHeap(filaPrioridade, tamFila, aux);
                    tamFila++;
                }
                it++;
            }
            //Escolha Gulosa(menor)!
            aux = menorHeap(filaPrioridade, tamFila);
            tamFila--;
   
            proxVert = aux.v2;
            atual = aux.v1;
   
            if(!marcados[proxVert])
            {
                arvPrim[atual].push_back(make_pair(proxVert, aux.peso));
                arvPrim[proxVert].push_back(make_pair(atual, aux.peso));
            }
   
            atual = proxVert;
            i++;
        }
        else
        {
            aux = menorHeap(filaPrioridade, tamFila);
            tamFila--;
   
            proxVert = aux.v2;
            atual = aux.v1;
   
            if(!marcados[proxVert])
            {
                arvPrim[atual].push_back(make_pair(proxVert, aux.peso));
                arvPrim[proxVert].push_back(make_pair(atual, aux.peso));
            }
            atual = proxVert;
        }
    }
}

void addHeap(Aresta *heap, int tam, Aresta aux)
{     
    int i = tam;
    Aresta temp;
    heap[tam] = aux;
      
    while(i >= 1)
    {
        if(heap[i / 2].peso > heap[i].peso)
        {
            //Vértice pai é maior que filho, Viola HEAP mínimo, Troca até ser válido.
            temp = heap[i / 2];
            heap[i / 2] = heap[i];
            heap[i] = temp;
              
            i = i / 2;
        }
        else
        {
            //Vértice pai é menor ou igual ao filho, Satisfaz a HEAP mínimo, Inserção OK 
            break;
        }
    }
}
  
void reconstruirHeap(Aresta *heap, int tam, int pos)
{
    int i = pos;
    Aresta temp;

    while((2 * i) < tam)        //Filho(vertice) esquerdo ainda existe!
    {
        if((2 * i) + 1 >= tam)  //Filho direito não existe, mas o direito existe!
        {
            if(heap[i].peso > heap[2 * i].peso) //Filho esquerdo menor que o pai
            {
                temp = heap[i];
                heap[i] = heap[2 * i];
                heap[2 * i] = temp;
                break;  //Quando chega nesse nível, filho à direita não existe, então o HEAP pode terminar aqui!
            }
        }
          
        //Ambos filhos existem!
        if(heap[i].peso > heap[2 * i].peso || heap[i].peso > heap[2 * i + 1].peso)
        {
            //Um dos filhos é menor que o pai
              
            if(heap[2 * i].peso <= heap[(2 * i) + 1].peso)  //Filho esquerdo menor que o direito, TROCA!
            {
                temp = heap[2 * i];
                heap[2 * i] = heap[i];
                heap[i] = temp;
                //E segue o heap!
                i = 2 * i;
            }
            else
            {
                if(heap[2 * i].peso > heap[(2 * i) + 1].peso)  //Filho esquerdo maior que o direito, TROCA!
                {
                    temp = heap[(2 * i) + 1];
                    heap[(2 * i) + 1] = heap[i];
                    heap[i] = temp;
                    //E segue o heap!
                    i = (2 * i) + 1;
                }
            }
        }
        else
        {
            //Pai é menor que os filhos!
            break;
        }
    }
}
  
void excluirV_Heap(Aresta *heap, int tam, int pos)
{
    //Troca o elemento a ser excluído com o último da Heap!
    Aresta temp = heap[pos];
    heap[pos] = heap[tam - 1];
    heap[tam - 1] = temp;
      
    //Diminue o tamanho da Heap, fazendo uma exclusão lógica!
    tam--;
    //Propriedade da Heap Mínima ferida!
    reconstruirHeap(heap, tam, pos);
}
  
Aresta menorHeap(Aresta *heap, int tam)
{
    Aresta menor = heap[0];
   
    excluirV_Heap(heap, tam, 0);
    //Retorna o elemento de menor prioridade e exclui do heap!
    return menor;
}

void Grafo::arvoreBFS(int *pai, int *nivel, int *peso)
{
    int i, par, niv = 0;
    bool flag = true;   //Indica se o grafo ainda pode ser explorado!

    for(i = 1; i <= vertices; i++)  //Inicializacao dos campos!
    {
        peso[i] = 0;
        pai[i] = 0;
        nivel[i] = -1;
    }
    nivel[1] = niv; //Vertice 1 com nivel 0(Topo da arvore)!

    while(flag)
    {
        flag = false;
        for(i = 1; i <= vertices; i++)
        {
            if(nivel[i] == niv)
            {
                flag = true;
                vector< pair<int, int> >::iterator it = arvPrim[i].begin();
                par = i;

                while(it != arvPrim[i].end())
                {
                    if(nivel[(*it).first] != -1)
                    {
                        it++;
                        continue;
                    }
                    nivel[(*it).first] = niv + 1;
                    pai[(*it).first] = par;
                    peso[(*it).first] = (*it).second;
                    
                    it++;
                }
            }
        }
        niv++;
    }
}

void Grafo::arvoreSteiner(int *vetTerminais, int *pai, int *nivel, int tam, int *peso, bool *marcados)
{
    int cont = 0, i, v, aux;

    for(i = 1; i <= vertices; i++) //Inicializando todos os vertices como nao-visitados! 
    {
        marcados[i] = false;
    }
    //Marca o primeiro vertice terminal!
    v = vetTerminais[cont];
    marcados[v] = true;
    cont++;

    while(cont < tam)   //Enquanto nao terminar o vetor de vertices terminais!
    {
        v = vetTerminais[cont]; //Proximo vertice terminal
        if(!marcados[v])        //Verifica se nao esta marcado
        {
            marcados[v] = true; //Marca o mesmo e analisa o pai, voltando na arvore!
            aux = pai[v];
            while(!marcados[aux])
            {
                marcados[aux] = true; 
                aux = pai[aux];                   
            }
        }
        cont++;
    }
}

void Grafo::imprimirArvSteiner(bool *marcados, int *peso)
{
	int i, soma = 0;
    cout << "\nAproximacao Arvore Steiner" << endl;
    //Impressao dos vertices da aproximação da Arvore de Steiner!
    for(i=1; i <= vertices; i++)
    {
        if(marcados[i])
        {
            soma += peso[i];
            cout << "\t" << i ;
        }
    }
    cout << endl << endl << "Soma das arestas = " << soma << endl;
}

int menu()
{
    int escolha;
    cout<<"  ============================MENU============================"<<endl;
    cout<<" ||                                                          ||"<<endl;
    cout<<" ||                                                          ||"<<endl;
    cout<<" || 1 - Imprimir Grafo de Entrada.                           ||"<<endl;
    cout<<" || 2 - Imprimir Arvore Geradora Minima - PRIM.              ||"<<endl;
    cout<<" || 3 - Imprimir Arvore Profundidade - BFS.                  ||"<<endl;
    cout<<" || 4 - Imprimir Arvore Steiner.                             ||"<<endl;
    cout<<" || 5 - Sair.                                                ||"<<endl;
    cout<<" ||                                                          ||"<<endl;
    cout<<"  ============================MENU============================"<<endl;
    cout<<endl;
    cout<<"  Escolha: ";  
    cin>>escolha;
    cin.ignore();
    return escolha;
}