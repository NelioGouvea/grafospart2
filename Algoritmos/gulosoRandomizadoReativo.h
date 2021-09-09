#ifndef GULOSORANDOMIZADOREATIVO_H_INCLUDED
#define GULOSORANDOMIZADOREATIVO_H_INCLUDED

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"
#include "../Algoritmos/FechoDireto.h"
#include "../Algoritmos/FechoIndireto.h"

#include <list>
#include <map>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#include "../Algoritmos/dcMST.h"
#include "../Algoritmos/guloso.h"

#define INFINITO 9999999

using namespace std;

void quicksort(float *vet, int began, int end)
{
    int i, j;
    float pivo, aux;
    i = began;
    j = end - 1;
    pivo = vet[(began + end) / 2];
    while (i <= j)
    {
        while (vet[i] < pivo && i < end)
        {
            i++;
        }
        while (vet[j] > pivo && j > began)
        {
            j--;
        }
        if (i <= j)
        {
            aux = vet[i];
            vet[i] = vet[j];
            vet[j] = aux;
            i++;
            j--;
        }
    }
    if (j > began)
        quicksort(vet, began, j + 1);
    if (i < end)
        quicksort(vet, i, end);
}

float escolheAlfa(float *prob, int tam)
{

    float *cop = prob;

    quicksort(cop, 0, tam);

    int x = (int)(tam / 5);
    float *maxProb = new float[x];

    for (size_t i = 0; i < x; i++)
    {
        maxProb[i] = cop[i];
    }

    return maxProb[((int)randomRange(0, x - 1))];
}

float calculaQ(float melhorPeso, float media)
{
    return (pow((melhorPeso / media), 10));
}

void atualizaProbabilidade(float *probAlfas, float *q, int *pesoMed, int *melhorPeso, int tam)
{
    int somaQ = 0;
    for (size_t i = 0; i < tam; i++)
    {
        q[i] = calculaQ(*melhorPeso, pesoMed[i]);
        somaQ += q[i];
    }

    for (size_t i = 0; i < tam; i++)
    {
        probAlfas[i] = q[i] / somaQ;
    }
}

void inicializaVetores(float *probAlfas, int *pesoMed, int peso, int tam)
{

    for (size_t i = 0; i < tam; i++)
    {
        probAlfas[i] = 1 / tam;
        pesoMed[i] = peso;
    }
}

void atualizaMedias(int *pesoMed, int tamAlfa, int alfaIndex, float pesoS, int bloco)
{
    pesoMed[alfaIndex] += (pesoS / bloco);
}

int procuraIndexAlfa(float *alfas, int tamAlfa, float alfa)
{
    for (size_t i = 0; i < tamAlfa; i++)
    {
        if (alfas[i] == alfa)
            return i;
    }

    return -1;
}

Grafo *gulosoRandomizadoReativo(Grafo *grafo, int d, float *alfas, int tamAlfa, int numIteracoes, int *peso, double *tempo, int bloco)
{
    clock_t t = clock();

    list<Aresta *> todasArestas;

    ordenaVetorArestas(grafo, &todasArestas);

    Grafo *s;
    Grafo *solBest = nullptr;

    int i = 0, k, alphaIndex;
    float alfa;
    int pesoS;
    float *probAlfas = new float[tamAlfa];
    float *q = new float[tamAlfa];
    int *pesoMed = new int[tamAlfa];
    *peso = 1;

    inicializaVetores(probAlfas, pesoMed, *peso, tamAlfa);
    while (i < numIteracoes)
    {
        
        if (i % bloco == 0)
            atualizaProbabilidade(probAlfas, q, pesoMed, peso, tamAlfa);

        i++;
        s = new Grafo(grafo->getOrdem(), false, true, false);
        s->geraVetNo();
          
        int cont = 0;
        alfa = escolheAlfa(probAlfas, tamAlfa);
        do
        {
            srand(time(NULL));

            k = randomRange(0, alfa * (todasArestas.size() - 1));

            Aresta *random;
            int j = 0;
            for (auto aresta = todasArestas.begin(); aresta != todasArestas.end(); aresta++)
            {
                if (j == k)
                {
                    random = (*aresta);
                }
                j++;
            }

            No *noFonte = s->getNoVet(random->getFonteId());
            No *noAlvo = s->getNoVet(random->getAlvoId());

            s->criaListaAdjacencia();

            Grafo *fecho = fechoDireto(s, noFonte->getId());

            if ((fecho->getListaNos().empty() || !fecho->procuraNo(noAlvo->getId())) && noAlvo->getGrau() < d && noFonte->getGrau() < d)
            {

                s->inserirAresta(noFonte->getId(), noAlvo->getId(), random->getPeso());
                cont++;
            }

            todasArestas.remove(random);

        } while (cont < grafo->getOrdem() - 1);
      
        s->criaListaAdjacencia();

        if (solBest == nullptr)
            solBest = s;

        else
        {
            int pesoSolBest = calculaPeso(solBest);
            pesoS = calculaPeso(s);

            if (pesoS < pesoSolBest)
                solBest = s;
        }
          
        atualizaMedias(pesoMed, tamAlfa, alphaIndex, pesoS, bloco);
    }

    solBest->criaListaAdjacencia();

    *peso = calculaPeso(solBest);

    t = clock() - t;
    *tempo = t / (double)CLOCKS_PER_SEC;

    return solBest;
}

#endif // GULOSORANDOMIZADOREATIVO_H_INCLUDED