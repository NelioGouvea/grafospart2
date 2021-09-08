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
#include "Algoritmos/guloso.h"
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


int menu()
{

    int selecao;

    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "[1] Guloso" << endl;
    cout << "[2] Guloso randomizado" << endl;
    cout << "[3] Guloso randomizado reativo" << endl;
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
   
    case 1:
    {
        double tempo;
        int peso;
        Grafo *arvore = guloso(graph, 3, &peso, &tempo);
        preencheDot(output_file, arvore, "guloso");
        output_file << endl;
        output_file << "Peso total = " << peso << endl;
        output_file << "Tempo total = " << tempo << " s" << endl;
        break;
    }
    case 2:
    {
        double tempo;
        int peso;
        Grafo *arvore = gulosoRandomizado(graph, 3, 0.5, 3, &peso, &tempo);
        preencheDot(output_file, arvore, "gulosoRandomizado");
        output_file << endl;
        output_file << "Peso total = " << peso << endl;
        output_file << "Tempo total = " << tempo << " s" << endl;
        break;
    }
    // case 3:
    // {
    //     double tempo;
    //     int peso;
    //     Grafo *arvore = gulosoRandomizado(graph, 3, 0.5, 3, &peso, &tempo);
    //     preencheDot(output_file, arvore, "gulosoRandomizado");
    //     output_file << endl;
    //     output_file << "Peso total = " << peso << endl;
    //     output_file << "Tempo total = " << tempo << " s" << endl;
    //     break;
    // }
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
