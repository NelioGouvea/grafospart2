#ifndef ORDNACAOTOPOLOGICA_H
#define ORDNACAOTOPOLOGICA_H

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"
#include "../Algoritmos/dcMST.h"

#include <list>

#include <iostream>

using namespace std;

// funcao auxiliar que verifica se um grafo tem um ciclo
bool auxContemCiclo(list<No*> listaNos, No *noAlvo, list<No*> *nosVisitados) {
    bool contem = contemNo(listaNos, noAlvo);

    list<No*>::iterator noAtual;
    for(noAtual = listaNos.begin(); noAtual != listaNos.end(); noAtual++) {
        if(!contemNo((*nosVisitados), (*noAtual))) {
            nosVisitados->push_back(*noAtual);
            
            contem = contem || auxContemCiclo((*noAtual)->getNosAdjacentes(), noAlvo, nosVisitados);
        }
    }
    return contem;
}

// funcao auxiliar que verifica se um grafo tem um ciclo
bool contemCiclo(Grafo *grafo) {
    list<No*> listaNos = grafo->getListaNos();

    bool contem = false;

    list<No*>::iterator noAtual;
    for(noAtual = listaNos.begin(); noAtual != listaNos.end(); noAtual++) {
        list<No*> nosVisitados;
        contem = contem || auxContemCiclo((*noAtual)->getNosAdjacentes(), (*noAtual), &nosVisitados);
    }
        
    return contem;
}

// funcao que faz a ordenacao topologica, baseada no algoritmo de Kahn
void OrdenacaoTopologica(Grafo *grafo) { // verfica se o grafo eh aciclico direcionado
    if(grafo->getDirecionado() && !contemCiclo(grafo)) {
        list<No*> listaOrdenada; // lista que guarda os nos ordenados
        list<No*> listaAux; // lista auxiliar, que guarda nos com grau de entrada iguais a 0

        list<No*> listaNos = grafo->getListaNos();

        list<No*>::iterator noAtual;
        for(noAtual = listaNos.begin(); noAtual != listaNos.end(); noAtual++) {
            if((*noAtual)->getGrauEntrada() == 0) { // se o grau de entrada eh 0
                listaAux.push_back(*noAtual); // insere na lista auxiliar
            }
        }

        while(!listaAux.empty()) {
            for(noAtual = listaNos.begin(); noAtual != listaNos.end(); noAtual++) {
                if(contemNo(listaAux, (*noAtual))) {
                    listaAux.remove(*noAtual); // remove o no da lista auxliar
                    listaOrdenada.push_back(*noAtual); // e insere na lista ordenada

                    list<No*> lista = (*noAtual)->getNosAdjacentes();
                    list<No*>::iterator noAux;
                    // pega os nos adjacentes, "remove" a aresta entre eles
                    for(noAux = lista.begin(); noAux != lista.end(); noAux++) {
                        
                        (*noAux)->incrementaGrauEntrada(-1); // "remove" a arestra

                        if((*noAux)->getGrauEntrada() == 0) // se o no adjacente tem o grau de entrada 0
                            listaAux.push_back(*noAux); // insere ele na lista auxiliar
                    }
                }
            }
        }      

        // imprime uma, das possiveis ordenacoes topologicas
        cout << "Uma ordenacao topologica: ";
        for(noAtual = listaOrdenada.begin(); noAtual != listaOrdenada.end(); noAtual++)
            cout << (*noAtual)->getId() << " ";
        cout << endl;
    }
    else
        cout << "O grafo nao eh um grafo aciclico direcionado" << endl;
}

#endif