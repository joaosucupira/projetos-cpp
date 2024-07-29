#pragma once
#include "endorfina.h"
#include "serotonina.h"

class Reg_Paciente{
    private:
        Endorfina nivel_endo;
        Serotonina nivel_sero;
        bool ok;

    public:
        Reg_Paciente();
        ~Reg_Paciente();
        const int getOK() const;
        void auto_avaliar();
};