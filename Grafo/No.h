#ifndef NO_H
#define NO_H
#include <list>
#include "Aresta.h"

class No
{
private:
    float peso;
    int id;
    float x;
    float y;
    list<Aresta *> listaAresta;
    list<No *> nosAdjacentes;

    int grauEntrada;
    int grauSaida;

    int grau;

public:
    //Construtor e destrutor
    No(int id, float peso);
    No(int id);
    No(int id, float x, float y);
    No();
    ~No();

    //gets
    int getPeso();
    int getId();
    float getX();
    float getY();
    Aresta *getAresta(int alvoId);
    list<Aresta *> getListaAresta();
    list<No *> getNosAdjacentes();

    int getGrauEntrada();
    int getGrauSaida();

    //sets
    void setPeso(int peso);
    void setId(int id);
    void setX(float x);
    void setY(float y);
    void setCoordenadas(float x, float y);
    void setNosAdjacentes(list<No *> nos);

    void inserirAresta(int alvoId, int peso);
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
