// Prof Simão - DAINF/UTFPR - Ctba - TecProg S71 - 2 Sem 2022 - 1a Parcial
// Hah couts suplementares para fins de entedimento da execução.

#include <vector>
#include <list>
#include <set>
#include <iostream>
using namespace std;

#include <stdlib.h>
#include <time.h>

namespace agentes
{
    class Agente
    {
        protected:
            static int cont_id;
            int id;
            int idade;

        public:

            Agente(const int idd = 21):
                id (cont_id++),
                idade(idd)
            {
                if (idade < 21) { idade = 21; }
                else { if (idade > 59) { idade = 59; } }
            }

            virtual ~Agente()
            {
                id = -1;
                idade = -1;
                //cont_id--;
            }

            virtual void agir() = 0;

            const int getIdade() const { return idade; }

            void operator++() { idade++; }

            //void setId(int i) { id = i; }
            const int getId() const { return id; }
    };
    int Agente::cont_id(0);

    class Secreto;

    class Espiao : public Agente
    {
        private:
            list<Secreto*> secres;
            bool bocoh;

        public:

            Espiao(const int idd = 21):
                Agente(idd),
                secres(),
                bocoh (true)
            {
                secres.clear();
                if ( 1 == (rand()%10) )
                {
                    bocoh = false;
                }
            }

            ~Espiao()
            {
                secres.clear();
            }

            void agir(); // a ser implementado depois da criação da classe Secreto dado a dependência.

            void incluirSecreto(Secreto* p)
            {
                if (p)
                {
                    secres.push_back(p);
                }
                else
                {
                    cout << "Nao se pode incluir nulo em secres!" << endl;
                }
            }

            const bool getBocoh() const
            {
                return bocoh;
            }

            void setBocoh(const bool b)
            {
                bocoh = b;
            }
    };

    class Secreto : public Agente
    {
        protected:
            vector<Espiao*>     espias;
            float               forca;

        public:
            Secreto(const int idd = 21) :
                Agente(idd),
                espias(),
                //forca(0)
                forca(100.0-(float)idd)
            {
                espias.clear();
            }

            virtual ~Secreto()
            {
                espias.clear();
            }

            virtual void agir()
            {
                if (espias.empty()) return;

                bool vai = false;
                do
                {
                    vai = false;
                    int tam = (int)espias.size();
                    int fim = -1;
                    if ((int)forca > tam)
                    {
                        fim = tam;
                    }
                    else
                    {
                        fim = (int)forca;
                    }

                    cout << "Entrei no loop principal!" << fim << endl;

                    for (int i=0; i<fim; i++)
                    {
                        if (espias[i] != NULL)
                        {
                            if (espias[i]->getBocoh())
                            {
                                //cout << endl;
                                cout << "Secreto excluindo de espias ID: " << espias[i]->getId() << endl;
                                espias.erase(espias.begin()+i);
                                // https://cplusplus.com/reference/vector/vector/erase/
                                vai = true;
                                break;
                            }
                        }
                        else
                        {
                            cout << "Ponteiro nulo em espias!" << endl;
                        }
                    }
                } while (vai);
            }

            virtual void incluirEspiao(Espiao* p)
            {
                if (p)
                {
                    espias.push_back(p);
                }
                else
                {
                    cout << "Nao se pode incluir nulo em espias!" << endl;
                }
            }

            const float getForca() const
            {
                return forca;
            }

            void operator--() { forca--; }
    };

    void Espiao::agir()
    {
            list<Secreto*>::iterator iterador;

            iterador = secres.begin();
            while ( iterador != secres.end() )
            {
                if ( *iterador )
                {
                    if ( (*iterador)->getForca() > 0.0 )
                    {
                        (*iterador)->operator--();
                        cout << "Diminuindo força de: " << *iterador << endl;
                    }
                    else
                    {
                        cout << "Espiao excluindo de secres: " << *iterador << endl;
                        secres.erase(iterador);
                        break;
                    }
                }
                else
                {
                    cout << "Ponteiro nulo em secres!" << endl;
                }
                iterador++;
            }
    }

    class Duplo : public Secreto
    {
        private:
            Espiao* pContato;

        public:
            Duplo(const int id = 21):
            Secreto(id),
            pContato(NULL)
            {
            }

            ~Duplo()
            {
                pContato = NULL;
            }

            void incluirEspiao(Espiao* p)
            {
                pContato = p;
            }

            void agir()
            {
                if (pContato)
                {
                    cout << "Desbocoziando: " << pContato << endl;
                    pContato->setBocoh(false);
                }
            }
    };
} using namespace agentes;

class Inteligencia
{
    private:
        set<Agente*> colecao;

    public:

        Inteligencia():
            colecao()
        {
            colecao.clear();
            criarAgentes();
            executar();
        }

        ~Inteligencia()
        {
            set<Agente*>::iterator it;
            it = colecao.begin();
            while (it != colecao.end())
            {
                cout << endl << "Deletando agente: " << (*it)->getId() << " --- " << *it << endl;

                if (*it)
                {
                    delete *it;
                }
                it++;
            }
            colecao.clear();
        }

        void criarAgentes()
        {
            const int max = 2;

            Secreto*    pS = NULL;
            Espiao*     pE = NULL;
            Duplo*      pD = NULL;

            vector<Secreto*>    vetSecretos;
            vector<Espiao*>     vetEspioes;
            //vector<Duplo*>      vetDuplos;

            //int cont = 0;
            for (int i = 0; i < max; i++)
            {
                pS = NULL;
                pE = NULL;
                pD = NULL;

                pS = new Secreto(i+21);
                //pS->setId(cont++);

                if (pS)
                {
                    vetSecretos.push_back(pS);
                    colecao.insert(static_cast<Agente*>(pS));
                    //cout << "Agente secreto inserido!" << endl;
                }
                else
                {
                    cout << "Agente secreto não alocado!" << endl;
                }
            }
                // ----------------------------------

            for (int i = 0; i < max; i++)
            {
                pE = new Espiao(i+31);
                //pE->setId(cont++);
                if (pE)
                {
                    vetEspioes.push_back(pE);
                    colecao.insert(static_cast<Agente*>(pE));
                }
                else
                {
                    cout << "Agente espião não alocado!" << endl;
                }

                // ----------------------------------
            }

            for (int i = 0; i < max; i++)
            {
                pD = new Duplo(i+41);
                if (pD)
                {
                    //vetDuplos.push_back(pD);
                    pD->incluirEspiao(vetEspioes[i]);
                    colecao.insert(static_cast<Agente*>(pD));
                }
                else
                {
                    cout << "Agente duplo não alocado!" << endl;
                }
            }

            for (int i = 0; i < max; i++)
            {
                for (int j = 0; j < max; j++)
                {
                    vetSecretos[i]->incluirEspiao(vetEspioes[j]);
                    vetEspioes[j]->incluirSecreto(vetSecretos[i]);
                }
            }
        }

        void executar()
        {
            //cout << "Método executar de Inteligência!" << endl << endl;
            set<Agente*>::iterator it;

            for (int i = 0; i < 10; i++)
            {
                it = colecao.begin();
                //int cont = 0;

                while (it != colecao.end())
                {
                    cout << endl << "Executando agente: " << (*it)->getId() << " --- " << *it << endl << endl;

                    if (*it)
                    {
                        (*it)->agir();
                        (*it)->operator++();
                    }
                    it++;
                    //cont++;
                }
            }
        }
};

int main ()
{
    //cout << "Funcao main!" << endl;

    time_t t;   // https://pt.wikipedia.org/wiki/Time_t
                // Para função srand e rand vide: https://www.tutorialspoint.com/c_standard_library/c_function_srand.htm
    srand((unsigned) time(&t));

    Inteligencia intel;

    return 0;
}

