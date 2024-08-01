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
        Inseto(const int energ = 0): energia(energ){ }

        ~Inseto(){ energia = -1;}

        const int getEnergia(){ return energia;}

        virtual void sobreviver() = 0;

        void operator--(int){ energia--;}
};

class Gafanhoto;

class Formiga: public Inseto
{
    private:
        static Gafanhoto* pGafa;
        static int cont;
        int ferrao;

    public:
        Formiga(const int energ = 1): Inseto(energ), ferrao(cont++){ }
        
        ~Formiga(){ ferrao = -1;}

        void sobreviver(); //implemetação posterior devido a relação de dependencia com a classe gafanhoto

        static void setGafa(Gafanhoto* pG){ pGafa = pG;}
};

Gafanhoto* Formiga::pGafa(NULL);
int Formiga::cont(1);

class Formigueiro;

class Gafanhoto: public Inseto
{
    private:
        const bool invasor;
        static Formigueiro* pGueiro;

    public:
        Gafanhoto(const int energ = 10): Inseto(energ), invasor((bool)(rand()%2)){ }

        ~Gafanhoto(){ }

        const bool getInvasor() const{ return invasor;}

        static void setFormigueiro(Formigueiro* p){ pGueiro = p;}

        void sobreviver();
};

Formigueiro* Gafanhoto:: pGueiro(NULL);

void Formiga::sobreviver()
{
    int n = ferrao;

    if(pGafa->getInvasor())
        n *= 2;

    for(int i=0; i<n; i++)
        (*pGafa)--;

    (*this)--;

    cout << "Formiga " << this << " sobreviveu!" << endl;
}

class Formigueiro
{
    private:
        vector<Formiga*> colecao;

    public:
        Formigueiro(const int nFormigas = 100): colecao()
        {
            colecao.clear();

            Formiga* miga;

            for(int i=0; i<nFormigas; i++)
            {
                srand(i*(int)time(NULL));
                miga = new Formiga((rand()%100)+1);

                if(miga)
                    colecao.push_back(miga);
            }
        }

        ~Formigueiro()
        {
            if(!colecao.empty())
            {
                int tam = (int)colecao.size();

                for(int i=0; i<tam; i++)
                {
                    if(colecao[i])
                        delete (colecao[i]);
                }

                colecao.clear();
            }
        }

        void eliminar_ultima()
        {
            cout << "Formiga " << colecao.back() << " eliminada do formigueiro!" << endl;
            
            if(!colecao.empty())
            {
                delete(colecao.back());
                colecao.pop_back();
            }
        }

        void eliminar_sem_energia()
        {
            vector<Formiga*>::iterator it = colecao.begin();
            Inseto* pInseto;

            while(it!=colecao.end())
            {
                if(*it)
                {
                    pInseto = static_cast<Inseto*>(*it);

                    if(pInseto->getEnergia()<=0)
                    {
                        cout << "Formiga " << (*it) << " eliminada!" << endl;
                        delete(*it);
                        colecao.erase(it);
                    }
                }

                it++;
            }
        }

        void defender(Gafanhoto* pGafa)
        {
            Formiga::setGafa(pGafa);
            Inseto* gafanhoto = static_cast<Inseto*>(pGafa);
            int tam;

            while(!colecao.empty() && gafanhoto->getEnergia()>0)
            {
                tam = colecao.size();

                for(int i=0; i<tam; i++)
                    colecao[i]->sobreviver();

                eliminar_sem_energia();
            }
        }
};

void Gafanhoto::sobreviver()
{
    pGueiro->eliminar_ultima();
    pGueiro->defender(this);

    cout << "Gafanhoto " << this << " sobreviveu!" << endl;
}

class Habitat
{
    private:
        const int num_gafs;
        Formigueiro formigueiro;
        list<Gafanhoto*> colecao;

    public:
        Habitat(): num_gafs(10), formigueiro(rand()%1001), colecao()
        {
            criarGafanhotos();

            executar();
        }

        ~Habitat()
        {
            list<Gafanhoto*>::iterator it;

            for(it=colecao.begin(); it!=colecao.end(); it++)
                delete(*it);

            colecao.clear();
        }

        void criarGafanhotos()
        {
            Gafanhoto* gaf;

            for(int i=0; i<num_gafs; i++)
            {
                srand(i*(int)time(NULL));
                gaf = new Gafanhoto((rand()%100)+1);

                if(gaf)
                {
                    colecao.push_back(gaf);
                    //gaf->setFormigueiro(&formigueiro);
                }
            }

            Gafanhoto::setFormigueiro(&formigueiro);
        }

        void executar()
        {
            list<Gafanhoto*>::iterator it;

            for(it = colecao.begin();it!=colecao.end();it++)
            {
                (*it)->sobreviver();
            }
        }
};

int main()
{
    srand(time(NULL));

    Habitat hab;

    return 0;
}