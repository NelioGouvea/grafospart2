#ifndef GULOSO_H_INCLUDED
#define GULOSO_H_INCLUDED

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"
#include "../Algoritmos/FechoDireto.h"
#include "../Algoritmos/FechoIndireto.h"

#include <list>
#include <map>

#include <iostream>

#define INFINITO 9999999

using namespace std;

#include "../Algoritmos/dcMST.h"

#define INFINITO 9999999

using namespace std;

//função auxiliar para ordenar a lista de arestas do menor peso para o maior
void ordenaVetorArestas(Grafo *grafo, list<Aresta *> *todasArestas)
{
	map<int, bool> visitado;
	
	list<No *> listaNos = grafo->getListaNos();
	for (auto no = listaNos.begin(); no != listaNos.end(); no++)
		visitado[(*no)->getId()] = false;

	
	for (auto no = listaNos.begin(); no != listaNos.end(); no++)
	{

		list<Aresta *> listaAresta = (*no)->getListaAresta();
		for (auto aresta = listaAresta.begin(); aresta != listaAresta.end(); aresta++)
		{
			if (visitado[(*aresta)->getAlvoId()] == false)
				todasArestas->push_back((*aresta));
		}

		visitado[(*no)->getId()] = true;
	}
	
	Aresta **vet = new Aresta *[todasArestas->size()];
	//Aresta **vet = (Aresta**)malloc(sizeof(todasArestas->size()));

	int i = 0;
	for (list<Aresta *>::iterator aresta = todasArestas->begin(); aresta != todasArestas->end(); aresta++)
	{
		
		vet[i] = (*aresta);
		i++;
	}
	
	quickSort(vet, 0, i);

	for (int j = 0; j < i; j++)
	{
		todasArestas->pop_front();
		todasArestas->push_back(vet[j]);
	}
		
}

Grafo *guloso(Grafo *grafo, int d, int *peso, double *tempo)
{
	clock_t t = clock();

	//cria- se uma lista de arestas
	list<Aresta *> todasArestas;

	//ordena-se a lista pelo peso das arestas (da menor para maior)
	ordenaVetorArestas(grafo, &todasArestas);

	//cria-se uma arvore sem arestas para cada nó
	//ou simplificando, um grafo de nós "soltos"( sem aresta )
	Grafo *s = new Grafo(grafo->getOrdem(), false, true, false);
	No *noFonte;
	No *noAlvo;
	Grafo *fecho;
	s->geraVetNo();
	//cria-se um contador que representará a quantidade de arestas = vertices-1
	int cont = 0;

	//enquanto a lista de arestas não for vazia e a quantidade de arestas não ser o limite
	while (!todasArestas.empty() && cont < grafo->getOrdem() - 1)
	{

		//seleciona a aresta de menor custo da lista ordenada de arestas
		// e os par de nó da sua extremidade
		noFonte = s->getNoVet(todasArestas.front()->getFonteId());
		noAlvo = s->getNoVet(todasArestas.front()->getAlvoId());

		s->criaListaAdjacencia();

		fecho = fechoDireto(s, noFonte->getId());

		//se os nós da extremidade da arestas não estiverem na mesma subArvore
		// e se o grau desses nós forem menor que d (nesse caso, menor que 3)
		if ((fecho->getListaNos().empty() || !fecho->procuraNo(noAlvo->getId())) && noAlvo->getGrau() < d && noFonte->getGrau() < d)
		{

			//insere-se a aresta de menor peso entre esses nós (vertices)
			s->inserirAresta(noFonte->getId(), noAlvo->getId(), todasArestas.front()->getPeso());

			//atualiza a quantidade de arestas
			cont++;
		}

		//remove a primeira aresta de menor peso
		todasArestas.pop_front();
		
	}

	s->criaListaAdjacencia();

	//calcula o peso da arvore, ou seja, o somatorio dos pesos de todas as arestas
	*peso = calculaPeso(s);

	t = clock() - t;
	*tempo = t / (double)CLOCKS_PER_SEC;

	return s;
}

#endif // GULOSO_H_INCLUDED