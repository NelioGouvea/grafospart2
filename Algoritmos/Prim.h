#ifndef PRIM_H_INCLUDED
#define PRIM_H_INCLUDED

#include <list>
#include <iostream>
#include <stdlib.h>
#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"

//Verifica e retorna a aresta de menor peso do grafo
Aresta *verificaArestaMenorPeso(Grafo *grafo)
{
    Aresta *resultado;
    No *no;
    float menorPeso = -1;
    float pesoAresta = -1;
    int fonteId = 1;
    int alvoId = 1;
    int fonteMin = 1;
    int alvoMin = 1;

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

//Confere dentro dos vertices do grafo, qual vertice da aresta de menor peso eles estao mais perto
//e retorna um vetor de inteiros, em que cada posição i refere ao vertice i+1 do grafo, e seu conteudo
//informa o id do vertice mais proximo
int *verificaVizinhoMaisProx(Grafo *grafo, Grafo *arvore, int size)
{

    int *prox = new int[size];
    int fonteId = 1;

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
                prox[fonteId-1] = u;
            }
            else
            {
                prox[fonteId-1] = v;
            }
    
    }

    return prox;
}

//Retorna dentre os vertices proximos o indice do vertice com menor custo
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

//Executa o algoritmo de Prim
Grafo *Prim(Grafo *grafo, list<int> subconjunto)
{
    Grafo *arvore = new Grafo(0, false, false, false);

    //Verifica e retorna aresta de menor peso do grafo e insere a aresta na arvore
    Aresta *aresta = verificaArestaMenorPeso(grafo);

    arvore->inserirNo(aresta->getFonteId(), 0);
    arvore->inserirNo(aresta->getAlvoId(), 0);
    arvore->inserirAresta(aresta->getFonteId(), aresta->getAlvoId(), 0);

    //inicializa o vetor que relaciona os vertices com o vizinho mais proximo
    int size = grafo->getOrdem();
    int *prox = verificaVizinhoMaisProx(grafo, arvore, size);

    //atualiza os vertices já visitados
    prox[aresta->getFonteId() - 1] = -1;
    prox[aresta->getAlvoId() - 1] = -1;

    //inicializa o vetor que relaciona os vertices com o peso do vizinho mais proximo
    float *aux = new float[size];

    for (int i = 0; i < size; i++)
    {
        aux[i] = grafo->verificaPesoAresta((i + 1), prox[i]);
    }

    //atualiza os vertices ja visitados
    aux[aresta->getFonteId() - 1] = -1;
    aux[aresta->getAlvoId() - 1] = -1;

    int cont = 0;
    int j = 0;
    int n = grafo->getOrdem();

    while (cont < (n - 2))
    {
        //procura o proximo vertice a ser inserido, procurando pelos vertices nao visitados e com custo minimo
        j = retornaJ(prox, aux, size);
        if (prox[j] != -1)
        {
            arvore->inserirNo((j + 1), 0);
            arvore->inserirAresta((j + 1), prox[j], 0);
            prox[j] = -1;
        }

        //atualiza a lista de proximos com base na proximidade deles com J,
        for (int i = 0; i < n; i++)
        {

            if (prox[i] != -1 && (aux[i] > grafo->verificaPesoAresta((i + 1), (j + 1))))
            {

                prox[i] = j + 1;
                aux[i] = grafo->verificaPesoAresta((i + 1), (j + 1));
            }
        }
        cont++;
    }

    return arvore;
}

#endif