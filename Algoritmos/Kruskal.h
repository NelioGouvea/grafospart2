#ifndef KRUSKAL_H_INCLUDED
#define KRUSKAL_H_INCLUDED

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"
#include "../Algoritmos/FechoDireto.h"
#include <list>
#include <iostream>

/* FUNÇÃO QUE RECEBE UM SUBCONJUNTO X DE VÉRTICES DO GRAFO E RETORNA UMA
ARVORE GERADORA MÍNIMA SOBRE O SUBGRAFO VERTICE-INDUZIDO POR X  USANDO O ALGORITMO DE KRUSKAL*/

using namespace std;

// funcao auxiliar que verifica se um no é adjacente ao outro
int contem(list<No *> listaNo, int id)
{
    list<No *>::iterator noAtual;
    for (noAtual = listaNo.begin(); noAtual != listaNo.end(); noAtual++)
        if ((*noAtual)->getId() == id)
        {
            return 1;
        }
    return 0;
}

// funcao para comparar e ordenar as arestas por peso
void quickSort(Aresta **vet, int p, int q)
{

    float pivo = vet[(int)((p + q) / 2)]->getPeso();
    int i = p;
    int j = q - 1;

    while (i <= j)
    {
        while (vet[i]->getPeso() < pivo)
        {
            i++;
        }

        while (vet[j]->getPeso() > pivo)
        {
            j--;
        }

        if (i <= j)
        {
            Aresta *aresta;
            aresta = vet[i];
            vet[i] = vet[j];
            vet[j] = aresta;
            i++;
            j--;
        }
    }

    if (p < j)
        quickSort(vet, p, j + 1);

    if (i < q)
    {
        quickSort(vet, i, q);
    }
}

Grafo *kruskal(Grafo *grafo, int *subConjuntoX, int tam)
{
    //*Cria listaAressta para receber e ordenar todas arestas do subconjuntoX
    list<Aresta *> listaAresta;

    //*Insere as arestas do subconjuntoX na listaAresta
    for (int i = 0; i < tam; i++)
    {

        No *no = grafo->getNo(subConjuntoX[i]);
        listaAresta.merge(no->getListaAresta());
    }

    //*Cria um vetor para receber as arestas e ordena-las
    Aresta **vec = new Aresta *[listaAresta.size()];

    list<Aresta *>::iterator arestaIterator;
    int cont = 0;

    //*Insere no vetor todas as arestas da listaAresta
    for (arestaIterator = listaAresta.begin(); arestaIterator != listaAresta.end(); arestaIterator++)
    {
        vec[cont] = (*arestaIterator);
        cont++;
    }

    //*Ordena em ordem crescente de peso todas as arestas
    quickSort(vec, 0, cont);

    //*Retorno as arestas ordenadas para a listaAresta
    for (int i = 0; i < cont; i++)
    {
        listaAresta.pop_front();
        listaAresta.push_back(vec[i]);
    }

    //*Sub arvores de vertices isolados
    Grafo *kruskal = new Grafo(0, false, true, false);

    //cria a floresta de nós isolados
    for (int i = 0; i < tam; i++)
    {
        kruskal->inserirNo(subConjuntoX[i], 0);
    }

    //Conta numero de aresta inseridas no kruskal e serve como flag do nosso while
    cont = 0;

    while ((cont < tam - 1) && listaAresta.size() != 0)
    {

        int u = listaAresta.front()->getFonteId();
        int v = listaAresta.front()->getAlvoId();


        No *no = kruskal->getNo(v);

        // verifica se estão na mesma sub arvore atraves do fecho das subarvores
        kruskal->criaListaAdjacencia();
        Grafo *novo = fechoDireto(kruskal, u);
    
        if ((novo->getListaNos().empty() || !novo->procuraNo(v)) && (u != v))
        {   
            //Unimos as subarvore U e V 
            kruskal->inserirAresta(u, v, listaAresta.front()->getPeso());
            cont++;
        }

        // remove a aresta
        listaAresta.pop_front();
    }

    return kruskal;
}

#endif // PINKFLOYD_H_INCLUDED