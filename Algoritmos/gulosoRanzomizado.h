#ifndef GULOSORANDOMIZADO_H_INCLUDED
#define GULOSORANDOMIZADO_H_INCLUDED

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"

#include <list>
#include <map>
#include <ctime>
#include <stdlib.h>

#include <iostream>

#include "../Algoritmos/Kruskal.h"
#include "../Algoritmos/dcMST.h"
#include "../Algoritmos/guloso.h"

#define INFINITO 9999999

using namespace std;

int randomRange(int menor, int maior) {
       return rand()%(maior-menor+1) + menor;
}

Grafo* gulosoRandomizado(Grafo *grafo, int d, float alfa, int numIteracoes, int *peso, double *tempo) {
	clock_t t = clock();
	
	list<Aresta*> todasArestas;

	ordenaVetorArestas(grafo, &todasArestas);

	Grafo *s;
	Grafo *solBest = nullptr;

	int i = 0, k;
	while(i < numIteracoes) {
		i++;
		s = new Grafo(grafo->getOrdem(), false, true, false);
		
		int cont = 0;
		do {
			srand(time(NULL));
			
			k = randomRange(0, alfa*(todasArestas.size() -1));

			Aresta *random;
			int j = 0;
			for(auto aresta = todasArestas.begin(); aresta != todasArestas.end(); aresta++) {
				if(j == k) {
					random = (*aresta);
				}
				j++;
			}

			No *noFonte = s->getNo(random->getFonteId());
			No *noAlvo = s->getNo(random->getAlvoId());
			
			s->criaListaAdjacencia();
			
			Grafo *fecho = fechoDireto(s, noFonte->getId());
		
			if ((fecho->getListaNos().empty() || !fecho->procuraNo(noAlvo->getId())) && noAlvo->getGrau() < d && noFonte->getGrau() < d) {   
				
				s->inserirAresta(noFonte->getId(), noAlvo->getId(), random->getPeso());
				cont++;
			}

			todasArestas.remove(random);

		} while(cont < grafo->getOrdem() - 1);

		s->criaListaAdjacencia();

		if(solBest == nullptr)
			solBest = s;
		
		else {
			int pesoSolBest = calculaPeso(solBest);
			int pesoS = calculaPeso(s);

			if(pesoS < pesoSolBest)
				solBest = s;
		}
	}

	solBest->criaListaAdjacencia();

	*peso = calculaPeso(solBest);

	t = clock() - t;
	*tempo = t / (double) CLOCKS_PER_SEC;

	return solBest;
}

#endif // GULOSORANDOMIZADO_H_INCLUDED