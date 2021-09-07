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

int randomRange(int menor, int maior) {
       return rand()%(maior-menor+1) + menor;
}

void gulosoRandomizado(Grafo *grafo, int d, float alfa, int numIteracoes) {
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
		cout << "Peso = " << calculaPeso(s) << endl;

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
	list<No*> lista = solBest->getListaNos();

	// for(auto a = lista.begin(); a != lista.end(); a++) {
	// cout << (*a)->getId() << " || ";

	// 	list<No*> listaAdj = (*a)->getNosAdjacentes();
	// 	for(auto b = listaAdj.begin(); b != listaAdj.end(); b++)
	// 		cout << (*b)->getId() <<  " - peso " << (*a)->getAresta((*b)->getId())->getPeso() << " - ";
	// 	cout << endl;
	// }
	// cout << endl;

	int peso = calculaPeso(solBest);
	cout << "Peso total = " << peso << endl;

	execute_falha_segmentacao();
}

#endif // GULOSORANDOMIZADO_H_INCLUDED