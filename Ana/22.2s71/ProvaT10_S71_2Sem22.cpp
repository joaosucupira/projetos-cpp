// Prof Sim�o - DAINF/UTFPR - Ctba - TecProg S71 - 2 Sem 2022 - 1a Parcial
// Hah couts suplementares para fins de entedimento da execu��o.

#include <vector>
#include <iostream>
using namespace std;

#include <stdlib.h>
#include <time.h>

namespace Bombas
{
    class Atomica
    {
        protected:
            static int potencial_atomico;
            int megatons;
        public:

            Atomica(const int mgtns = 5):
                megatons (mgtns)
            {
                potencial_atomico += megatons;
            }

            virtual ~Atomica()
            {
                potencial_atomico -= megatons;
            }

            const int getMegatons() const
            {
                return megatons;
            }

            static const int getPotencialAtomico()
            {
                return potencial_atomico;
            }

            virtual bool explodir() = 0;

    };

    int Atomica::potencial_atomico(0);

    class Fusao;

    class Fissao : public Atomica
    {
        private:
            int     potencial;
            Fusao*  pFusao;
        public:

            Fissao(Fusao* pFs = NULL, const int mgtns = 15)
                : Atomica(mgtns), potencial (0), pFusao (pFs)
            {
                time_t t;     // https://pt.wikipedia.org/wiki/Time_t
                // Para fun��o srand e rand vide: https://www.tutorialspoint.com/c_standard_library/c_function_srand.htm
                // https://www.cprogressivo.net/2013/03/Como-gerar-numeros-aleatorios-em-C-com-a-rand-srand-e-seed.html
                srand((unsigned) time(&t));
                potencial = rand()%100;

            }

            ~Fissao()
            {
                pFusao = NULL;
            }

            bool explodir();
    };

    class Fusao : public Atomica
    {
        private:
            Fissao  Detonadora;
            bool    duplicador;

        public:
            Fusao(const int mgtns = 100) :
                Atomica(mgtns),
                Detonadora(this),
                duplicador(false)
            {
            }

            ~Fusao() { }

            void setDuplicador(bool dp)
            {
                duplicador = dp;
            }

            const int getMegatons() const
            {
                if (duplicador)
                {
                    return (2 * megatons);
                    //potencial_atomico += megatons;
                }
                else
                {
                    return megatons;
                }
            }

            bool explodir()
            {
                if (Detonadora.explodir())
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        };

        bool Fissao::explodir()
        {
            if (potencial <= 33)
            {
                return false;
            }
            else
            {
                if (potencial > 77)
                {
                    pFusao->setDuplicador(true);
                }
                return true;
            }
        }
} // Fim namespace
using namespace Bombas;

class Arsenal
{
private:
    vector<Fusao*> colecao;

public:
    Arsenal(const int nBombas = 100):
    colecao()
    {
        Fusao* pFusao = NULL;

        for (int i = 0; i < nBombas; i++)
        {
            pFusao = new Fusao();
            if (pFusao)
            {
                colecao.push_back(pFusao);
            }
            else
            {
                // ...
            }
            pFusao = NULL;
        }
    }

    ~Arsenal()
    {
        Fusao* pFusao = NULL;
        int nBombas = (int) colecao.size();
        for (int i = 0; i < nBombas; i++)
        {
            pFusao = colecao[i];

            if (pFusao)
            {
                delete(pFusao);
            }
            pFusao = NULL;
        }
        colecao.clear();
    }

    const int getTotalMegatons() //const
    {
        int total = NULL;
        Fusao* pFusao = NULL;

        int nBombas = (int) colecao.size();

        for (int i = 0; i < nBombas; i++)
        {
            pFusao = colecao[i];
            if (pFusao)
            {
                if (pFusao->explodir())
                {
                    total += pFusao->getMegatons();
                }
            }
            pFusao = NULL;
        }
        return total;
    }
};


class Nacao
{
private:
    static const int    MAX;
    bool                atacar;
    Nacao*              pBeligerante;

public:
    Arsenal             arsenal; // Apessar de n�o ser boa pr�tica, modelo permite.

public:
    Nacao():
        atacar(false),
        pBeligerante(NULL),
        arsenal()
    {
        int p = 0;
        time_t t;
        srand((unsigned) time(&t));
        p = rand()%100;
        if (1 == p)
        {
            atacar = true;
        }
    }

    ~Nacao()
    {
        pBeligerante = NULL;
    }

    const int getAtacar() const
    {
        return atacar;
    }

    void informar_dano() // a revelia do enuncinado, n�o constante pq nao funcionaria.
    {
        cout << endl;

        if (pBeligerante)
        {
            int total = arsenal.getTotalMegatons();

            total+= pBeligerante->arsenal.getTotalMegatons();
            if ( total > MAX )
            {
                cout << "Fim do Mundo!" << endl;
                exit(0);
            }
            else
            {
                if ( total > (MAX/2))
                {
                    cout << "Inverno Nuclear!" << endl;
                }
                else
                {
                    cout << "Planeta Danificado!" << endl;
                }
            }
        }
        else
        {
            cout << "N�o h� danos!" << endl;
        }
    }

    void defender()
    {
        cout << endl;

        if (pBeligerante)
        {
            if (pBeligerante->getAtacar())
            {
                informar_dano();
            }
            else
            {
                cout << "H� paz e o planeta est� a salvo!" << endl;
            }
        }
        else
        {
            cout << "N�o h� beligerante!" << endl;
        }
        cout << endl;
    }

    void operator=(Nacao* pBelg)
    {
        pBeligerante = pBelg;
    }
};

const int Nacao::MAX(1000);

int main ()
{
    Nacao A;
    Nacao B;

    cout << "O pontecial at�mico � pelo menos: " << Atomica::getPotencialAtomico() << " megatons!" << endl << endl;

    A = &B;
    A.defender();
    B.defender();
    return 0;
}

