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
#include "Algoritmos/dcMST.h"

using namespace std;

int main()
{
    std::cout << "Criando Grafo Ordem Zero" << endl;
    Grafo *grafo = new Grafo(9, false, true, false);

    // std::cout << "Inserindo Nos" << endl;

    // std::cout << "Conferindo Insersao" << endl;

    std::cout << "Numero de Nos : " << grafo->getOrdem() << endl
              << endl;

    std::cout << "Inserindo Arestas" << endl;
    
    grafo->inserirAresta(1, 2, 224);
    grafo->inserirAresta(1, 3, 224);
    grafo->inserirAresta(1, 4, 361);
    grafo->inserirAresta(1, 5, 671);
    grafo->inserirAresta(1, 6, 300);
    grafo->inserirAresta(1, 7, 539);
    grafo->inserirAresta(1, 8, 800);
    grafo->inserirAresta(1, 9, 943);

    grafo->inserirAresta(2, 3, 200);
    grafo->inserirAresta(2, 4, 200);
    grafo->inserirAresta(2, 5, 447);
    grafo->inserirAresta(2, 6, 283);
    grafo->inserirAresta(2, 7, 400);
    grafo->inserirAresta(2, 8, 728);
    grafo->inserirAresta(2, 9, 762);

    grafo->inserirAresta(3, 4, 400);
    grafo->inserirAresta(3, 5, 566);
    grafo->inserirAresta(3, 6, 447);
    grafo->inserirAresta(3, 7, 600);
    grafo->inserirAresta(3, 8, 922);
    grafo->inserirAresta(3, 9, 949);

    grafo->inserirAresta(4, 5, 400);
    grafo->inserirAresta(4, 6, 200);
    grafo->inserirAresta(4, 7, 200);
    grafo->inserirAresta(4, 8, 539);
    grafo->inserirAresta(4, 9, 583);

    grafo->inserirAresta(5, 6, 600);
    grafo->inserirAresta(5, 7, 447);
    grafo->inserirAresta(5, 8, 781);
    grafo->inserirAresta(5, 9, 510);

    grafo->inserirAresta(6, 7, 283);
    grafo->inserirAresta(6, 8, 500);
    grafo->inserirAresta(6, 9, 707);

    grafo->inserirAresta(7, 8, 361);
    grafo->inserirAresta(7, 9, 424);

    grafo->inserirAresta(8, 9, 500);
    

    //grafo->inserirAresta(1, 2, 9);
    // grafo->inserirAresta(1, 3, 15);
    // grafo->inserirAresta(1, 4, 17);
    // grafo->inserirAresta(1, 5, 8);
    // grafo->inserirAresta(1, 6, 19);

    // grafo->inserirAresta(2, 3, 12);
    // grafo->inserirAresta(2, 4, 18);
    // grafo->inserirAresta(2, 5, 6);
    // grafo->inserirAresta(2, 6, 20);

    // grafo->inserirAresta(3, 4, 13);
    // grafo->inserirAresta(3, 5, 10);
    // grafo->inserirAresta(3, 6, 21);

    // grafo->inserirAresta(4, 5, 7);
    // grafo->inserirAresta(4, 6, 5);

    // grafo->inserirAresta(5, 6, 22);

    std::cout << "Numero de arestas : " << grafo->getNumeroArestas() << endl
              << endl;
    
    grafo->criaListaAdjacencia();
    grafo->imprimeListaAdjacencia();


    cout << dcMST(grafo, 3) << endl;
}