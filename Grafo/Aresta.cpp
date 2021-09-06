#include "Aresta.h"
#include <iostream>

using namespace std;

/**************************************************************************************************
 * Defining the Aresta's methods
**************************************************************************************************/

// Constructor
Aresta::Aresta(int alvoId, int fonteId, int peso)
{

    this->alvoId = alvoId;
    this->fonteId = fonteId;
    this->peso = peso;
}

// Destructor
Aresta::~Aresta()
{
}

// Getters
int Aresta::getAlvoId()
{

    return this->alvoId;
}
int Aresta::getFonteId()
{

    return this->fonteId;
}
int Aresta::getPeso()
{

    return this->peso;
}

// Setters
void Aresta::setPeso(int peso)
{

    this->peso = peso;
}
