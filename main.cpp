#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include "Grafo/Grafo.h"
#include "Grafo/No.h"
#include "Grafo/Aresta.h"
#include "Algoritmos/Dijkstra.h"
#include "Algoritmos/FechoDireto.h"
#include "Algoritmos/FechoIndireto.h"
#include "Algoritmos/Kruskal.h"
#include "Algoritmos/OrdenacaoTopologica.h"
#include "Algoritmos/PinkFloyd.h"
#include "Algoritmos/Prim.h"
#include "Algoritmos/gulosoRanzomizado.h"
#include <string>

using namespace std;

//função auxiliar para calcular peso das arestas 
int calculaPeso(float x1, float y1, float x2, float y2)
{
    float valF = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );
    int valL = (int) valF;
    float valR = valF - valL;   

    if(valR >= 0.5)
        return valL + 1;
    return valL;
}


Grafo *leituraMST(ifstream &input_file)
{
    //Variáveis para auxiliar na criação dos nós no Grafo
    int ordem;
    int id;
    float x, y;

    //Pegando a ordem do grafo
    input_file >> ordem;    

    //Criando objeto grafo
    Grafo *graph = new Grafo(ordem, 0, 1, 0);

    while (input_file >> id >> x >> y)
    {
        graph->getNo(id)->setCoordenadas(x,y);
    }

    No *no1;
    No *no2;
    //adicionando aresta de um no para os demais
    for(int i=1; i<=ordem; i++){
        for(int j= i+1; j<=ordem; j++){
            no1 = graph->getNo(i);
            no2 = graph->getNo(j);
            graph->inserirAresta(i,j,calculaPeso(no1->getX(), no1->getY(), no2->getX(), no2->getY()));
        }
    }

    graph->criaListaAdjacencia();

    return graph;
}

Grafo *leitura(ifstream &input_file, int directed, int weightedEdge, int weightedNode)
{

    //Variáveis para auxiliar na criação dos nós no Grafo
    int idNodeSource;
    int idNodeTarget;
    int order;

    //Pegando a ordem do grafo
    input_file >> order;

    //Criando objeto grafo
    Grafo *graph = new Grafo(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo

    if (!graph->getPonderadoAresta() && !graph->getPonderadoNo())
    {

        while (input_file >> idNodeSource >> idNodeTarget)
        {

            graph->inserirAresta(idNodeSource, idNodeTarget, 0);
        }
    }
    else if (graph->getPonderadoAresta() && !graph->getPonderadoNo())
    {

        float edgeWeight;

        while (input_file >> idNodeSource >> idNodeTarget >> edgeWeight)
        {

            graph->inserirAresta(idNodeSource, idNodeTarget, edgeWeight);
        }
    }
    else if (graph->getPonderadoNo() && !graph->getPonderadoAresta())
    {

        float nodeSourceWeight, nodeTargetWeight;

        while (input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight)
        {

            graph->inserirAresta(idNodeSource, idNodeTarget, 0);
            graph->getNo(idNodeSource)->setPeso(nodeSourceWeight);
            graph->getNo(idNodeTarget)->setPeso(nodeTargetWeight);
        }
    }
    else if (graph->getPonderadoNo() && graph->getPonderadoAresta())
    {

        float nodeSourceWeight, nodeTargetWeight, edgeWeight;

        while (input_file >> idNodeSource >> nodeSourceWeight >> idNodeTarget >> nodeTargetWeight)
        {

            graph->inserirAresta(idNodeSource, idNodeTarget, edgeWeight);
            graph->getNo(idNodeSource)->setPeso(nodeSourceWeight);
            graph->getNo(idNodeTarget)->setPeso(nodeTargetWeight);
        }
    }

    graph->criaListaAdjacencia();

    return graph;
}

Grafo *leituraInstancia(ifstream &input_file, int directed, int weightedEdge, int weightedNode)
{

    //Variáveis para auxiliar na criação dos nós no Grafo
    cout << "criando variavei de auxilio " << endl;
    int idNodeSource;
    int idNodeTarget;
    int order;
    int numEdges;

    //Pegando a ordem do grafo
    cout << "pegando ordem do grafo " << endl;
    input_file >> order >> numEdges;

    //Criando objeto grafo
    cout << "criando objeto de grafo " << endl;
    Grafo *graph = new Grafo(order, directed, weightedEdge, weightedNode);

    //Leitura de arquivo
    cout << "fazendo leitura do arquivo " << endl;
    while (input_file >> idNodeSource >> idNodeTarget)
    {

        graph->inserirAresta(idNodeSource, idNodeTarget, 0);
    }

    cout << "voltando para main " << endl;

    graph->criaListaAdjacencia();

    return graph;
}

int menu()
{

    int selecao;

    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "[1] Fecho Transitivo Direto" << endl;
    cout << "[2] Fecho Transitivo Indireto" << endl;
    cout << "[3] Caminho Minimo entre dois vertices - Dijkstra" << endl;
    cout << "[4] Caminho Minimo entre dois vertices - Floyd" << endl;
    cout << "[5] Arvore Geradora Minima de Prim" << endl;
    cout << "[6] Arvore Geradora Minima de Kruskal" << endl;
    cout << "[7] Imprimir caminhamento em profundidade" << endl;
    cout << "[8] Imprimir ordenacao topologica" << endl;
    cout << "[9] dcMST" << endl;
    cout << "[0] Sair" << endl;

    cin >> selecao;

    return selecao;
}
void preencheDot(ofstream &output_file, Grafo *grafo, string nome)
{

    if (grafo->getDirecionado())
    {
        output_file << "strict digraph " << nome << "{ " << endl;
    }
    else
    {
        output_file << "strict graph " << nome << "{ " << endl;
    }
    grafo->criaListaAdjacencia();
    list<No *> listaNos = grafo->getListaNos();
    list<No *>::iterator noIt;
    for (noIt = listaNos.begin(); noIt != listaNos.end(); noIt++)
    {
        list<No *> adj = (*noIt)->getNosAdjacentes();
        if (!adj.empty())
        {
            list<No *>::iterator adjIt;
            for (adjIt = adj.begin(); adjIt != adj.end(); adjIt++)
            {
                if (grafo->getDirecionado())
                {
                    output_file << (*noIt)->getId() << " -> " << (*adjIt)->getId() << endl;
                }
                else
                {
                    output_file << (*noIt)->getId() << " -- " << (*adjIt)->getId() << endl;
                }
            }
        }
    }

    output_file << " } " << endl
                << endl;
}

//Preenche os grafos sem arestas como lista de vertices no output
void preencheFechos(ofstream &output_file, Grafo *grafo, string nome)
{

    list<No *> listaNos = grafo->getListaNos();
    list<No *>::iterator noIt;
    output_file << "Vertices do Fecho " << nome << " : " << endl;
    for (noIt = listaNos.begin(); noIt != listaNos.end(); noIt++)
    {
        output_file << (*noIt)->getId() << " - ";
    }

    output_file << endl
                << endl;
}

void selecionar(int selecao, Grafo *graph, ofstream &output_file)
{

    switch (selecao)
    {

    //Fecho Transitivo Direto
    case 1:
    {
        if (graph->getDirecionado())
        {
            int id;
            cout << "Digite o Identificador do vertice: " << endl;
            cin >> id;
            Grafo *novo = fechoDireto(graph, id);
            preencheFechos(output_file, novo, "Direto");
            break;
        }
        else
        {
            cout << "ERRO: Grafo invalido, tente usar um grafo direcionado" << endl;
        }
        break;
    }
    //Fecho Transitivo Indireto usando um Grafo Direcionado
    case 2:
    {
        if (graph->getDirecionado())
        {
            int id;
            cout << "Digite o Identificador do vertice: " << endl;
            cin >> id;
            Grafo *novo = FechoIndireto(graph, id);
            preencheFechos(output_file, novo, "Indireto");
        }
        else
        {
            cout << "ERRO: Grafo invalido, tente usar um grafo direcionado" << endl;
        }
        break;
    }

        //Caminho mínimo entre dois vértices usando Djkstra;
    case 3:
    {
        int id1, id2;
        cout << "Digite o Identificador do primeiro vertice: " << endl;
        cin >> id1;
        cout << "Digite o Identificador do segundo vertice: " << endl;
        cin >> id2;

        float caminho = Dijkstra(graph, id1, id2);

        if (caminho == 9999999)
        {
            cout << "Sem caminho direto entre os nos " << id1 << " e " << id2 << endl;
            output_file << "Sem caminho direto entre os nos " << id1 << " e " << id2 << endl;
        }

        else
        {
            cout << "Caminho entre os nos " << id1 << " e " << id2 << " : " << caminho << endl;
            output_file << "Caminho entre os nos " << id1 << " e " << id2 << " : " << caminho << endl;
        }
        break;
    }

        //Caminho mínimo entre dois vértices usando Floyd;
    case 4:
    {

        int id1, id2;
        cout << "Digite o Identificador do primeiro vertice: " << endl;
        cin >> id1;
        cout << "Digite o Identificador do segundo vertice: " << endl;
        cin >> id2;

        float custo = custoPinkFloyd(graph, id1, id2);
        if (custo == -99999)
        {
            cout << "Sem caminho direto entre os nos " << id1 << " e " << id2 << endl;
            output_file << "Sem caminho direto entre os nos " << id1 << " e " << id2 << endl;
        }

        else
        {
            cout << "Caminho entre os nos " << id1 << " e " << id2 << " : " << custo << endl;
            output_file << "Caminho entre os nos " << id1 << " e " << id2 << " : " << custo << endl;
        }

        break;
    }

        //AGM Prim;
    case 5:
    {
        if (!graph->getDirecionado())
        {
            list<int> subconjunto;
            int id = 0;
            int cont = 1;
            cout << "Digite os identificadores para adicionar ao subconjunto, ao termino digite -1 para finalizar as insercoes" << endl;
            while (id != -1)
            {

                cout << "Digite o elemento " << cont << " : ";
                cin >> id;
                if (id != -1)
                {
                    subconjunto.push_back(id);
                }

                cont++;
            }
            cout << endl;

            cout << "Entrando em PRIM" << endl;
            Grafo *arvore = Prim(graph, subconjunto);
            preencheDot(output_file, arvore, "Prim");
        }
        else
        {
            cout << "ERRO: Grafo invalido, tente usar um grafo direcionado" << endl;
        }

        break;
    }

        //AGM Kruskal;
    case 6:
    {
        list<int> subconjunto;
        int id = 0;
        int cont = 1;
        cout << "Digite os identificadores para adicionar ao subconjunto, ao termino digite -1 para finalizar as insercoes" << endl;
        while (id != -1)
        {

            cout << "Digite o elemento " << cont << " : ";
            cin >> id;
            subconjunto.push_back(id);
            cont++;
        }
        cout << endl;
        int *subconjuntoVet = new int[subconjunto.size()];

        int i = 0;
        list<int>::iterator it;
        for (it = subconjunto.begin(); it != subconjunto.end(); it++)
        {
            subconjuntoVet[i] = (*it);
            i++;
        }

        Grafo *arvore = kruskal(graph, subconjuntoVet, i - 1);
        preencheDot(output_file, arvore, "Kruskal");
        break;
    }
    //Caminhamento em Profundidade
    case 7:
    {
        int id;
        cout << "Digite o Identificador do vertice: " << endl;
        cin >> id;

        Grafo *arvore = graph->caminhamentoProfundidade(id);
        preencheFechos(output_file, arvore, "Caminhamento");
        break;
    }
    //Ordenação Topologica;
    case 8:
    {
        OrdenacaoTopologica(graph);
        break;
    }
    case 9:
    {
        Grafo *arvore = new Grafo(0, false, false, false);
        gulosoRandomizado(graph, 3, 0.1, 3);
        preencheDot(output_file, arvore, "dcMST");
        break;
    }
    default:
    {
        cout << " Error!!! invalid option!!" << endl;
    }
    }
}

int mainMenu(ofstream &output_file, Grafo *graph)
{

    int selecao = 1;

    while (selecao != 0)
    {
        system("clear");
        selecao = menu();

        if (output_file.is_open())
            selecionar(selecao, graph, output_file);

        else
            cout << "Unable to open the output_file" << endl;

        output_file << endl;
    }

    return 0;
}

int main(int argc, char const *argv[])
{

    //Verificação se todos os parâmetros do programa foram entrados
    if (argc != 6)
    {

        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> <directed> <weighted_edge> <weighted_node> " << endl;
        return 1;
    }

    string program_name(argv[0]);
    string input_file_name(argv[1]);

    string instance;
    if (input_file_name.find("v") <= input_file_name.size())
    {
        string instance = input_file_name.substr(input_file_name.find("v"));
        cout << "Running " << program_name << " with instance " << instance << " ... " << endl;
    }

    //Abrindo arquivo de entrada
    cout << "Abrindo arquivo entrada " << endl;
    ifstream input_file;
    ofstream output_file;
    input_file.open(argv[1], ios::in);
    output_file.open(argv[2], ios::out | ios::trunc);

    Grafo *graph;

    if (input_file.is_open())
    {
        cout << "Carregando grafo " << endl;
        graph = leituraMST(input_file);
    }
    else
    {
        cout << "Unable to open " << argv[1];
    }

    cout << "Abrindo menu " << endl;
    mainMenu(output_file, graph);

    //Fechando arquivo de entrada
    input_file.close();

    //Fechando arquivo de saída
    output_file.close();

    return 0;
}
