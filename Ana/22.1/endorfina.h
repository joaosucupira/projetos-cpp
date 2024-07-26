#pragma once
#include "hormonio.h"
#include "serotonina.h"
using namespace std;

class Endorfina : public Hormonio{
    private:
        float real;
        Serotonina* pSero;

    public:
        Endorfina();
        ~Endorfina();
        const float getReal() const;
        void setSero(Serotonina* pS);
        void calcular_nivel();
};