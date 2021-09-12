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

float escolheAlfa(float *prob, int tam, int *alphaIndex)
{

    //copia a lista de probabilidades e ordena de forma crescente
    float *cop = prob;
    quicksort(cop, 0, tam);

    //escolhe randomicamente entre a metade das probabilidade de maior valor
    int x = (int)(tam / 2);
    //float *maxProb = new float[x];
    float *maxProb = (float *)malloc(sizeof(x));
    for (size_t i = 0; i < x; i++)
    {
        maxProb[i] = cop[i];
    }

    *alphaIndex = ((int)randomRange(0, x - 1));
    return maxProb[*alphaIndex];
}

float calculaQ(float melhorPeso, float media)
{
    //calcula o valor q para o calculo das probabilidades
    return (pow((melhorPeso / media), 10));
}

void atualizaProbabilidade(float *probAlfas, float *q, int *pesoMed, int *melhorPeso, int tam)
{

    //atualiza os valores qi
    int somaQ = 0;
    for (size_t i = 0; i < tam; i++)
    {
        q[i] = calculaQ(*melhorPeso, pesoMed[i]);
        somaQ += q[i];
    }

    //atualiza as probabilidades com base em q
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

Grafo *gulosoRandomizadoReativo(Grafo *grafo, int d, float *alfas, int tamAlfa, int numIteracoes, int *peso, double *tempo, int bloco)
{
    clock_t t = clock();

    //cria- se uma lista de arestas e uma lista auxiliar
    list<Aresta *> todasArestas;
    list<Aresta *> AuxtodasArestas;

    //ordena-se a lista pelo peso das arestas (da menor para maior)
    ordenaVetorArestas(grafo, &todasArestas);

    //cria-se as arvores soluções
    Grafo *s;
    Grafo *solBest = nullptr;

    //cria-se variaveis de apoio: Contador (i)(j), valor randomico (k), index alpha
    int i = 0, k, j, alphaIndex;
    float alfa;
    int pesoS;

    // float *probAlfas = new float[tamAlfa];
    // float *q = new float[tamAlfa];
    // int *pesoMed = new int[tamAlfa];

    float *probAlfas = (float *)malloc(sizeof(tamAlfa));
    float *q = (float *)malloc(sizeof(tamAlfa));
    int *pesoMed = (int *)malloc(sizeof(tamAlfa));

    No *noFonte;
    No *noAlvo;
    Grafo *fecho;
    Aresta *random;
    *peso = 1;

    //inicializa os vetores criados com os valores iniciais
    inicializaVetores(probAlfas, pesoMed, *peso, tamAlfa);
    //executa ate alcançar o numero de iterações
    while (i < numIteracoes)
    {

        if (i % bloco == 0)
        {
            //atualiza problabilidade a cada BLOCO numero de iteração
            atualizaProbabilidade(probAlfas, q, pesoMed, peso, tamAlfa);
        }
        i++;

        //instancia a arvore a ser gerada 
        s = new Grafo(grafo->getOrdem(), false, true, false);
        s->geraVetNo();

        int cont = 0;
        //escolhe o alfa a ser utilizado
        alfa = escolheAlfa(probAlfas, tamAlfa, &alphaIndex);
        int cabaco = 0;
        AuxtodasArestas = todasArestas;
        do
        {

            srand(time(NULL));
            //gera o numero randomico com base em alfa
            k = randomRange(0, alfa * (AuxtodasArestas.size() - 1));

            j = 0;
            //procura a aresta correspondente
            for (auto aresta = AuxtodasArestas.begin(); aresta != AuxtodasArestas.end(); aresta++)
            {
                if (j == k)
                {
                    random = (*aresta);
                }
                j++;
            }

            // seleciona os par de nós da aresta escolhida da sua extremidade
            noFonte = s->getNoVet(random->getFonteId());
            noAlvo = s->getNoVet(random->getAlvoId());

            //utiliza o fecho para verificar ciclos
            s->criaListaAdjacencia();
            fecho = fechoDireto(s, noFonte->getId());
 
            if ((fecho->getListaNos().empty() || !fecho->procuraNo(noAlvo->getId())) && noAlvo->getGrau() < d && noFonte->getGrau() < d)
            {

                s->inserirAresta(noFonte->getId(), noAlvo->getId(), random->getPeso());
                cont++;
            }

            //remove aresta da lista de arestas
            AuxtodasArestas.remove(random);
           
            delete fecho;
        } while (cont < grafo->getOrdem() - 1);

        s->criaListaAdjacencia();


        //verifica se é a primeira iteração, caso não calcula os pesos e compara das arvores s e solbest
        if (solBest == nullptr)
        {
            solBest = s;
        }
        else
        {

            int pesoSolBest = calculaPeso(solBest);
            pesoS = calculaPeso(s);
            if (pesoS < pesoSolBest)
            {
                solBest = s;
            }
        }

        //atualiza as medidas medias para atualização das probabilidades
        atualizaMedias(pesoMed, tamAlfa, alphaIndex, pesoS, bloco);

        delete s;
    }

    *peso = calculaPeso(solBest);

    t = clock() - t;
    *tempo = t / (double)CLOCKS_PER_SEC;
    cout << "SAI" << endl;
    return solBest;
}

#endif // GULOSORANDOMIZADOREATIVO_H_INCLUDED