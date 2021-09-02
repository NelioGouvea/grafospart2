#ifndef DCMST_H_INCLUDED
#define DCMST_H_INCLUDED

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"

#include <list>
#include <map>

#include <iostream>

using namespace std;

void quickSort(No **vet, No *ref, int p, int q)
{

    float pivo = ref->getAresta(vet[(int)((p + q) / 2)]->getId())->getPeso();
    int i = p;
    int j = q - 1;

    while (i <= j) {
		while (ref->getAresta(vet[i]->getId())->getPeso() < pivo)
            i++;

        while (ref->getAresta(vet[j]->getId())->getPeso() > pivo)
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
        quickSort(vet, ref, p, j + 1);

    if (i < q)
    {
        quickSort(vet,ref, i, q);
    }
}

bool contem(list<No*> listaNo, No *noAlvo) {
    list<No*>::iterator noAtual;
    for(noAtual = listaNo.begin(); noAtual != listaNo.end(); noAtual++)
        if((*noAtual) == noAlvo)
            return true;
    return false;                   
}

void dcMST(Grafo *grafo, int d) {
	// calcula Wd, Vd
	map<No*, int> Wd;
	map<No*, list<No*>> Vd;

	list<No*> V = grafo->getListaNos();
	for(auto v = V.begin(); v != V.end(); v++) {
		int tam = (*v)->getNosAdjacentes().size();
		No **vet = new No*[tam];

		list<No*> adj = (*v)->getNosAdjacentes(); int i = 0;
		for(auto aux = adj.begin(); aux != adj.end(); aux++) {
			vet[i] = (*aux); 
			i++;
		}

		quickSort(vet, (*v), 0, tam);

		int peso = 0;
		list<No*> dAdj;
		for(int j = 0; j < d; j++) {
			peso += (*v)->getAresta(vet[j]->getId())->getPeso();
			dAdj.push_back(vet[j]);
		}

		Wd[(*v)] = peso;
		Vd[(*v)] = dAdj;
	}

	// inicializa valores
	Grafo *T = new Grafo(0, false, true, true);
	list<No*> S;
	list<No*> U = V;
	int flag1 = 0, flag2 = 0;
	int count = 0;
	list<No*> Ws;

	map<No*, int> mark;
	for(auto i = V.begin(); i != V.end(); i++) { 
		mark[(*i)] = 0;
		(*i)->setGrau(0);
	}

	No *v1;
	int menor = 999999;
	for(auto v = U.begin(); v != U.end(); v++) {
		if(Wd[(*v)] < menor) {
			menor = Wd[(*v)];
			v1 = (*v);
		}
			
	}

	mark[v1] = 1;
	S.push_back(v1);
	U.remove(v1);

	while(!U.empty()) {
		No *v2;
		menor = 999999;
		for(auto v = U.begin(); v != U.end(); v++) {
			if(Wd[(*v)] < menor) {
				menor = Wd[(*v)];
				v2 = (*v);
			}	
		}

		list<No*>::iterator vx;
		for(vx = Vd[v2].begin(); vx != Vd[v2].end(); vx++) {
			if(mark[(*vx)] == 1) {
				flag1 = 1;
				break;
			}
		}

		if(flag1 == 1) {
			mark[v2] = 1;
			S.push_back(v2);
			U.remove(v2);

			if(T->getNo(v2->getId()) == nullptr)
				T->inserirNo(v2->getId());
			
			if(T->getNo((*vx)->getId()) == nullptr)
				T->inserirNo((*vx)->getId());

			T->inserirAresta(v2->getId(), (*vx)->getId(), v2->getAresta((*vx)->getId())->getPeso());

			v2->setGrau(v2->getGrau()+1);
			(*vx)->setGrau((*vx)->getGrau()+1);

			cout << "v2 = " << v2->getId() << " grau " << v2->getGrau() << endl;

			if(flag2 == 1) {
				U.merge(Ws);
				Ws.clear();
				count = 0;
				flag2 = 0;
			}
		}

		else {
			U.remove(v2);
			Ws.push_back(v2);
			flag2 = 1;
			count++;
		}
	}

	if(count > 0) {
		for(auto i = Ws.begin(); i != Ws.end(); i++) {
			No *minimo;
			int menor = 99999;
			for(auto j = S.begin(); j != S.end(); j++) {
				if((*i)->getAresta((*j)->getId()) != nullptr) {
					if((*i)->getAresta((*j)->getId())->getPeso() < menor) {
						menor = (*i)->getAresta((*j)->getId())->getPeso();
						minimo = (*j);
					}
				}
			}

			if(T->getNo((*i)->getId()) == nullptr)
				T->inserirNo((*i)->getId());
			
			if(T->getNo(minimo->getId()) == nullptr)
				T->inserirNo(minimo->getId());

			T->inserirAresta((*i)->getId(), minimo->getId(), (*i)->getAresta(minimo->getId())->getPeso());
		}
	}

	T->criaListaAdjacencia();
	T->imprimeListaAdjacencia();
	auto lista = T->getListaNos();
	for(auto a = lista.begin(); a != lista.end(); a++)
		cout << (*a)->getId() << endl;
}

#endif // DCMST_H_INCLUDED