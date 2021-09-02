#ifndef NO_H
#define NO_H
#include <list>
#include "Aresta.h"

class No
{
private:
    float peso;
    int id;
    list<Aresta *> listaAresta;
    list<No *> nosAdjacentes;

    int grauEntrada;
    int grauSaida;

    int grau;

public:
    //Construtor e destrutor
    No(int id, float peso);
    No(int id);
    No();
    ~No();

    //gets
    float getPeso();
    int getId();
    Aresta *getAresta(int alvoId);
    list<Aresta *> getListaAresta();
    list<No *> getNosAdjacentes();

    int getGrauEntrada();
    int getGrauSaida();

    //sets
    void setPeso(float peso);
    void setId(int id);
    void setNosAdjacentes(list<No *> nos);

    void inserirAresta(int alvoId, float peso);
    void inserirAresta(int alvoId);
    void removerAresta(int alvoId);

    bool operator==(const No &s) const { return id == s.id; }
    bool operator!=(const No &s) const { return !operator==(s); }

    void incrementaGrauEntrada(int val);
    void incrementaGrauSaida(int val);

    int getGrau();
    void setGrau(int val);
};

#endif // NO_H
