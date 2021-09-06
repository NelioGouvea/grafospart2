#ifndef GRAFO_H_INCLUDED
#define GRAFO_H_INCLUDED

#include <fstream>
#include <stack> // pilha para usar na DFS
#include "No.h"
#include "Aresta.h"
#include <list>
using namespace std;

class Grafo
{

    //Atributes
private:
    int ordem;
    int numeroArestas;
    bool direcionado;
    bool ponderadoAresta;
    bool ponderadoNo;
    list<No *> listaNo;
    list<list<No *>> listaAdjacencia;

public:
    //Constructor
    Grafo(int ordem, bool direcionado, bool ponderadoAresta, bool ponderadoNo);
    //Destructor
    ~Grafo();
    //Getters
    int getOrdem();
    int getNumeroArestas();
    bool getDirecionado();
    bool getPonderadoAresta();
    bool getPonderadoNo();
    No *getNo(int id);
    list<No *> getListaNos();
    list<list<No *>> getListaAdjacencia();
    Grafo *caminhamentoProfundidade(int id);
    void auxCaminhamentoProfundidade(No *filho, list<No *> *listaVisitados, Grafo *grafo);

    //Setters
    void inserirNo(int id, int peso);
    void inserirNo(int id);
    void inserirAresta(int fonteId, int alvoId, int peso);
    void inserirAresta(int fonteId, int alvoId);
    //Methods
    void removeNo(int id);
    bool procuraNo(int id);
    int verificaPesoAresta(int fonteId, int alvoId);

    void criaListaAdjacencia();
    void imprimeListaAdjacencia();
    bool verificaVisita(No *no, list<No *> listaVisitados);

private:
};

#endif // GRAFO_H_INCLUDED
