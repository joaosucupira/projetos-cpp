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
        Endorfina() : Hormonio() {
            real = 1.0 * (rand() % 11); 
        }
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
class Serotonina : public Hormonio {
    private:
        int inteiro;
        Endorfina* pEndo;
    public:
        Serotonina() : Hormonio() {
            inteiro = rand() % 11;
            if (estresse == true && inteiro > 0) { inteiro--; }
        }
        ~Serotonina() {
            pEndo = NULL;
        }
        void setEndo(Endorfina* pE) {
            pEndo = pE;
        }
        void calcular_nivel(); // Alhures para evitar dependencia
        const int getInteiro() const { return inteiro; }
};

// Polimorficos:
void Endorfina::calcular_nivel() {
    int intSero = pSero->getInteiro();
    if (real >= 6.0 && real <= 10.0 && intSero > 7) {
        nivel = "normal";
    } else if (real > 3.0 && real < 6.0 && intSero > 5) {
        nivel = "medio";
    } else if (real >= 0.0 && real <= 3.0 && intSero > 0) {
        nivel = "baixo";
    } 
}

void Serotonina::calcular_nivel() {
    float realEndo = pEndo->getReal();
    if (inteiro >= 7 && inteiro <= 10 && realEndo > 5.0) {
        nivel = "normal";
    } else if (inteiro > 4 && inteiro < 7 && realEndo > 3.0) {
        nivel = "medio";
    } else if (inteiro >= 0.0 && inteiro <= 4.0 && realEndo > 0.0) {
        nivel = "baixo";
    } 
}

// Classe gerenciadora que agrega hormonios do paciente
// Agrega: 1 SEROTONINA, 1 ENDORFINA
class Reg_Paciente {
    private:
        bool ok;
        Endorfina nivel_endo;
        Serotonina nivel_sero;
    public: 
        Reg_Paciente() {
            nivel_endo.calcular_nivel();
            nivel_sero.calcular_nivel();
        }
        const int getOK() const { return ok; }
        void auto_avaliar() {
            if (nivel_endo.getNivel() == "baixo" || nivel_sero.getNivel() == "baixo") {
                ok = false;
            } else {
                ok = true;
            }
        }
};

// Classe principal executadora das classes do projeto
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
    srand(time(NULL));

    Diagnosticador diag1(100);
    // cout << diag1 << endl;
    return 0;
}