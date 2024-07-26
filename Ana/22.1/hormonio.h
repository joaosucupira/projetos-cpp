#pragma once
#include <string>
using namespace std;

class Hormonio{
    protected:
        string nivel;
        bool estresse;
    
    private:
        static int soma;

    public:
        Hormonio();
        virtual ~Hormonio(); //pode fazer isso??
        const string getNivel() const;
        virtual void calcular_nivel() = 0; 
        //função virtual pura que faz de Hormonio uma classe abstrata
};