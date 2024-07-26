#include "endorfina.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>


Endorfina::Endorfina():
Hormonio()/*,
pSero(NULL),
real(-1.0)*/
{
    pSero = NULL;
    srand((int)time(NULL));
    real = ((float)rand()/RAND_MAX)*10; //um numero real entre 0 e 10
}

Endorfina::~Endorfina()
{
    nivel = "indeterminado";
    real = -1.0;
    pSero = NULL;
}

const float Endorfina::getReal() const
{
    return real;
}

void Endorfina::setSero(Serotonina* pS)
{
    pSero = pS; //associa serotonina a endorfina
    pS->setEndo(this); //e endorfina a serotonina
}

void Endorfina::calcular_nivel()
{
    if(real<=3.0 && pSero->getInteiro()>0)
        nivel = "baixo";
    else if(real>3.0 && real<6.0 && pSero->getInteiro()>5)
        nivel = "medio";
    else if(real>=6.0 && pSero->getInteiro()>7)
        nivel = "normal";

    std::cout << "Real: " << real << " Nivel de endorfina: " << nivel << endl;
}