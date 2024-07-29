// Prof Sim�o - DAINF/UTFPR - Ctba - TecProg S73
// - 1 Sem 2023 - 1a Parcial
// Hah couts suplementares para fins de entedimento da execu��o.

#include <stdlib.h>
#include <time.h>

#include <list>
#include <iostream>
using namespace std;

namespace Controlados
{
    class Ente
    {
        protected:
            int x;
            int y;

        protected:
            static int cont;

        public:
            Ente(const int xx = 0, const int yy = 0):
                x (xx),
                y (yy)
            {

            }

            virtual ~Ente()
            {
            }

            virtual const int getX() const
            {
                return x;
            }

            /* void setX(const int xx)
            {
                x = xx;
            }*/

            virtual const int getY() const
            {
                return y;
            }

            virtual const bool mover(Ente* pE) = 0;
    };
    int Ente::cont(35);

    class Carro : public Ente
    {
        private:
            const int y;
            static const int MAXVEL;
            int  velocidade;    // como o enunciado pedia para alterar a velocidade, ela n�o poderia ser constante...
            int multas;

        public:
            Carro (const int xx = 0):
                Ente(xx, 5),
                y(5),
                velocidade(cont++),
                multas(0)
            {

            }

            ~Carro ()
            {
            }

            const bool mover(Ente* pE)
            {
                x += velocidade;

                //cout << "Movi o carro: " << pE << endl;


                //cout << "O carro " << this << " tem o seguinte x y " << getX() << " " << getY() << endl;
                //cout << "O pedestre " << pE << " tem o seguinte x y " << pE->getX() << " " << pE->getY() << endl;

                if (    (getX() == pE->getX()) &&
                        (getY() == pE->getY()) )
                {

                    cout << "O carro " << this << " e o pedestre " << pE << " estao na mesma posi��o!" << endl;

                    if (velocidade > MAXVEL)
                    {
                        cout << "Acidente grave com do carro " << this << " para com o pedestre " << pE << endl;

                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                return false;
            }

            const bool frenar()
            {

                if (velocidade <= MAXVEL)
                {
                    velocidade = 0;
                    return true;
                }
                else
                {
                    velocidade = (int)velocidade/2;
                    return false;
                }
            }

            const int getY() const
            {
                return y;
            }

            void operator++()
            {
                multas++;
            }

            const int getMultas() const
            {
                return multas;
            }
        };

        const int  Carro::MAXVEL = 40;


        class Pedestre : public Ente
        {
            private:
                const int x;
                bool guarda;

            public:
                Pedestre(const int yy = 0) :
                    Ente(5, yy),
                    x(5),
                    guarda(false)
                {
                    if ( (int)(rand()%100) == 1)
                    {
                        guarda = true;
                    }
                }

                ~Pedestre()
                {
                }

                const bool mover(Ente* pE)
                {
                    y++;

                    cout << "Movi o pedestre: " << pE << endl;

                    Carro* pC = static_cast<Carro*> (pE);


                    bool rC = pC->mover((Ente*)this);

                    if (    (getX() == pC->getX()) &&
                            (getY() == pC->getY()) )
                    {
                        if (rC && guarda)
                        {
                            pC->operator++();
                        }

                        if (pC->frenar())
                        {
                            cout << "Frenou a tempo! Acidente menor!" << endl;
                            return false;
                        }
                        else
                        {
                            cout << "Acidente grave!" << endl;
                            return true;
                        }
                }

                return false;
            }

            const int getX() const
            {
                return x;
            }
    };
}
using namespace Controlados;

namespace Controle
{
    class Conjunto
    {
        private:
            list<Ente*> coletivo;
            list<Ente*>::iterator it;

        public:

            Conjunto():
                coletivo(),
                it()
            {
                coletivo.clear();
            }

            ~Conjunto()
            {
                Ente* pE = NULL;

                it = coletivo.begin();

                while (it != coletivo.end())
                {
                    pE= *it;

                    if (pE)
                    {
                        delete pE;
                        pE = NULL;
                    }
                    it++;
                }
                coletivo.clear();
            }

            void incluir(Ente* pe)
            {
                if (pe)
                {
                    coletivo.push_back(pe);
                }
                else
                {
                    cout << "Nao se pode incluir ponteiro nulo!" << endl;
                }
            }

            void irParaInicio()
            {
                it = coletivo.begin();
            }

            void operator++()
            {
                //cout << "Incrementei o iterador!" << endl;
                it++;
            }

            const bool fim() const
            {
                if (it == coletivo.end())
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            Ente* getAtual()
            {
                return *it;
            }
    };

    class Transito
    {
        private:
            Conjunto pedestres;
            Conjunto carros;

        public:
            Transito():
                pedestres(),
                carros()
            {
                Pedestre* pP = NULL;
                for (int i = 0; i < 50; i++)
                {
                    int y = (int)(rand()%10);
                    pP = new Pedestre(y);

                    if (pP)
                    {
                        pedestres.incluir(pP);
                        //cout << " Inclui o Pedestre: " << pP << endl;

                    }
                    else
                    {
                        cout << " Pedestre n�o instaciando - ponteiro nulo! " << endl;
                    }
                }

                Carro* pC = NULL;
                for (int i = 0; i < 10; i++)
                {
                    int x = (int)(rand()%10);

                    pC = new Carro(x);

                    if (pC)
                    {
                        carros.incluir(pC);
                       // cout << " Inclui o Carro: " << pC << endl;

                    }
                    else
                    {
                        cout << " Pedestre n�o instaciando - ponteiro nulo! " << endl;
                    }
                }

                transitar();
            }

            ~Transito()
            {
            }

            void transitar()
            {
                bool flag = false;

                for (int i = 0; i < 10; i++)
                {
                    Pedestre* pP = NULL;

                    pedestres.irParaInicio();

                    while (pedestres.fim() == false)
                    {

                         pP = (Pedestre*)pedestres.getAtual();

                         carros.irParaInicio();

                         while (carros.fim() == false)
                         {
                            //Ente*  pE  = NULL;
                            Carro* pC = NULL;
                            //pE = carros.getAtual();
                            pC = (Carro*) carros.getAtual();

                            bool rP = pP->mover((Ente*)pC);

                            //cout << "Movendo o ente " << pE << " que � o pedestre " << pP << " em fun��o do carro " << pC << endl;

                            if (rP == true)
                            {
                                flag = true;
                            }

                            carros.operator++();
                         }
                         pedestres.operator++();
                    }

                    if (flag)
                    {
                        carros.irParaInicio();

                        int cont = 0;

                        while (carros.fim() == false)
                        {
                            Carro* pC = NULL;
                            pC = (Carro*) carros.getAtual();
                            cout << "A quantidade de multa do carro " << cont << " eh: " << pC->getMultas();
                            carros.operator++();
                        }

                        break;
                    }
                }
                //cout << "Msg" << endl;
            }
    };
}
using namespace Controle;

int main ()
{

   time_t tempo;
   // https://pt.wikipedia.org/wiki/Time_t
   // Para fun��o srand e rand vide:
   // https://www.tutorialspoint.com/c_standard_library/c_function_srand.htm
   srand((unsigned) time(&tempo));

   Transito t;

   return 0;
}




