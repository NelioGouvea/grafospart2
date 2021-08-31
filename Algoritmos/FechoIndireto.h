#ifndef FECHOINDIRETO_H
#define FECHOINDIRETO_H

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"

#include "Dijkstra.h"

#include <list>

#include <iostream>

using namespace std;

// funcao auxiliar que acha os nos ascendentes
void achaNosAscendentes(No* noAlvo, list<No*> *fecho, list<No*> listaNos) {
    list<No*>::iterator noAtual;
    for(noAtual = listaNos.begin(); noAtual != listaNos.end(); noAtual++) {
        if(contemNo((*noAtual)->getNosAdjacentes(), noAlvo)) { // verifica se um no eh adjacente a outro
            if(!contemNo((*fecho), (*noAtual))) { // se for, e nao estiver no fecho
                fecho->push_back(*noAtual); // o outro no eh inserido na lista 
                achaNosAscendentes((*noAtual), fecho, listaNos); // procura os outros ascendentes
            }
        }
    }
}

// funcao que encontra o fecho transitivo indireto de um no
Grafo* FechoIndireto(Grafo *grafo, int idAlvo) {
    No *noAlvo = grafo->getNo(idAlvo);

    list<No*> fecho; // lista com os nos ascendentes 

    list<No*> listaNos = grafo->getListaNos();
    
    list<No*>::iterator noAtual;
    for(noAtual = listaNos.begin(); noAtual != listaNos.end(); noAtual++) {
        if(contemNo((*noAtual)->getNosAdjacentes(), noAlvo)) {  // verifica se um no eh adjacente a outro
            if(!contemNo(fecho, (*noAtual))) { // se for, e nao estiver no fecho
                fecho.push_back(*noAtual); // o outro no eh inserido na lista 
                achaNosAscendentes((*noAtual), &fecho, listaNos); // procura os outros ascendentes
            }
        }
    }

    Grafo *novoGrafo = new Grafo(0, false, false, false);

    cout << "Fecho Transitivo Indireto do no " << idAlvo << ": " << endl;
    for(noAtual = fecho.begin(); noAtual != fecho.end(); noAtual++)
        if((*noAtual) != noAlvo) {
            cout << (*noAtual)->getId() << " ";
            novoGrafo->inserirNo((*noAtual)->getId());
        }
    cout << endl;

    return novoGrafo;
}

#endif
