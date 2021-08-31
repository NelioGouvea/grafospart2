#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"

#include <list>
#include <map>

#include <iostream>

#define INFINITO 9999999

using namespace std;

// funcao auxiliar que verifica se um no esta contido numa lista
bool contemNo(list<No*> listaNo, No *noAlvo) {
    list<No*>::iterator noAtual;
    for(noAtual = listaNo.begin(); noAtual != listaNo.end(); noAtual++)
        if((*noAtual) == noAlvo)
            return true;
    return false;                   
}

// funcao dijkstra, recebe um grafo, o id de dois nos como parametros, e retorna o caminho minino entre eles
float Dijkstra(Grafo *grafo, int fonteId, int alvoId) {
    No *noFonte = grafo->getNo(fonteId);
    No *noAlvo = grafo->getNo(alvoId);

    list<No*> listaNo = grafo->getListaNos(); 

    // inicializa os valores das listas
    
    // lista S no slide
    list<No*> nosVisitados; // lista que guarda o no fonte
    
    // lista S barra no slide
    list<No*> nosNaoVisitados; // lista que guarda os nos que nao foram visitados
    
    // lista pi no slide
    map<No*, int> custos; // dicionario que recebe um no e guarda seu peso referente ao no fonte
    
    list<No*>::iterator noAux;
    for(noAux = listaNo.begin(); noAux != listaNo.end(); noAux++) {
        if((*noAux) == noFonte) { // o custo de um no com ele mesmo recebe 0
            nosVisitados.push_back(*noAux);
            custos[*noAux] = 0;
        }
        else {
            nosNaoVisitados.push_back(*noAux);
            
            int custo = 0;
            if(contemNo(noFonte->getNosAdjacentes(), (*noAux))) // se o no alvo fizer parte da vizinhanca
                custo = noFonte->getAresta((*noAux)->getId())->getPeso(); // o custo recebe o peso da aresta entre eles
            else // se nao fizer parte da vizinhanca
                custo = INFINITO; // o custo recebe infinito
            custos[*noAux] = custo;
        }
    }
    
    while(!nosNaoVisitados.empty()) {
        int menorCusto = INFINITO;
        No *noMenorCusto = nullptr;

        // seleciona um no, tal que ele tem o menor custo 
        for(noAux = listaNo.begin(); noAux != listaNo.end(); noAux++) {
            if(contemNo(nosNaoVisitados, (*noAux))) {
                if(custos[*noAux] <= menorCusto) {
                    menorCusto = (custos[*noAux]);
                    noMenorCusto = (*noAux);
                }
            }
        }

        // remove o no com menor custo da lista de nao visitados
        nosNaoVisitados.remove(noMenorCusto);
        // insere ele na lista de visitados (nao sei se realmente precisa disso...)
        nosVisitados.push_back(noMenorCusto);

        // calcula o novo custo
        for(noAux = listaNo.begin(); noAux != listaNo.end(); noAux++) {
            if(contemNo(noMenorCusto->getNosAdjacentes(), (*noAux))) {
                int custoAux = noMenorCusto->getAresta((*noAux)->getId())->getPeso();

                if(custoAux + menorCusto < custos[*noAux]) { // se o novo custo for menor que o atual
                    custos[*noAux] = custoAux + menorCusto; // o custo eh atualizado

                    nosNaoVisitados.push_back(noMenorCusto); // o no eh devolvido na lista de nao visitados
                    nosVisitados.remove(noMenorCusto); // e removido da lista de visitados
                }
            }
        }
    }
        
    return custos[noAlvo];
}

#endif
