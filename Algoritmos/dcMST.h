#ifndef DCMST_H_INCLUDED
#define DCMST_H_INCLUDED

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"
#include "Kruskal.h"

#include <list>
#include <iostream>

using namespace std;


Grafo* dcMST(Grafo *grafo, int k, int tam, int *subConjuntoX){
	//TODO Fase 1
	Grafo *agm = kruskal(grafo, subConjuntoX, tam);
	return agm;
	//TODO Fase 2
}


#endif // DCMST_H_INCLUDED