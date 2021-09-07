#ifndef GULOSO_H_INCLUDED
#define GULOSO_H_INCLUDED

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"

#include <list>
#include <map>

#include <iostream>

#define INFINITO 9999999

using namespace std;

#include "../Algoritmos/Kruskal.h"
#include "../Algoritmos/dcMST.h"

#define INFINITO 9999999

using namespace std;

void ordenaVetorArestas(Grafo *grafo, list<Aresta*> *todasArestas) {
	map<int, bool> visitado;

	list<No*> listaNos = grafo->getListaNos();
	for(auto no = listaNos.begin(); no != listaNos.end(); no++)
		visitado[(*no)->getId()] = false;

	for(auto no = listaNos.begin(); no != listaNos.end(); no++) {
		
		list<Aresta*> listaAresta = (*no)->getListaAresta();
		for(auto aresta = listaAresta.begin(); aresta != listaAresta.end(); aresta++) {
			if(visitado[(*aresta)->getAlvoId()] == false)
				todasArestas->push_back((*aresta));
		}

		visitado[(*no)->getId()] = true;
	}

	Aresta **vet = new Aresta *[todasArestas->size()];

	int i = 0;
	for(auto aresta = todasArestas->begin(); aresta != todasArestas->end(); aresta++) {
		vet[i] = (*aresta);
		i++;
	}

	quickSort(vet, 0, i);

	for (int j = 0; j < i; j++) {
        todasArestas->pop_front();
        todasArestas->push_back(vet[j]);
    }
}

Grafo* guloso(Grafo *grafo, int d, int *peso, double *tempo) {
	clock_t t = clock();
	
	list<Aresta*> todasArestas;

	ordenaVetorArestas(grafo, &todasArestas);

	Grafo *s = new Grafo(grafo->getOrdem(), false, true, false);

	int cont = 0;
	while(!todasArestas.empty() && cont < grafo->getOrdem() - 1) {
		No *noFonte = s->getNo(todasArestas.front()->getFonteId());
		No *noAlvo = s->getNo(todasArestas.front()->getAlvoId());

        s->criaListaAdjacencia();
        
		Grafo *fecho = fechoDireto(s, noFonte->getId());
    
        if ((fecho->getListaNos().empty() || !fecho->procuraNo(noAlvo->getId())) && noAlvo->getGrau() < d && noFonte->getGrau() < d) {   
            
            s->inserirAresta(noFonte->getId(), noAlvo->getId(), todasArestas.front()->getPeso());
            cont++;
        }

		todasArestas.pop_front();
	}

	s->criaListaAdjacencia();

	*peso = calculaPeso(s);
	
	t = clock() - t;
	*tempo = t / (double) CLOCKS_PER_SEC;

	return s;
}

#endif // GULOSO_H_INCLUDED