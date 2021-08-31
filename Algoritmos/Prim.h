#ifndef PRIM_H_INCLUDED
#define PRIM_H_INCLUDED


#include <list>
#include <iostream>
#include <stdlib.h>
#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"

bool verificaArestaConectado(Aresta *aresta, int idNo)
{

    if (aresta->getAlvoId() == idNo)
    {
        return true;
    }

    return false;
}

Aresta *verificaArestaMenorPeso(Grafo *grafo)
{
    Aresta *resultado;
    No *no;
    float menorPeso = -1;
    float pesoAresta = -1;
    int fonteId = 0;
    int alvoId = 0;
    int fonteMin = 0;
    int alvoMin = 0;

    list<list<No *>> listAdj = grafo->getListaAdjacencia();
    list<list<No *>>::iterator listNoIt;

    for (listNoIt = listAdj.begin(); listNoIt != listAdj.end(); listNoIt++)
    {

        list<No *>::iterator listAdjIt;
        for (listAdjIt = listNoIt->begin(); listAdjIt != listNoIt->end(); listAdjIt++)
        {

            alvoId = (*listAdjIt)->getId();

            if (grafo->verificaPesoAresta(fonteId, alvoId) != -1)
            {
                pesoAresta = grafo->verificaPesoAresta(fonteId, alvoId);
                if (menorPeso == -1 || pesoAresta < menorPeso)
                {
                    menorPeso = pesoAresta;
                    fonteMin = fonteId;
                    alvoMin = alvoId;
                }
            }
        }
        fonteId++;
    }

    no = grafo->getNo(fonteMin);
    resultado = no->getAresta(alvoMin);

    return resultado;
}

int *verificaVizinhoMaisProx(Grafo *grafo, Grafo *arvore, int size)
{

    int *prox = new int[size];

    float menorPeso = -1;
    float pesoAresta = -1;
    int fonteId = 0;
    int alvoId = 0;
    int fonteMin = 0;
    int alvoMin = 0;

    for (int i = 0; i < size; i++)
    {
        prox[i] = -1;
    }

    int u = (arvore->getListaNos().front())->getId();
    int v = (arvore->getListaNos().back())->getId();

    list<No *> listAdj = grafo->getListaNos();
    list<No *>::iterator listNoIt;
    for (listNoIt = listAdj.begin(); listNoIt != listAdj.end(); listNoIt++)
    {

        fonteId = (*listNoIt)->getId();

        if (grafo->verificaPesoAresta(fonteId, u) < grafo->verificaPesoAresta(fonteId, v))
        {
            //cout << "Teste 0 " << endl;
            pesoAresta = grafo->verificaPesoAresta(fonteId, u);
            prox[fonteId] = u;
        }
        else
        {
            pesoAresta = grafo->verificaPesoAresta(fonteId, v);
            prox[fonteId] = v;
        }
    }

    return prox;
}

int retornaJ(int *prox, float *aux, int size)
{

    float menorValor = -1;
    int menorIndex = -1;
    for (int i = 0; i < size; i++)
    {
        if (prox[i] != -1)
        {
            if (menorValor == -1 || aux[i] < menorValor)
            {

                menorValor = aux[i];
                menorIndex = i;
            }
        }
    }
    return menorIndex;
}
Grafo *Prim(Grafo *grafo, list<int> subconjunto)
{

    Grafo *arvore = new Grafo(0, false, false, false);

    Aresta *aresta = verificaArestaMenorPeso(grafo);

    arvore->inserirNo(aresta->getFonteId(), 0);
    arvore->inserirNo(aresta->getAlvoId(), 0);

    arvore->inserirAresta(aresta->getFonteId(), aresta->getAlvoId(), 0);

    int size = grafo->getListaAdjacencia().size();

    int *prox = verificaVizinhoMaisProx(grafo, arvore, size);

    prox[aresta->getFonteId()] = -1;
    prox[aresta->getAlvoId()] = -1;

    float *aux = new float[size];

    for (int i = 0; i < size; i++)
    {
        aux[i] = grafo->verificaPesoAresta(i, prox[i]);
    }

    aux[aresta->getFonteId()] = -1;
    aux[aresta->getAlvoId()] = -1;

    int cont = 0;
    int j = 0;
    int n = grafo->getOrdem();
    // for (int i = 0; i < size; i++)
    // {
    //     cout << "Prox[" << i << "] : " << prox[i] << endl;
    // }
    // for (int i = 0; i < size; i++)
    // {
    //     cout << "Aux[" << i << "] : " << aux[i] << endl;
    // }

    while (cont < (n - 2))
    {
        j = retornaJ(prox, aux, size);
        cout << "J : " << j << endl;
        if (prox[j] != -1)
        {
            arvore->inserirNo(j, 0);
            arvore->inserirAresta(j, prox[j], 0);
            prox[j] = -1;
        }
        for (int i = 0; i < n; i++)
        {

            if (prox[i] != -1 && (aux[i] > grafo->verificaPesoAresta(i, j)))
            {
           
                prox[i] = j;
                aux[i] = grafo->verificaPesoAresta(i, j);
               
            }
        }

        cont++;
    }



    for (int i = 0; i < size; i++)
    {
        cout << "Prox[" << i << "] : " << prox[i] << endl;
    }
    for (int i = 0; i < size; i++)
    {
        cout << "Aux[" << i << "] : " << aux[i] << endl;
    }

    return arvore;
}

#endif