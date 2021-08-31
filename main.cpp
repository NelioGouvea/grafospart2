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
#include <string>

using namespace std;

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
    cout << "[3] Caminho Mínimo entre dois vertices - Dijkstra" << endl;
    cout << "[4] Caminho Mínimo entre dois vertices - Floyd" << endl;
    cout << "[5] Arvore Geradora Mínima de Prim" << endl;
    cout << "[6] Arvore Geradora Mínima de Kruskal" << endl;
    cout << "[7] Imprimir caminhamento em profundidade" << endl;
    cout << "[8] Imprimir ordenacao topologica" << endl;
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
    list<No *> listaNos = grafo->getListaNos();

    list<No *>::iterator noIt;
    for (noIt = listaNos.begin(); noIt != listaNos.end(); noIt++)
    {
        list<No *> adj = (*noIt)->getNosAdjacentes();
        list<No *>::iterator adjIt;
        for (adjIt = adj.begin(); adjIt != adj.end(); adjIt++)
        {
            output_file << (*noIt)->getId() << " -- " << (*adjIt)->getId() << endl;
        }
    }

    output_file << " } " << endl << endl;
}
void selecionar(int selecao, Grafo *graph, ofstream &output_file)
{

    switch (selecao)
    {

    //Fecho Transitivo Direto usando um Grafo Direcionado
    case 1:
    {
        if (graph->getDirecionado())
        {
            int id;
            cout << "Digite o Identificador do vertice: " << endl;
            cin >> id;
            fechoDireto(graph, id);
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
            preencheDot(output_file, novo, "Fecho Transitivo Indireto");
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

        if(caminho == 9999999)
            cout << "Sem caminho direto entre os nos " << id1 << " e " << id2 << endl;
        else
            cout << "Caminho entre os nos " << id1 << " e " << id2 << " : " << caminho << endl;

        break;
    }

        //Caminho mínimo entre dois vértices usando Floyd;
    case 4:
    {
        if (graph->getDirecionado())
        {
            int id1, id2;
            cout << "Digite o Identificador do primeiro vertice: " << endl;
            cin >> id1;
            cout << "Digite o Identificador do segundo vertice: " << endl;
            cin >> id2;

            float custo = custoPinkFloyd(graph, id1, id2);
        }
        else
        {

            cout << "ERRO: Grafo invalido, tente usar um grafo direcionado" << endl;
        }

        break;
    }

        //AGM Prim;
    case 5:
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

        Grafo *arvore = Prim(graph, subconjunto);
        preencheDot(output_file, arvore, "Prim");
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

        Grafo *arvore = kruskal(graph, subconjuntoVet, subconjunto.size());
        preencheDot(output_file, graph, "Kruskal");
        break;
    }
    //Caminhamento em Profundidade
    case 7:
    {
        int id;
        cout << "Digite o Identificador do vertice: " << endl;
        cin >> id;

        Grafo *arvore = graph->caminhamentoProfundidade(id);
        preencheDot(output_file, graph, "Caminhamento");
        break;
    }
    //Ordenação Topologica;
    case 8:
    {

        OrdenacaoTopologica(graph);
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
        graph = leitura(input_file, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
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
