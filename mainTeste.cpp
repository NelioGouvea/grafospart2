#include <iostream>
#include <stdlib.h>
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

using namespace std;

int main()
{
    std::cout << "Criando Grafo Ordem Zero" << endl;
    Grafo *grafo = new Grafo(6, false, true, false);

    // std::cout << "Inserindo Nos" << endl;

    // std::cout << "Conferindo Insersao" << endl;

    std::cout << "Numero de Nos : " << grafo->getOrdem() << endl
              << endl;

    std::cout << "Inserindo Arestas" << endl;

    grafo->inserirAresta(1, 2, 4);
    grafo->inserirAresta(1, 4, 3);
    grafo->inserirAresta(1, 5, 4);
    grafo->inserirAresta(2, 5, 9);
    grafo->inserirAresta(2, 6, 5);
    grafo->inserirAresta(2, 3, 8);
    grafo->inserirAresta(3, 4, 9);
    grafo->inserirAresta(3, 6, 2);
    grafo->inserirAresta(3, 5, 3);
    grafo->inserirAresta(4, 6, 7);
    grafo->inserirAresta(5, 6, 2);

    std::cout << "Numero de arestas : " << grafo->getNumeroArestas() << endl
              << endl;
    int vetor[] = {1, 2, 3, 4, 5, 6};
    Grafo *novo = kruskal(grafo, vetor, 6);
}