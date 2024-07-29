// Prof Sim�o - DAINF/UTFPR - Ctba - TecProg S71 - 2 Sem 2023 - 1a Parcial
// H� couts suplementares para fins de entedimento da execu��o.

#include <vector>
#include <list>
#include <set>
#include <iostream>
using namespace std;

#include <stdlib.h>
#include <time.h>

class Cidadao
{
    protected:
        static int cont_id;
        int id;

    public:
        Cidadao():
            id (cont_id++)
        {
        }

        virtual ~Cidadao()
        {
            id = -1;
        }

        virtual void votar() = 0;

        virtual void desalienar() = 0;

        virtual const int getLimpo() const = 0;

        const int getId() const { return id; }
};
int Cidadao::cont_id(0);

class Partido
{
    private:
        list <Cidadao*> filiados;

    public:
        Partido()
        {
            filiados.clear();
        }

        ~Partido()
        {
            filiados.clear();
        }

        void filiar(Cidadao* p)
        {
            if (p)
            {
                filiados.push_back(p);
            }
            else
            {
                cout << "Ponteiro nulo em filiados n�o permitido!" << endl;
            }
        }

        void desfiliar(Cidadao* p)
        {
            if (p)
            {
                filiados.remove(p);
            }
            else
            {
                cout << "Ponteiro nul em filiados n�o permitido!" << endl;
            }
        }

        void politizar()
        {
            list<Cidadao*>::iterator iterador;

            iterador = filiados.begin();

            while (iterador != filiados.end())
            {
                (*iterador)-> desalienar();
                if ( (*iterador)-> getLimpo() < 77 )
                {
                    cout << "Desfiliando: " << (*iterador)->getId() << endl;
                    desfiliar(*iterador);
                }
                iterador++;
            }
        }
};

class Candidato;

class Eleitor : public Cidadao
{
    private:
        bool alienado;
        set<Candidato*> cands;
        const int limpo;

    public:
        Eleitor():
            Cidadao(),
            alienado ((bool)(rand()%2)),
            cands(),
            limpo(100)
        {
            cands.clear();
        }

        virtual ~Eleitor()
        {
            cands.clear();
        }

        virtual void votar(); // a ser implementado depois da cria��o da classe Candidato dado a depend�ncia.

        virtual const int getLimpo() const { return limpo; }

        void desalienar() { alienado = false; }

        void considerar(Candidato* p)
        {
            if (p)
            {
                cands.insert(p);
            }
            else
            {
                cout << "Nao se pode incluir nulo em cands!" << endl;
            }
        }
};

class Candidato : public Eleitor
{
    protected:
        static int  votos_total;
        int         votos;
        int         limpo;

    public:
        Candidato() :
            Eleitor(),
            votos(0),
            limpo(rand()%100)
        {
        }

        virtual ~Candidato() // como h� outra fun��o virtual, deixei construtora tamb�m virtual...
        {
        }

        virtual void votar() // uma outra solu��o seria deixar como n�o virtual afirmando que � classe folha...
        {
            operator++();

            cout    << "Eleitor candidato "         <<  getId()
                    << " votando em si mesmo: "     << getId()
                    << endl;
        }

        const int getLimpo() const
        {
            return limpo;
        }

        void operator++() { votos++; votos_total++; }

        const bool getEleito() const
        {
            return ( votos > (int)(votos_total/2) ) ? true : false;
        }
};

int Candidato::votos_total(0);

void Eleitor::votar()
{
    if (cands.empty())
    {
        cout << "O eleitor n�o conhece nenhum candidato! " << endl;
        return;
    }

    set<Candidato*>::iterator iterador;

    int cont = 10;

    do{
        iterador = cands.begin();
        while ( iterador != cands.end() )
        {
            if ( *iterador )
            {
                if ( alienado )
                {
                    if ((rand()%100) <= cont)
                    {
                        (*iterador)->operator++();

                        cout << "Eleitor alienado " <<  getId()
                             << " votando em: "     << (*iterador)->getId()
                             << endl;
                        return;
                    }
                    else
                    {
                        cout << "Indecis�o do eleitor alienado id: " << getId() << endl;
                    }
                    cont+=10;
                }
                else
                {
                    if ( (rand()%100) <= (*iterador)->getLimpo() )
                    {
                        (*iterador)->operator++();

                         cout << "Eleitor conciente " <<  getId()
                             << " votando em: "       << (*iterador)->getId()
                             << endl;
                        return;
                    }
                    else
                    {
                        cout << "Indecis�o do eleitor conciente id: " << getId() << endl;
                    }
                }
            }
            else
            {
                cout << "Ponteiro nulo em candes!" << endl;
                return;
            }
            iterador++;
        }
    } while (true);
};

class Eleicao
{
    private:
        vector<Eleitor*>    eleitores;
        vector<Candidato*>  candidatos;
        vector<Partido*>    partidos;

    public:

        Eleicao():
            eleitores(),
            candidatos(),
            partidos()
        {
            eleitores.clear();
            candidatos.clear();
            partidos.clear();

            criarPartidos();
            criarEleitores();
            criarCandidatos();
            executar();
        }

        ~Eleicao()
        {
            for (int i = 0; i < (int)eleitores.size(); i++)
                if (eleitores[i]) delete eleitores[i];

            /*for (int i = 0; i < (int)candidatos.size(); i++)
                if (candidatos[i]) delete candidatos[i];*/

            for (int i = 0; i < (int)partidos.size(); i++)
                if (partidos[i]) delete partidos[i];

            eleitores.clear();
            candidatos.clear();
            partidos.clear();
        }

        // ----------------------------------

        // poderia ser um solu��o bem mais simples criando dois partidos,
        // associando depois a eles candidatos e eleitores.

        void criarPartidos()
        {
            const int max = 2;
            Partido* p = NULL;

            for (int i = 0; i < max; i++)
            {
                p = new Partido();
                if (p)
                {
                    partidos.push_back(p);
                }
                else
                {
                    cout << "Partido n�o alocado!" << endl;
                }
            }
            cout << "Fim criacao partidos!" << endl;
        }

        // ----------------------------------


        void criarEleitores()
        {
            const int max = 20;
            Eleitor* p = NULL;

            int cont = 0;
            int npart = (int)partidos.size();
            int j = 0;

            for (int i = 0; i < max; i++)
            {
                p = new Eleitor();
                if (p)
                {
                    eleitores.push_back(p);

                    if ( cont > max/(npart*2) )
                    {
                        cont = 0;
                        j++;
                    }

                    if ( j < npart )
                    {
                        partidos[j]->filiar(p);
                        cont++;
                    }
                }
                else
                {
                    cout << "Eleitor n�o alocado!" << endl;
                }
            }

            cout << "Fim criacao eleitores!" << endl;
        }

        // ----------------------------------
        // poderia ser um solu��o bem mais simples criando tr�s candidatos na m�o,
        // associando-os depois aos eleitores.

        void criarCandidatos()
        {
            const int max = 3;
            Candidato* p = NULL;

            int cont    = 0;
            int npart   = (int)partidos.size();
            int k       = 0;

            for (int i = 0; i < max; i++)
            {
                p = new Candidato();
                if (p)
                {
                    candidatos.push_back(p);
                    for (int j = 0; j < (int)eleitores.size(); j++)
                    {
                        eleitores[j]->considerar(p);
                    }

                    if ( cont > max/(npart*2) )
                    {
                        cont = 0;
                        k++;
                    }

                    if (k < npart)
                    {
                        partidos[k]->filiar(p);
                        cont++;
                    }
                }
                else
                {
                    cout << "Candidato n�o alocado!" << endl;
                }
            }

            for (int i = 0; i < max; i++)
            {
                eleitores.push_back(candidatos[i]);
            }

            cout << "Fim criacao candidatos!" << endl;
        }


        void executar()
        {
            //cout << "M�todo executar de Intelig�ncia!" << endl << endl;
            for (int i = 0; i < (int)partidos.size(); i++ )
            {
                partidos[i]->politizar();
            }

            for (int i = 0; i < (int)eleitores.size(); i++ )
            {
                eleitores[i]->votar();
            }

            for (int i = 0; i < (int)candidatos.size(); i++ )
            {
                if (candidatos[i]->getEleito())
                {
                    cout << "O seguinte candidato foi eleito, de id: " << candidatos[i]->getId() << endl;
                }
                else
                {
                    cout << "O seguinte candidato N�O foi eleito, de id: " << candidatos[i]->getId() << endl;
                }
            }
        }
};

int main ()
{
    //cout << "Funcao main!" << endl;

    time_t t;   // https://pt.wikipedia.org/wiki/Time_t
                // Para fun��o srand e rand vide: https://www.tutorialspoint.com/c_standard_library/c_function_srand.htm
    srand((unsigned) time(&t));

    Eleicao e;

    return 0;
}

