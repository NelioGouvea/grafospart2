#ifndef DCMST_H_INCLUDED
#define DCMST_H_INCLUDED

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"

#include <list>
#include <map>

#include <iostream>

using namespace std;

void execute_falha_segmentacao()
{
	int *ponteiroperigoso = (int*) 100;
	int test = *ponteiroperigoso;
}

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

float dcMST(Grafo *grafo, int d) {
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

	list<No*> Vx;

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

	int cont = 0;
	while(!U.empty()) {
		No *v2;
		menor = 999999;
		for(auto v = U.begin(); v != U.end(); v++) {
			cout << U.size() << endl;
			//cout << "peso do no "<< (*v)->getId() << " = "  << Wd[*v] << " ";
			if(Wd[(*v)] <= menor) {
				menor = Wd[(*v)];
				v2 = (*v);
			}	
		}
		cout << "foi" << endl;
		flag1 = 0;

		list<No*>::iterator vx;
		for(vx = Vd[v2].begin(); vx != Vd[v2].end(); vx++) {
			if(mark[(*vx)] == 1) {
				flag1 = 1;
				cout << "aqui" << endl;
				break;
			}
		}

		cout << "fos" << endl;
		if(flag1 == 1) {
			cout << "aqui" << endl;
			mark[v2] = 1;
			S.push_back(v2);
			U.remove(v2);

			if(T->getNo(v2->getId()) == nullptr)
				T->inserirNo(v2->getId());
			

			if(T->getNo((*vx)->getId()) == nullptr)
				T->inserirNo((*vx)->getId());

			T->inserirAresta(v2->getId(), (*vx)->getId(), v2->getAresta((*vx)->getId())->getPeso());
			//cout << v2->getId() << "--" << (*vx)->getId() << "--" << v2->getAresta((*vx)->getId())->getPeso() << endl;
			//cout << "peso = " << Wd[v2] << endl << endl; 

			v2->setGrau(v2->getGrau()+1);
			(*vx)->setGrau((*vx)->getGrau()+1);

			if((*vx)->getGrau() == d) {
				Vx.push_back(*vx);
				
				cout << "morri" << endl;
				for(auto v = U.begin(); v != U.end(); v++) {
					if(contem(Vd[*v], (*vx))) {
						int tam = (*v)->getNosAdjacentes().size() - Vx.size();
						No **vet = new No*[tam];

						list<No*> adj = (*v)->getNosAdjacentes();
						
						for(auto aux = Vx.begin(); aux != Vx.end(); aux++)
							adj.remove(*aux);

						int i = 0;
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

						cout << endl << "Atualizou o: " << (*v)->getId() << endl << endl;
					}
				}
			}

			if(flag2 == 1) {
				cout << "======= AGORA =======" << endl;
				cout << "cont 1 = " << cont << endl;
				//U.merge(Ws);
				//Ws.clear();
				count = 0;
				flag2 = 0;
			}
		}

		else {
			cout << "======= AGORA =======" << endl;
			cout << "cont = " << cont << endl;
			U.remove(v2);
			Ws.push_back(v2);
			flag2 = 1;
			count++;
		}
		cont++;
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

	cout << endl;
	T->criaListaAdjacencia();
	T->imprimeListaAdjacencia();
	auto lista = T->getListaNos();
	for(auto a = lista.begin(); a != lista.end(); a++) {
		cout << (*a)->getId() << " ";

		list<No*> listaAdj = (*a)->getNosAdjacentes();
		for(auto b = listaAdj.begin(); b != listaAdj.end(); b++)
			cout << (*b)->getId() << " - ";
		cout << endl;
	}
	cout << endl;

	// percorre todos os nos de T || primeio estagio da fase 2
	for(auto noAtual = lista.begin(); noAtual != lista.end(); noAtual++) {
		// verifica os que tem grau igual a d
		if((*noAtual)->getGrau() == d) {
			
			cout << "No = " << (*noAtual)->getId() << endl;
			
			// guarda a lista de arestas do noAtual
			list<Aresta*> listaAresta = (*noAtual)->getListaAresta();
			
			for(auto arestaAtual = listaAresta.begin(); arestaAtual != listaAresta.end(); arestaAtual++) {

				// no alvo da arestaAtual
				No* noAlvo = T->getNo((*arestaAtual)->getAlvoId());

				// guarda a lista de arestas do noAlvo, aresta que estao no grafo
				list<Aresta*> listaArestaAux = grafo->getNo(noAlvo->getId())->getListaAresta();
				
				for(auto arestaAux = listaArestaAux.begin(); arestaAux != listaArestaAux.end(); arestaAux++) {
					
					// verifica se o peso da arestaAux eh menor ou igual do que a atestaAtual
					if((*arestaAux)->getPeso() <= (*arestaAtual)->getPeso()) {
						
						// no alvo do noAlvo
						No* noAux = T->getNo((*arestaAux)->getAlvoId());

						// verfica se o noAux nao eh o noAtual || se o grau do noAux eh menor que d || se o noAlvo e o noAux ja tem um aresta em T
						if(noAux->getId() != (*noAtual)->getId() && noAux->getGrau() < d && noAlvo->getAresta(noAux->getId()) == nullptr) {

							// remove aresta entre o noAtual e o noAlvo
							(*noAtual)->removerAresta(noAlvo->getId());
							(*noAtual)->setGrau((*noAtual)->getGrau() - 1);

							noAlvo->removerAresta((*noAtual)->getId());
							noAlvo->setGrau(noAlvo->getGrau() - 1);

							// adciona aresta entre o noAlvo e noAux
							T->inserirAresta(noAlvo->getId(), noAux->getId(), (*arestaAux)->getPeso());
							T->criaListaAdjacencia();
						}
					}
				}
			}
		}
	}

	T->criaListaAdjacencia();
	lista = T->getListaNos();
	
	for(auto a = lista.begin(); a != lista.end(); a++) {
		cout << (*a)->getId() << " || ";

		list<No*> listaAdj = (*a)->getNosAdjacentes();
		for(auto b = listaAdj.begin(); b != listaAdj.end(); b++)
			cout << (*b)->getId() << " - ";
		cout << endl;
	}
	cout << endl;

	// percorre todos os nos de T || segundo estagio da fase 2
	for(auto noAtual = lista.begin(); noAtual != lista.end(); noAtual++) {
		
		// verifica se o noAtual tem grau menor que d
		if((*noAtual)->getGrau() < d) {

			// guarda a lista de aresta de arestas do noAtual
			list<Aresta*> listaAresta = (*noAtual)->getListaAresta();
			
			for(auto arestaAtual = listaAresta.begin(); arestaAtual != listaAresta.end(); arestaAtual++) {
				
				// no alvo da arestaAtual
				No *noAlvo = T->getNo((*arestaAtual)->getAlvoId());
				
				// guarda a lista de arestas do noAlvo, aresta que estao no grafo
				list<Aresta*> listaArestaAux = grafo->getNo((*noAtual)->getId())->getListaAresta();

				for(auto arestaAux = listaArestaAux.begin(); arestaAux != listaArestaAux.end(); arestaAux++) {
					
					// verfica se o peso da arestaAux eh menor do que a arestaAtual
					if((*arestaAux)->getPeso() < (*arestaAtual)->getPeso()) {
						
						// guarda o no alvo do noAux
						No *noAux = T->getNo((*arestaAux)->getAlvoId());
						
						// verifica se a aresta ja esta em T || se o grau do noAux nao eh 1, se for ele nao tera mais arestas || se o grau do noAux nao eh d
						if((*noAtual)->getAresta(noAux->getId()) == nullptr && noAux->getGrau() > 1 && noAux->getGrau() < d) {
							
							// remove a aresta entre o noAtual e o noAlvo
							(*noAtual)->removerAresta(noAlvo->getId());
							(*noAtual)->setGrau((*noAtual)->getGrau() - 1);

							noAlvo->removerAresta((*noAtual)->getId());
							noAlvo->setGrau(noAlvo->getGrau() - 1);

							// insere a aresta entre o noAtual e o noAux
							T->inserirAresta((*noAtual)->getId(), noAux->getId(), (*arestaAux)->getPeso());
							T->criaListaAdjacencia();
						}				
					}
				}
			}
		}
	}

	T->criaListaAdjacencia();
	lista = T->getListaNos();
	
	for(auto a = lista.begin(); a != lista.end(); a++) {
		//cout << (*a)->getId() << " || ";

		list<No*> listaAdj = (*a)->getNosAdjacentes();
		for(auto b = listaAdj.begin(); b != listaAdj.end(); b++)
			//cout << (*b)->getId() << " - ";
		cout << endl;
	}
	cout << endl;

	int total = 0;
	for(auto a = lista.begin(); a != lista.end(); a++) {
		//cout << (*a)->getId() << " || ";

		list<No*> listaAdj = (*a)->getNosAdjacentes();
		for(auto b = listaAdj.begin(); b != listaAdj.end(); b++) {
			//cout << (*b)->getId() << " - ";
			total += (*a)->getAresta((*b)->getId())->getPeso();
		}


		cout << endl;
	}
	cout << endl;
	cout << "Total = " << total/2 << endl;
	execute_falha_segmentacao();
	return total/2;
}

#endif // DCMST_H_INCLUDED