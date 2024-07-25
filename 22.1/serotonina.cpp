#include "serotonina.h"
#include "endorfina.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

Serotonina::Serotonina():
Hormonio()/*,
inteiro(-1),
pEndo(NULL)*/
{
    time_t t;
    pEndo = NULL;
    srand(3*((int)time(&t)));
    inteiro = rand()%11; //um inteiro entre 0 e 10
    if(estresse && inteiro>=1)
        inteiro--;
}

Serotonina::~Serotonina()
{
    nivel = "indeterminado";
    inteiro = -1;
    pEndo = NULL;
}

const int Serotonina::getInteiro() const
{
    return inteiro;
}

void Serotonina::setEndo(Endorfina* pE)
{
    pEndo = pE;
}

void Serotonina::calcular_nivel()
{
    if(inteiro<=4 && pEndo->getReal()>0.0)
        nivel = "baixo";
    else if(inteiro>4 && inteiro<7 && pEndo->getReal()>3.0)
        nivel = "medio";
    else if(inteiro>=7 && pEndo->getReal()>5.0)
        nivel = "normal";

    std::cout << "Inteiro: " << inteiro << " Nivel de serotonina: " << nivel << endl;
}