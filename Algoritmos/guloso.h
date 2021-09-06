#ifndef DCMST_H_INCLUDED
#define DCMST_H_INCLUDED

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"

#include <list>
#include <map>

#include <iostream>

#define INFINITO 9999999

using namespace std;


// funcao auxiliar que ordena um vetor de nohs
// a ordenacao comparada pelo peso da aresta entre os nohs no vetor e um noh de referencia
void quickSort(No **vet, No *noReferencia, int p, int q) {
    int pivo = noReferencia->getAresta(vet[(int)((p + q) / 2)]->getId())->getPeso();
    int i = p;
    int j = q - 1;

    while (i <= j) {
		while (noReferencia->getAresta(vet[i]->getId())->getPeso() < pivo)
            i++;

        while (noReferencia->getAresta(vet[j]->getId())->getPeso() > pivo)
            j--;

        if (i <= j) {
            No *aux;
            aux = vet[i];
            vet[i] = vet[j];
            vet[j] = aux;
            i++;
            j--;
        }
    }

    if (p < j)
        quickSort(vet, noReferencia, p, j + 1);

    if (i < q)
    {
        quickSort(vet, noReferencia, i, q);
    }
}

// funcao auxiliar que verifica se um noh esta numa lista
bool contem(list<No*> listaNo, No *noAlvo) {
    list<No*>::iterator noAtual;
    for(noAtual = listaNo.begin(); noAtual != listaNo.end(); noAtual++)
        if((*noAtual) == noAlvo)
            return true;
    return false;                   
}

// funcao auxiliar que insere na arvore os nohs que ainda entao com grau 0
void insereRestanteNos(list<No*> nosEstadoEspera, list<No*> nosVisitados, Grafo *arvore, int d) {
	// lista auxiliar, inicialmente recebe uma copia da lista nosVisitados
	// usada para achar nohs para inserir arestas
	list<No*> listaAux = nosVisitados;
	
	// percorre a lista noEstadoEspera
	for(auto noAtual = nosEstadoEspera.begin(); noAtual != nosEstadoEspera.end(); noAtual++) {
		
		No *noMenorPeso = listaAux.front();
        int menorPeso = (*noAtual)->getAresta(noMenorPeso->getId())->getPeso();

		// procura pelo noh com a aresta com o menor peso
		for(auto noAux = listaAux.begin(); noAux != listaAux.end(); noAux++) {
			if((*noAtual)->getAresta((*noAux)->getId())->getPeso() < menorPeso) {
				menorPeso = (*noAtual)->getAresta((*noAux)->getId())->getPeso();
				noMenorPeso = (*noAux);
			}
		}

		// insere o noAtual na arvore
		if(arvore->getNo((*noAtual)->getId()) == nullptr)
			arvore->inserirNo((*noAtual)->getId());

		// insere o noMenorPeso na arvore
		if(arvore->getNo(noMenorPeso->getId()) == nullptr)
			arvore->inserirNo(noMenorPeso->getId());

		// insere a aresta entre os dois na arvore
		arvore->inserirAresta((*noAtual)->getId(), noMenorPeso->getId(), menorPeso);

		// aumenta o grau dos dois nohs
		(*noAtual)->setGrau((*noAtual)->getGrau() + 1);
		noMenorPeso->setGrau(noMenorPeso->getGrau() + 1);
		
		// agora o noAtual tem uma aresta, ele eh inserido na listaAux
		listaAux.push_back((*noAtual));

		// se o noMenorPeso ficou com grau maximo, ele eh removido da listaAux
		if(noMenorPeso->getGrau() == d)
			listaAux.remove(noMenorPeso);
	}
}

// fase um da heuristica
void faseUm(Grafo *grafo, Grafo *arvore, int d) {
	// guarda o peso de cada noh
	map<No*, int> pesos;

	// guarda a lista dos d-primeiros nohs adjacentes de cada noh
	map<No*, list<No*>> dNosAdjacentes;

	// lista de todos os nohs
	list<No*> listaNosGrafo = grafo->getListaNos();
	
	// percorre a lista de nohs do grafo, calculando o peso e settando a lista de d nohs adjacentes de cada noh
	for(auto noAtual = listaNosGrafo.begin(); noAtual != listaNosGrafo.end(); noAtual++) {
		
		// a lista dAdjacentes guarda os nohs em ordem crescente, de acordo com o peso da arestas entre os nohs
		
		int tamanho = (*noAtual)->getNosAdjacentes().size();
		No **vet = new No*[tamanho]; // vetor auxilar 

		list<No*> adjacentes = (*noAtual)->getNosAdjacentes(); 
		int i = 0;
		for(auto noAux = adjacentes.begin(); noAux != adjacentes.end(); noAux++) {
			vet[i] = (*noAux); 
			i++;
		}

		// ordena o vetor
		quickSort(vet, (*noAtual), 0, tamanho);
		
		int peso = 0; 
		list<No*> dAdjacentes;

		// calcula o peso e setta a lista dNosAdjacentes de cada noh
		for(int j = 0; j < d; j++) {
			peso += (*noAtual)->getAresta(vet[j]->getId())->getPeso();
			dAdjacentes.push_back(vet[j]);
		}

		pesos[(*noAtual)] = peso;
		dNosAdjacentes[(*noAtual)] = dAdjacentes;
	}

	// guarda os nohs visitados, inicialmente vazia
	list<No*> nosVisitados;

	// guarda os nohs nao visitados. inicialemente recebe um copia da lista de nohs do grafo
	list<No*> nosNaoVisitados = listaNosGrafo;
	int flag1 = 0, flag2 = 0;
	int count = 0;
	
	// guarda os nohs que estao em estado de espera
	list<No*> nosEstadoEspera;

	// guarda os nohs que ficaram com grau maximo, grau igual a d
	list<No*> nosGrauMaximo;

	// setta o grau de todos os nohs pra 0
	for(auto noAtual = listaNosGrafo.begin(); noAtual != listaNosGrafo.end(); noAtual++)
		(*noAtual)->setGrau(0);

	No *noMenorPeso = nosNaoVisitados.front();
	int menorPeso = INFINITO;
	
	// procura pelo primeiro noh com menor peso
	for(auto noAtual = nosNaoVisitados.begin(); noAtual != nosNaoVisitados.end(); noAtual++) {
		if(pesos[(*noAtual)] < menorPeso) {
			menorPeso = pesos[(*noAtual)];
			noMenorPeso = (*noAtual);
		}
	}

	// insere o noMenorPeso na lista nosVisitados, remove da nosNaovisitados
	nosVisitados.push_back(noMenorPeso);
	nosNaoVisitados.remove(noMenorPeso);

	while(!nosNaoVisitados.empty()) {
		noMenorPeso = nosNaoVisitados.front();
		menorPeso = INFINITO;
		
		// procura pelo noh com menor peso
		for(auto noAtual = nosNaoVisitados.begin(); noAtual != nosNaoVisitados.end(); noAtual++) {
			if(pesos[(*noAtual)] < menorPeso) {
				menorPeso = pesos[(*noAtual)];
				noMenorPeso = (*noAtual);
			}
		}
		
		flag1 = 0;
		
		// guarda o primeiro noh na lista nosVisitados que faz parte dos dNosAdjacentes do noMenorPeso
		list<No*>::iterator noAux;
		
		// percorre a lista dNosAdjacentes para encontrar o primeiro noh na lista nosVisitados
		for(noAux = dNosAdjacentes[noMenorPeso].begin(); noAux != dNosAdjacentes[noMenorPeso].end(); noAux++) {
			if(contem(nosVisitados, (*noAux))) {
				flag1 = 1;
				break;
			}
		}

		// se encontrou
		if(flag1 == 1) {
			
			// insere o noMenorPeso na lista nosVisitados, remove da nosNaovisitados
			nosVisitados.push_back(noMenorPeso);
			nosNaoVisitados.remove(noMenorPeso);

			// insere o noMenorPeso na arvore
			if(arvore->getNo(noMenorPeso->getId()) == nullptr)
				arvore->inserirNo(noMenorPeso->getId());

			// insere o noAux na arvore
			if(arvore->getNo((*noAux)->getId()) == nullptr)
				arvore->inserirNo((*noAux)->getId());

			// insere um aresta entre eles, com o peso da aresta entre ele no grafo
			arvore->inserirAresta(noMenorPeso->getId(), (*noAux)->getId(), noMenorPeso->getAresta((*noAux)->getId())->getPeso());

			// aumenta o grau dos dois nohs
			noMenorPeso->setGrau(noMenorPeso->getGrau()+1);
			(*noAux)->setGrau((*noAux)->getGrau()+1);

			// se o noAux ficou com grau maximo 
			// o peso e a lista dNosAdjacentes de alguns nohs sao atualizados
			if((*noAux)->getGrau() == d) {
				
				// insere o noAux na lista nosGrauMaximo
				nosGrauMaximo.push_back((*noAux));
				
				// precorre a lista nosNaoVisitados
				for(auto noAtual = nosNaoVisitados.begin(); noAtual != nosNaoVisitados.end(); noAtual++) {
					
					// se o noAux esta na lista dNosAdjacentes do noAtual, atualiza o peso e a lista
					if(contem(dNosAdjacentes[*noAtual], (*noAux))) {
						int tamanho = (*noAtual)->getNosAdjacentes().size() - nosGrauMaximo.size();
						No **vetor = new No*[tamanho];

						list<No*> adjacentes = (*noAtual)->getNosAdjacentes();
						
						// remove os nos com grau maximo da lista de adjacentes do noAtual
						for(auto aux = nosGrauMaximo.begin(); aux != nosGrauMaximo.end(); aux++)
							adjacentes.remove(*aux);

						int i = 0;
						for(auto aux = adjacentes.begin(); aux != adjacentes.end(); aux++) {
							vetor[i] = (*aux); 
							i++;
						}

						// ordena o vetor
						quickSort(vetor, (*noAtual), 0, tamanho);

						int peso = 0;
						list<No*> dAdjacentes;
						for(int j = 0; j < d; j++) {
							peso += (*noAtual)->getAresta(vetor[j]->getId())->getPeso();
							dAdjacentes.push_back(vetor[j]);
						}

						// o peso e a lista dNosAdjacentes sao atualizados
						pesos[(*noAtual)] = peso;
						dNosAdjacentes[(*noAtual)] = dAdjacentes;
					}
				}
			}

			if(flag2 == 1) {
				// faz a uniao da lista nosNaoVisitado com nosEstadoEspera
				for(auto noAtual = nosEstadoEspera.begin(); noAtual != nosEstadoEspera.end(); noAtual++)
					if(!contem(nosNaoVisitados, (*noAtual)))
						nosNaoVisitados.push_back((*noAtual));

				// limpa a lista nosEstadoEspera	
				nosEstadoEspera.clear();
				
				count = 0;
				flag2 = 0;
			}
		}

		else {
			
			// insere o noMenorPeso na lista nosVisitados, remove da nosNaovisitados
			nosNaoVisitados.remove(noMenorPeso);
			nosEstadoEspera.push_back(noMenorPeso);
			
			flag2 = 1;
			count++;
		}
	}

	// se count maior que 0, entao existem nohs que ainda estao com grau 0
	// a funcao insereRestanteNos eh chamada para inserir os nohs na arvore
	if(count > 0)
		insereRestanteNos(nosEstadoEspera, nosVisitados, arvore, d);
}