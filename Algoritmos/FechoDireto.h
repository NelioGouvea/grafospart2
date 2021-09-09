#ifndef FECHODIRETO_H_INCLUDED
#define FECHODIRETO_H_INCLUDED

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"
#include "../Algoritmos/dcMST.h"
#include <iostream>
#include <stdlib.h>


//preenche a lista de nós visitados
void auxFechoDireto(Grafo *grafo, No *noAtual, list<No *> *visitados)
{
    //verifica se o nó atual da recursão já foi visitado
    if (grafo->verificaVisita(noAtual, *visitados))
    {
        //se nao for, adiciona ele a lista de visitados e continua a recursão
        visitados->push_back(noAtual);

        //pega a lista de adjacencia do nó atual e itera ela, e para cada nó adjacente que não foi visitado
        //envia ele para a recursão
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

//Retorna um novo grafo sem aresta com todos os nós que o vértice idAlvo alcança dentro do grafo enviado
Grafo* fechoDireto(Grafo *grafo, int idAlvo)
{
   
    No *no = grafo->getNoVet(idAlvo);
    list<No *> visitados;
    Grafo* fechoDireto = new Grafo(0, false, false, false);
    auxFechoDireto(grafo, no, &visitados);
    list<No *>::iterator visitadosIt;
    for (visitadosIt = visitados.begin(); visitadosIt != visitados.end(); visitadosIt++)
    {
        fechoDireto->inserirNo((*visitadosIt)->getId());
    }
    return fechoDireto;

};

#endif 