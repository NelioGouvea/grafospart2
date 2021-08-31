#include "Grafo.h"
#include "No.h"
#include "Aresta.h"
#include <list>
#include <iterator>
#include <algorithm> // função find
#include <stack>     // pilha para usar na DFS

#include <iostream>

using namespace std;

Grafo::Grafo(int ordem, bool direcionado, bool ponderadoAresta, bool ponderadoNo)
{

    this->ordem = 0;
    this->direcionado = direcionado;
    this->ponderadoAresta = ponderadoAresta;
    this->ponderadoNo = ponderadoNo;
    //this->primeiroNo = this->ultimoNo = nullptr;
    this->numeroArestas = 0;

    for (size_t i = 1; i <= ordem; i++)
    {
        inserirNo(i, 0);
    }
}

Grafo::~Grafo()
{

    // No *proximoNo = this->primeiroNo;

    // while (proximoNo != nullptr)
    // {

    //     proximoNo->removeAllEdges();
    //     No *auxNo = proximoNo->getNextNode();
    //     delete proximoNo;
    //     proximoNo = auxNo;
    // }

    this->listaNo.clear();
}

// OK
int Grafo::getOrdem()
{

    return this->ordem;
}

// OK
int Grafo::getNumeroArestas()
{

    return this->numeroArestas;
}

// OK
bool Grafo::getDirecionado()
{

    return this->direcionado;
}

// OK
bool Grafo::getPonderadoAresta()
{

    return this->ponderadoAresta;
}

// OK
bool Grafo::getPonderadoNo()
{

    return this->ponderadoNo;
}

// OK
void Grafo::inserirNo(int id, float peso)
{
    No *no = new No(id, peso);

    this->listaNo.push_back(no);
    this->ordem++;
}

void Grafo::inserirNo(int id)
{
    No *no = new No(id);

    this->listaNo.push_back(no);
    this->ordem++;
}
No *Grafo::getNo(int id)
{

    No *no;

    list<No *>::iterator itLista;
    for (itLista = this->listaNo.begin(); itLista != this->listaNo.end(); itLista++)
    {
        if ((*itLista)->getId() == id)
        {
            no = *itLista;
            return no;
        }
    }

    return nullptr;
}

list<No *> Grafo::getListaNos()
{
    return this->listaNo;
}

void Grafo::inserirAresta(int fonteId, int alvoId, float peso)
{
    No *noFonte = getNo(fonteId);
    noFonte->inserirAresta(alvoId, peso);

    No *noAlvo = getNo(alvoId);
    noAlvo->incrementaGrauEntrada(1);

    if (!this->direcionado)
        noAlvo->inserirAresta(fonteId, peso);

    this->numeroArestas++;
}
void Grafo::inserirAresta(int fonteId, int alvoId)
{
    No *noFonte = getNo(fonteId);
    noFonte->inserirAresta(alvoId);

    No *noAlvo = getNo(alvoId);
    noAlvo->incrementaGrauEntrada(1);

    if (!this->direcionado)
        noAlvo->inserirAresta(fonteId);

    this->numeroArestas++;
}

bool Grafo::procuraNo(int id)
{
    if (getNo(id) != nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}
list<list<No *>> Grafo::getListaAdjacencia()
{

    return this->listaAdjacencia;
}
void Grafo::removeNo(int id)
{
    list<No *>::iterator itLista;
    for (itLista = this->listaNo.begin(); itLista != this->listaNo.end(); itLista++)
    {
        if ((*itLista)->getId() != id)
        {
            this->listaNo.remove(*itLista);
        }
    }
}

float Grafo::verificaPesoAresta(int fonteId, int alvoId)
{
    No *primeiro = this->listaNo.front();
    No *no = getNo(fonteId);

    if (no->getAresta(alvoId) != nullptr)
    {
        //cout << "PESO ARESTA : " << no->getAresta(alvoId)->getPeso() << endl;
        return no->getAresta(alvoId)->getPeso();
    }
    //retorna 99999 porque nao existe caminho entre esses dois id's
    return 99999;
}

void Grafo::criaListaAdjacencia()
{
    list<No *>::iterator noAtual;
    for (noAtual = this->listaNo.begin(); noAtual != this->listaNo.end(); noAtual++)
    {
        list<No *> nosAdjacentes;

        list<Aresta *> listaAresta = (*noAtual)->getListaAresta();

        list<Aresta *>::iterator arestaAtual;
        for (arestaAtual = listaAresta.begin(); arestaAtual != listaAresta.end(); arestaAtual++)
        {
            No *noAux = getNo((*arestaAtual)->getAlvoId());

            nosAdjacentes.push_back(noAux);
        }

        this->listaAdjacencia.push_back(nosAdjacentes);

        (*noAtual)->setNosAdjacentes(nosAdjacentes);
    }
}

void Grafo::imprimeListaAdjacencia()
{
    list<list<No *>>::iterator noAtual;
    for (noAtual = this->listaAdjacencia.begin(); noAtual != this->listaAdjacencia.end(); noAtual++)
    {

        list<No *>::iterator noAdjacente;
        for (noAdjacente = noAtual->begin(); noAdjacente != noAtual->end(); noAdjacente++)
            cout << (*noAdjacente)->getId() << " ";
        cout << endl;
    }
    cout << endl;
}

//Funcao para retornar a arvore dada pelo caminhamento em profundidade a partir de dado no parametro
Grafo *Grafo::caminhamentoProfundidade(int id)
{
    //Inizializo o no como sendo o nó parametro 
    No *no = this->getNo(id);
    //Lista para armazenar nós visitados
    list<No *> listaVisitados;

    //Crio um grafo para receber a arvore a ser retornada
    Grafo *retorno = new Grafo(0, false, false, false);

    //Chamo a funcao que funcionara de forma recursiva
    auxCaminhamentoProfundidade(no, &listaVisitados, retorno);

    //retorno a arvore resultante
    return retorno;
}

//Funcao para auxiliar no caminhamento em profundidade
void Grafo::auxCaminhamentoProfundidade(No *filho, list<No *> *listaVisitados, Grafo *grafo)
{
    //Verifico se o no já foi visitado
    if (verificaVisita(filho, (*listaVisitados)))
    {
        //Se nao visitado insiro o no na lista
        listaVisitados->push_back(filho);
        //Insiro o nó no grafo a ser retornado
        grafo->inserirNo(filho->getId(), filho->getPeso());

        //crio uma lista auxiliar que recebe os nos visitados
        list<No *> auxVisitados;
        auxVisitados = *listaVisitados;

        //Percorre os nós enquanto a lista auxiliar não estiver vazia
        while (!auxVisitados.empty())
        {
            //Verifico o peso da aresta e caso != -1 criamos a aresta no grafo retorno com peso 0
            if (this->verificaPesoAresta(filho->getId(), auxVisitados.back()->getId()) != -1)
            {
                grafo->inserirAresta(filho->getId(), auxVisitados.back()->getId(), 0);
                break;
            }
            //Apos o nó ser visitado o retiramos da lista auxiliar
            auxVisitados.pop_back();
        }
        //Criamos uma lista de nós auxiliar
        list<No *> listaAux;
        //Insiro os nós adjacentes na listaAux
        listaAux = filho->getNosAdjacentes();
        //Enquanto essa listaAux nao for vazia chamamos a função recursivamente para percorrer os nós
        while (!listaAux.empty())
        {
            //Chamada recursiva
            auxCaminhamentoProfundidade(listaAux.front(), listaVisitados, grafo);
            //Após visitados retiramos os nós da listaAux
            listaAux.pop_front();
        }
        cout << endl;
    }
}

// funcao para verificar se um no ja foi visitado 
bool Grafo::verificaVisita(No *no, list<No *> listaVisitados)
{
    list<No *>::iterator noIterator;

    for (noIterator = listaVisitados.begin(); noIterator != listaVisitados.end(); noIterator++)
    {
        if ((*noIterator)->getId() == no->getId())
            return false;
    }
    return true;
}
