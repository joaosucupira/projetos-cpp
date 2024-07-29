#pragma once
#include "hormonio.h"

class Endorfina;

class Serotonina : public Hormonio{
    private:
        int inteiro;
        Endorfina* pEndo;

    public:
        Serotonina();
        ~Serotonina();
        const int getInteiro() const;
        void setEndo(Endorfina* pE);
        void calcular_nivel();
};