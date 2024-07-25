#pragma once
#include "reg_paciente.h"
#include <list>
#include <sstream>
using namespace std;

class Diagnosticador{
    private:
        list<Reg_Paciente*> listap; //lista de ponteiros para registros
        ostringstream saida; //atributo que funciona como buffer para armezenar a % de pacientes ok

    public:
        Diagnosticador(const int num = 10);//construtora com 1 parametro definido por default como 10
        ~Diagnosticador();  
        ostringstream& getSaida();
        void diagnosticar();
        //sobrecarga do parametro << para imprimir variaveis do tipo Diagnosticador
};

//nao funciona essa merda de sobrecarga
ostream& operator<<(ostream& saida, Diagnosticador &d);
