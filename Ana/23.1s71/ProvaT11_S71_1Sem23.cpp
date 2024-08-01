// Prof Simão - DAINF/UTFPR - Ctba - TecProg S71 - 1 Sem 2022 - 1a Parcial
// Hah couts suplementares para fins de entedimento da execução.

#include <vector>
#include <list>
#include <iostream>
using namespace std;

#include <stdlib.h>
#include <time.h>

class Inseto
{
    private:
        int energia;

    public:

        Inseto(const int energ = 0): energia(energ)
        {
        }

        virtual ~Inseto()
        {
            energia = -1;
        }

        const int getEnergia() const { return energia; }

        virtual void sobreviver() = 0;

        void operator--() { energia--; }
};

class Formigueiro;

class Gafanhoto : public Inseto
{
    private:
        const bool invasor;
        static Formigueiro* pGueiro;

    public:

        Gafanhoto(const int energ = 10):
            Inseto(energ),
            invasor ((bool)(rand()%2))
        {
        }

        ~Gafanhoto()
        {
        }

        const bool getInvasor() const { return invasor; }

        static void setFormigueiro(Formigueiro* p) { pGueiro = p; }

        void sobreviver(); // a ser implementado depois da criação da classe Formigueiro dado a dependência.
};

Formigueiro* Gafanhoto::pGueiro(NULL);

class Formiga : public Inseto
{
    private:
        static Gafanhoto*   pGafa;
        static int          cont;
        int                 ferrao;

    public:
        Formiga(const int energ = 1) :
            Inseto(energ),
            ferrao(cont++)
        {
            //cont++;
        }

        ~Formiga()
        {
            cont--;
        }

        static void setGafa(Gafanhoto* pG)
        {
            pGafa = pG;
        }

        void sobreviver()
        {
            if (pGafa)
            {
                int quant = 0;

                pGafa->getInvasor() ? quant = 2*ferrao : quant = ferrao;

                for (int i = 0; i < quant; i++)
                {
                    pGafa->operator--();
                    //cout << "Decrementei energia do Gafanhoto: " << pGafa->getEnergia() << endl;
                }
                operator--();
            }
            else
            {
                cout << "Nao há gafanhoto atacando o formigueiro! - pGafa é nulo!" << endl;
            }
        }
};

Gafanhoto* Formiga::pGafa(NULL);
int Formiga::cont(1);

class Formigueiro
{
    private:
        vector<Formiga*> colecao;

    public:
        Formigueiro(const int nFormigas = 100):
        colecao()
        {
            colecao.clear();

            Formiga* pF = NULL;

            for (int i = 0; i < nFormigas; i++)
            {
                pF = new Formiga(100);
                if (pF)
                {
                    colecao.push_back(pF);
                }
                else
                {
                    continue;
                    cout << "Alocação de formiga não realizada!" << endl;
                }
            }
        }

        ~Formigueiro()
        {
            int tam = (int) colecao.size();
            for (int i = 0; i < tam; i++)
            {
                if (colecao[i])
                {
                    delete colecao[i];
                    colecao[i] = NULL;
                }
                else
                {
                }
            }
            colecao.clear();
        }

        void eliminar_ultima()
        {
            if (!colecao.empty())
            {
                Formiga* pF = NULL;
                pF = colecao.back();
                colecao.pop_back();
                if (pF)
                {
                    delete pF;
                    pF = NULL;
                }
                else
                {
                    cout << "A última formiga estava nula!" << endl;
                }
            }
            else
            {
                cout << "Colecao de formigas vazia!" << endl;
            }
        }

        void eliminar_sem_energia()
        {
            // Para eliminar os ponteiros aterrados com nulo
            bool flag = true;
            //int tam = 0;
            while (flag)
            {
                int tam = (int) colecao.size();

                //cout << "O tamanho do vector agora eh: " << tam << endl;
                //system("pause");

                for (int i = 0; i < tam; i++)
                {
                    if (colecao[i]->getEnergia() <= 0 )
                    {
                            delete colecao[i];
                            colecao[i] = NULL;
                    }

                    if (NULL == colecao[i])
                    {
                        //cout << "O elemento do vector da seguinte posicao serah eliminado: " << i << endl;
                        //system("pause");
                        vector<Formiga*>::iterator it;
                        it = colecao.begin()+i;
                        colecao.erase(it);
                        break;   // dado que tam nao eh mais válido.
                    }
                }
                flag = false;
            }

            // ums solucao quiçá melhor seria por iterador...
        }

        void defender(Gafanhoto* pGafa)
        {
            bool    luta    = true;
            //int     tam     = 0;
            Formiga::setGafa(pGafa);
            while (luta)
            {
                int tam = (int)colecao.size();

                if (pGafa->getEnergia() <= 0)
                {
                    cout << "Gafanhoto derrotado" << endl;
                    luta = false;
                }

                if (tam <= 0)
                {
                    cout << "Formigueiro derrotado" << endl;
                    luta = false;
                }

                if (false==luta)
                {
                    return;
                }

                for (int i = 0; i < tam; i++)
                {
                    if (colecao[i])
                    {
                        colecao[i]->sobreviver();
                    }
                }

                eliminar_sem_energia();
            }
        }
};

void Gafanhoto::sobreviver()
{
    if (pGueiro)
    {
        pGueiro->eliminar_ultima();
        pGueiro->defender(this);
    }
    else
    {
        cout << "Ponteiro nulo!" << endl;
    }
}

class Habitat
{
    private:
        const int           num_gafs;
        Formigueiro         formigueiro;
        list<Gafanhoto*>    colecao;

    public:

        Habitat():
            num_gafs(10),
            formigueiro((int)(rand()%1000)), // https://www.cprogressivo.net/2013/03/Como-gerar-numeros-aleatorios-em-C-com-a-rand-srand-e-seed.html
            colecao()
        {
            colecao.clear();
            Gafanhoto::setFormigueiro(&formigueiro);
            criarGafanhotos();
            executar();
        }

        ~Habitat()
        {
            list<Gafanhoto*>::iterator it;
            it = colecao.begin();
            while (it != colecao.end())
            {
                if (*it)
                {
                    delete (*it);
                }
                it++;
            }
            colecao.clear();

        }

        void criarGafanhotos()
        {
            Gafanhoto* pG = NULL;

            for (int i = 0; i < num_gafs; i++)
            {
                pG = new Gafanhoto(1);
                if (pG)
                {
                    colecao.push_back(pG);
                    pG = NULL;
                }
                else
                {
                    cout << "Gafanhoto não alocado" << endl;
                }
            }
        }

        void executar()
        {
            list<Gafanhoto*>::iterator it;
            it = colecao.begin();
            while (it != colecao.end())
            {
                if (*it)
                {
                    (*it)->sobreviver();
                }
                it++;
            }
        }
};

int main ()
{
    time_t t;   // https://pt.wikipedia.org/wiki/Time_t
                // Para função srand e rand vide: https://www.tutorialspoint.com/c_standard_library/c_function_srand.htm
    srand((unsigned) time(&t));

    Habitat habitat;
    return 0;
}

