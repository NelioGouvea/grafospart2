#include "Aresta.h"
#include <iostream>

using namespace std;

/**************************************************************************************************
 * Defining the Aresta's methods
**************************************************************************************************/

// Constructor
Aresta::Aresta(int alvoId, int fonteId, float peso)
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
float Aresta::getPeso()
{

    return this->peso;
}

// Setters
void Aresta::setPeso(float peso)
{

    this->peso = peso;
}
