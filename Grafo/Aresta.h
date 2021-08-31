/**************************************************************************************************
 * Implementation of the TAD Aresta
**************************************************************************************************/

#ifndef ARESTA_H_INCLUDED
#define ARESTA_H_INCLUDED

using namespace std;

// Definition of the Edge class
class Aresta
{

    // Attributes
private:
    int alvoId;
    int fonteId;
    float peso;

public:
    // Constructor
    Aresta(int alvoId, int fonteId, float peso);
    // Destructor
    ~Aresta();
    // Getters
    int getAlvoId();
    int getFonteId();
    float getPeso();
    // Setters
    void setPeso(float peso);
    bool operator==(const Aresta &s) const { return alvoId == s.alvoId; }
    bool operator!=(const Aresta &s) const { return !operator==(s); }

    bool operator<(const Aresta &s) const { return peso < s.peso; }
    bool operator>(const Aresta &s) const { return peso > s.peso; }
    bool operator<=(const Aresta &s) const { return peso <= s.peso; }
    bool operator>=(const Aresta &s) const { return peso >= s.peso; }
};

#endif // ARESTA_H_INCLUDED
