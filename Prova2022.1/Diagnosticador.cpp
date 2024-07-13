#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <list>

using namespace std;

class Hormonio;
class Endorfina;
class Serotonina;
class Reg_Paciente;

// Classe abstrata, derivadas: Endorfina, Serotonina.
// Polimorficas: calcular_nivel
class Hormonio {
    protected:
        string nivel = "indeterminado";
        bool estresse;
        static int soma;
    public:
        Hormonio() {
            soma++;
            estresse = (soma % 2 == 0 ? true : false);
        }
        ~Hormonio() {
            soma--;
            estresse = NULL;
        }
        const string getNivel() const { return nivel; }
        virtual void calcular_nivel() = 0;
};

// Soma inicializado alhures
int Hormonio::soma = 0;

// Classe derivada de: Hormonio (polimorfica:calcular_nivel)
class Endorfina : public Hormonio {
    private:
        float real;
        Serotonina* pSero;
    public:
        Endorfina() : Hormonio() {}
        ~Endorfina() {
            pSero = NULL;
        } 
        const float getReal() const { return real; }
        void setSero(Serotonina* pS) {
            pSero = pS;
        }
        void calcular_nivel(); // Alhures para evitar dependencia
};

// Classe derivada de: Hormonio (polimorfica:calcular_nivel)
class Serotonina {};
class Diagnosticador {
    private:
        list<Reg_Paciente*> listap;
        ostringstream saida;
    public:
        Diagnosticador(const int num = 10) {}
        ~Diagnosticador() {}
        ostringstream& getSaida() { return saida; }
        void diagnosticar() {}
        ostringstream& operator<<(Diagnosticador diag) {
            return getSaida();
        };

};

int main() {
    Diagnosticador diag1(100);
    // cout << diag1 << endl;
    return 0;
}