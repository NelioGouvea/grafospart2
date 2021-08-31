#ifndef PINKFLOYD_H_INCLUDED
#define PINKFLOYD_H_INCLUDED

#include "../Grafo/Grafo.h"
#include "../Grafo/No.h"
#include "../Grafo/Aresta.h"

#include <iostream>

/* FUNÇÃO QUE RECEBE DOIS ID'S DE VÉRTICES DO GRAFO E RETORNA O CAMINHO 
MINIMO ENTRE ESTES DOIS VÉRTICES USANDO O ALGORITMO DE FLOYD*/

//função auxiliar que retorna o menor numero
float menorCaminho(float a, float b)
{
    if (a <= b)
        return a;
    else
        return b;
}

using namespace std;

//Função Floyd que retorna o caminho mínimo entre estes dois vértices
float custoPinkFloyd(Grafo *grafo, int id1, int id2)
{
    //verifica se os id's passados estão no grafo
    if (grafo->procuraNo(id1) && grafo->procuraNo(id2))
    {
        int tamanho = grafo->getOrdem();
        //matriz para armazenar custo dos caminhos
        float matrizC[tamanho][tamanho];

        //caminho mais curto de i a j sem nós intermediarios (A°)
        for (int i = 1; i <= tamanho; i++)
            for (int j = 1; j <= tamanho; j++)
            {
                //verifica se não esta no mesmo vertice, quando não ha aresta entre dois id's verifica peso retorna 99999 (infinito)
                if (i != j)
                    matrizC[i-1][j-1] = grafo->verificaPesoAresta(i, j);
                else
                    matrizC[i-1][j-1] = (float)0;
            }
        //caminho mais curto de i a j com k nós intermediarios (A^k)
        for (int k = 0; k < tamanho; k++)
            for (int i = 0; i < tamanho; i++)
                for (int j = 0; j < tamanho; j++)
                {
                    //chama a função auxiliar e atribui o menor caminho de um vertice ao outro
                    matrizC[i][j] = menorCaminho(matrizC[i][j], (matrizC[i][k] + matrizC[k][j]));
                }
        return matrizC[id1-1][id2-1];
    }
    else
    {

        return -99999;
    }
}

#endif // PINKFLOYD_H_INCLUDED