#include <iostream>
#include <list>
#include <time.h>
#include <stdlib.h>
using namespace std;

// class Carro;
// class Pedestre;

namespace Controlados {
    class Ente {
        protected:
            int x, y;
            static int cont;
        public:
            Ente(const int xx = 0, const int yy = 0) : x(xx), y(yy) {
            }

            virtual ~Ente() {}

            const int getX() const { return x; }
            const int getY() const { return y; }

            virtual const bool mover(Ente* pE) = 0; // Polimorfica pura
    };
    int Ente::cont(35);

    class Carro : public Ente {
        private:
            const int y;
            static const int MAXVEL;
            int velocidade;
            int multas;
        public:
            Carro(const int xx = 0) : Ente(xx, 5), y(5), velocidade(cont++), multas(0) {}
            // ? - por que é necessario inicializar y duas vezes?

            ~Carro() {}

            const bool mover(Ente* pE); // implementado a frente para evitar dependecias
            const bool frenar() {
                if (velocidade <= MAXVEL) {
                    velocidade = 0;
                    return true;
                } else {
                    velocidade = (int)(velocidade / 2);
                    return false;
                }
            }
            const int getY() const { return y; };
            void operator++() { multas++; }
            const int getX() const { return x; }

        // class Pedestre : public Ente {
        //     private:
        //         const int x;
        //         bool guarda;
        //     public:
        //         Pedestre(const int yy = 0) : Ente(3, yy), x(3), guarda(false) {
        //             if (1 == (rand() % 100)) { guarda = true; }
        //         }
        //         ~Pedestre() {}

        //         const bool Pedestre::mover(Ente* pE) {
        //             y++;
        //             bool moveu = pE->mover(this);

        //             if ((x == pE->getX() && y == pE->getY()) && guarda == true) {
        //                 *(pE)++; // multando o carro

        //                 if (moveu) {
        //                     if (static_cast<Carro*>(pE)->frenar()) {
        //                         return false;
        //                     } else {
        //                         // cout << endl << "! - Acidente GRAVE." << endl;
        //                         return true;
        //                     }
        //                 }
        //             }
        //         }

        //         const int getX() const { return x; }


        // };

    };
    const int Carro::MAXVEL(40);

    class Pedestre : public Ente {
        private:
            const int x;
            bool guarda;
        public:
            Pedestre(const int yy = 0) : Ente(3, yy), x(3), guarda(false) {
                if (1 == (rand() % 100)) { guarda = true; }
            }
            ~Pedestre() {}

            const bool mover(Ente* pE); // implementeado a frente por organização

            const int getX() const { return x; }


    };

    // Métodos mover:
    const bool Carro::mover(Ente* pE) {
        x += velocidade;
        if ((x == pE->getX() && y == pE->getY()) && velocidade > MAXVEL) {
            return true;
        } else {
            return false;
        }
    }
    const bool Pedestre::mover(Ente* pE) {
        y++;
        bool moveu = pE->mover(this);

        if ((x == pE->getX() && y == pE->getY()) && guarda == true) {
            *(pE)++; // multando o carro

            if (moveu) {
                if (static_cast<Carro*>(pE)->frenar()) {
                    return false;
                } else {
                    // cout << endl << "! - Acidente GRAVE." << endl;
                    return true;
                }
            }
        }
    }

}
using namespace Controlados;

namespace Controle {

    class Conjunto {
        private:
            list<Ente*> coletivo;
            list<Ente*>::iterator it;
        public:
            Conjunto() : coletivo(), it() {
                Ente* pE = NULL;
                it = coletivo.begin();
                while (it != coletivo.end()) {
                    pE = *it;
                    if (pE) {
                        delete pE;
                        pE = NULL;
                    }
                    it++;
                }
                coletivo.clear();
            }
            ~Conjunto() {
                coletivo.clear();
            }
            void incluir(Ente* pE) {
                if (pE) {
                    coletivo.push_back((pE));  
                } else {
                    cout << endl << "! - Ponteiro nulo negado." << endl;
                }
            }
            void irParaInicio() {
                it = coletivo.begin();
            }
            void operator++() { it++; }
            const bool fim() const { 
                return (it == coletivo.end() ? true : false);
            }
            Ente* getAtual() { return *it; }

    };

    class Transito {
        private:
            Conjunto pedestres;
            Conjunto carros;
        public:
            Transito() : pedestres(), carros() {
                // populando pedestres
                Pedestre* pP = NULL; // Agora consigo criar Pedestre sem ambiguidade !
                
                for (int i = 0; i < 50; i++) {
                    int y = (int)(rand() % 10);
                    pP = new Pedestre(y);
                    if (pP) {
                        pedestres.incluir(pP);
                    } else {
                        cout << endl << "! - Ponteiro nulo recusado." << endl;
                    }
                }

                Carro* pC = NULL;

                for (int i = 0; i < 10; i++) {
                    int x = (int)(rand() % 10);
                    pC = new Carro(x);
                    if (pC) {
                        carros.incluir(pC);
                    } else {
                        cout << endl << "! - Ponteiro nulo recusado." << endl;
                    }
                }

            }
            ~Transito() {}
            void transitar() {
                for (int i = 0; i < 10; i++) {
                    pedestres.irParaInicio();
                    while (pedestres.fim() == false) {
                        carros.irParaInicio();
                        while (carros.fim() == false) {
                            pedestres.getAtual()->mover(carros.getAtual());
                            carros.operator++();
                        }
                        
                        pedestres.operator++();
                    }
                }
            }

    };
}
using namespace Controle;

int main() {
    time_t tempo;
    srand((unsigned) time(&tempo));

    return 0;
}