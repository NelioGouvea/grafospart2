#ifndef GULOSORANDOMIZADO_H_INCLUDED
#define GULOSORANDOMIZADO_H_INCLUDED

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"
#include "../Algoritmos/FechoDireto.h"
#include "../Algoritmos/FechoIndireto.h"

#include <list>
#include <map>
#include <ctime>
#include <stdlib.h>

#include <iostream>

#include "../Algoritmos/dcMST.h"
#include "../Algoritmos/guloso.h"

#define INFINITO 9999999

using namespace std;

// funcao auxiliar que gera um numero aleatorio no intervalo [menor, maior]
int randomRange(int menor, int maior) {
       return rand()%(maior-menor+1) + menor;
}

Grafo* gulosoRandomizado(Grafo *grafo, int d, float alfa, int numIteracoes, int *peso, double *tempo) {
	clock_t t = clock();
	
	// cria uma lista com todas as arestas
	list<Aresta*> todasArestas;
	
	//ordena a lista pelo peso das arestas (da menor para maior)
	ordenaVetorArestas(grafo, &todasArestas);
	
	// cria uma lista auxiliar para nao perder todas as arestas, qunado houver a remocao no while
	list<Aresta*> auxAresta = todasArestas;

	Grafo *s;
	Grafo *solBest = nullptr;

	int i = 0, k;
	
	//enquanto nao foram feitas todas as iteracoes
	while(i < numIteracoes) {
		i++;
		
		s = new Grafo(grafo->getOrdem(), false, true, false);
		s->geraVetNo();
		int cont = 0;
		
		//enquanto a lista de arestas não for vazia e a quantidade de arestas não ser o limite
		do {
			srand(time(NULL));
			
			// gera um valor aleatorio para k
			k = randomRange(0, alfa*(todasArestas.size() -1));
			
			// guarda a aresta da posicao k 
			Aresta *random;
			
			int j = 0;
			
			// pega a aresta na posicao k da lista com todas as arestas
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
			
			
			// se os nós da extremidade da arestas não estiverem na mesma subArvore
			// e se o grau desses nós forem menor que d (nesse caso, menor que 3)
			if ((fecho->getListaNos().empty() || !fecho->procuraNo(noAlvo->getId())) && noAlvo->getGrau() < d && noFonte->getGrau() < d) {   
				
				//insere-se a aresta de menor peso entre esses nós (vertices)
				s->inserirAresta(noFonte->getId(), noAlvo->getId(), random->getPeso());
				//atualiza a quantidade de arestas
				cont++;
			}
			
			// remove a aresta da lista
			todasArestas.remove(random);

		} while(cont < grafo->getOrdem() - 1);
		
		// todasArestas recebe seu valor original
		todasArestas = auxAresta;

		s->criaListaAdjacencia();

		// solBest igual a nullptr quer dizer a primeira iteracao
		if(solBest == nullptr)
			solBest = s; // solbest recebe o s
		
		else {
			// calcula o peso de solBest e s
			int pesoSolBest = calculaPeso(solBest);
			int pesoS = calculaPeso(s);
			
			// a arvore de menor peso e guardada em solBest
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
