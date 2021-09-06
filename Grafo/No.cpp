#include <list>
#include <iterator>
#include "Aresta.h"
#include "No.h"

#include <iostream>

using namespace std;

//Construtores

No::No(int id, float x, float y)
{
    this->id = id;
    this->x = x;
    this->y = y;
}
No::No(int id, int peso)
{
    this->peso = peso;
    this->id = id;
}
No::No(int id)
{
    this->id = id;
}
No::No()
{
    this->peso = -1;
    this->id = -1;
}

//Destrutor
No::~No()
{
    this->listaAresta.~list();
    this->nosAdjacentes.~list();
}

//gets

//retorna o id
int No::getId()
{
    return this->id;
}
//retorna o peso
int No::getPeso()
{
    return this->peso;
}
float No::getX()
{
    return this->x;
}
float No::getY()
{
    return this->y;
}

Aresta *No::getAresta(int alvoId)
{
    Aresta *aresta;

    list<Aresta *>::iterator itLista;
    for (itLista = this->listaAresta.begin(); itLista != this->listaAresta.end(); itLista++)
    {
        if ((*itLista)->getAlvoId() == alvoId)
        {
            aresta = *itLista;
            return aresta;
        }
    }

    return nullptr;
}

list<Aresta *> No::getListaAresta()
{
    return this->listaAresta;
}

list<No *> No::getNosAdjacentes()
{
    return this->nosAdjacentes;
}

int No::getGrauEntrada()
{
    return this->grauEntrada;
}

int No::getGrauSaida()
{
    return this->grauSaida;
}

//setters

//adiciona valor a variavel peso
void No::setPeso(int peso)
{
    this->peso = peso;
}
//adiciona valor a variavel id
void No::setId(int id)
{
    this->id = id;
}
void No::setX(float x)
{
    this->x = x;
}
void No::setY(float y)
{
    this->y = y;
}
void No::setCoordenadas(float x, float y)
{
    this->x = x;
    this->y = y;
}
void No::setNosAdjacentes(list<No *> nos)
{
    this->nosAdjacentes = nos;
}

void No::inserirAresta(int alvoId, int peso)
{
    Aresta *aresta = new Aresta(alvoId, this->id, peso);

    bool jaTem = false;
    list<Aresta *>::iterator arestaAtual;
    for (arestaAtual = this->listaAresta.begin(); arestaAtual != this->listaAresta.end(); arestaAtual++)
        if ((*arestaAtual)->getAlvoId() == alvoId)
            jaTem = true;

    if (!jaTem)
    {
        this->listaAresta.push_back(aresta);
        this->grauSaida++;
    }
}
void No::inserirAresta(int alvoId)
{
    Aresta *aresta = new Aresta(alvoId, this->id, 0);

    bool jaTem = false;
    list<Aresta *>::iterator arestaAtual;
    for (arestaAtual = this->listaAresta.begin(); arestaAtual != this->listaAresta.end(); arestaAtual++)
        if ((*arestaAtual)->getAlvoId() == alvoId)
            jaTem = true;

    if (!jaTem)
    {
        this->listaAresta.push_back(aresta);
        this->grauSaida++;
    }
}
void No::removerAresta(int alvoId)
{
    list<Aresta *>::iterator itLista;
    for (itLista = this->listaAresta.begin(); itLista != this->listaAresta.end(); itLista++)
    {
        if ((*itLista)->getAlvoId() == alvoId)
        {
            this->listaAresta.remove(*itLista);
            this->grauSaida--;
            break;
        }
    }
}

void No::incrementaGrauEntrada(int val)
{
    this->grauEntrada += val;
}
void No::incrementaGrauSaida(int val)
{
    this->grauSaida += val;
}

int No::getGrau() {
    return this->grau;
}

void No::setGrau(int val) {
    this->grau = val;
}

bool No::getVisitado() {
    return this->visitado;
}

void No::setVisitado(bool val) {
    this->visitado = val;
}