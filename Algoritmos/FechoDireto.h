#ifndef FECHODIRETO_H_INCLUDED
#define FECHODIRETO_H_INCLUDED

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"
#include <iostream>
#include <stdlib.h>

void auxFechoDireto(Grafo *grafo, No *noAtual, list<No *> *visitados)
{
    
    if (grafo->verificaVisita(noAtual, *visitados))
    {
        
        visitados->push_back(noAtual);
        list<No *> listaAdj = noAtual->getNosAdjacentes();

        list<No *>::iterator listaAdjIt;
        for (listaAdjIt = listaAdj.begin(); listaAdjIt != listaAdj.end(); listaAdjIt++)
        {
            
            if (grafo->verificaVisita((*listaAdjIt), *visitados))
            {
                
                auxFechoDireto(grafo, (*listaAdjIt), visitados);
            }
        }
    }
};

void fechoDireto(Grafo *grafo, int idAlvo)
{

    No *no = grafo->getNo(idAlvo);
    list<No *> visitados;

    auxFechoDireto(grafo, no, &visitados);
     cout << visitados.size() << endl;
    cout << "Fecho do vertice " << idAlvo << " : ";
    list<No *>::iterator visitadosIt;
    for (visitadosIt = visitados.begin(); visitadosIt != visitados.end(); visitadosIt++)
    {
        cout << " - " << (*visitadosIt)->getId();
    }
};

#endif 