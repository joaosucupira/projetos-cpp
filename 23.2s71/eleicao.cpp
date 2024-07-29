/*solução implementada por Ana Julia Molinos*/
#include <list>
#include <vector>
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
            Agente(const int idd = 21): id(++cont_id), idade(idd)
            {
                if(idd<21){ idade = 21;}
                else if(idd>59){ idade = 59;}
            }

            virtual ~Agente(){ id = -1; idade = -1; cont_id--;}

            virtual void agir() = 0;

            const int getIdade() const{ return idade;}

            const int getId() const{ return id;}

            void operator++(int){ idade++;}
    };

    int Agente::cont_id = 0;

    class Secreto;

    class Espiao: public Agente
    {
        private:
            list<Secreto*> secres;
            bool boco;

        public:
            Espiao(const int idd = 21): Agente(idd), secres(), boco((bool)(1==rand()%10))
            { secres.clear();}

            ~Espiao(){ secres.clear();}
            
            //implementado posteriormente devido a dependencia da implementação da classe Secreto
            void agir(); 

            void incluirSecreto(Secreto* p)
            {
                if(p)
                { 
                    secres.push_back(p);
                    cout << "Agente secreto incluido!" << endl;
                }
                else
                    cout << "Ptr nulo!" << endl;
            }

            void setBoco(const bool b){ boco = b;}

            const bool getBoco() const{ return boco;}
    };

    class Secreto: public Agente
    {
        protected:
            vector<Espiao*> espias;
            float força;

        public:
            Secreto(const int idd = 21): Agente(idd), espias(), força(100.0-idade)
            { espias.clear();}

            virtual ~Secreto(){ espias.clear(); força = -1.0;}

            virtual void agir()
            {
                vector<Espiao*>::iterator it = espias.begin();
                int cont = 0;

                /*percorre o vetor de espiões removendo os bocós até alcançar a
                  quantidade equivalente a sua força*/
                while (cont<força && it!=espias.end())
                {
                    if((*it)->getBoco()) 
                    { 
                        espias.erase(it);
                        cont++;
                        cout << "Boco id " << (*it)->getId() << " removido!" << endl;
                    }
                    it++;
                }
            }

            const float getForça() const{ return força;}

            virtual void incluirEspiao(Espiao* p)
            {
                if(p)
                {
                    espias.push_back(p);
                    cout << "Espiao incluido!" << endl;
                }
                else{ cout << "Ptr nulo!" << endl;}
            }

            void operator--(int){ força--;}
    };

    void Espiao::agir()
    {
        bool flag_parar = false;
        Secreto* ag_sec;
        list<Secreto*>::iterator it = secres.begin();

        while(it!=secres.end() && !flag_parar)
        {
            ag_sec = (*it);
            (*ag_sec)--;

            if (ag_sec->getForça() < 0)
            {
                secres.remove(ag_sec);
                flag_parar = true;
            }
            it++;
        }
    }

    class Duplo: public Secreto
    {
        private:
            Espiao* pContato;

        public:
            Duplo(const int idd = 21): Secreto(idd), pContato(NULL){ }
            ~Duplo(){ pContato = NULL;}

            void agir()
            {
                if(pContato){ pContato->setBoco(false);}
                else{ cout << "Contato nulo!" << endl;}
            }

            void incluirEspiao(Espiao* p)
            {
                if(p)
                {
                    pContato = p;
                    cout << "Espiao incluido!" << endl;
                }
                else{ cout << "Ptr nulo!" << endl;}
            }
    };
} // namespace agentes

using namespace agentes;

#define N_AGSECRETO 5
#define N_AGESPIAO 4
#define N_AGDUPLO 2

class Inteligencia
{
    private:
        set<Agente*> colecao;

    public:
        Inteligencia(): colecao()
        {
            srand(time(NULL));

            colecao.clear();
            criarAgentes();
            executar();
        }

        ~Inteligencia()
        {
            set<Agente*>::iterator it;

            for(it=colecao.begin(); it!=colecao.end(); it++)
                delete (*it);

            colecao.clear();
        }

        void criarAgentes()
        {
            //criar ags. secretos
            vector<Secreto*> secs;
            int cont = 0;
            Secreto* ag_sec;

            while(cont<N_AGSECRETO)
            {
                srand(cont * time(NULL));
                ag_sec = new Secreto(rand()%100);
                colecao.insert(static_cast<Agente*>(ag_sec));
                secs.push_back(ag_sec);
                cont++;
            }

            //criar ags. duplos
            vector<Duplo*> dups;
            cont = 0;
            Duplo* ag_dup;

            while(cont<N_AGDUPLO)
            {
                ag_dup = new Duplo(rand()%100);
                colecao.insert(static_cast<Agente*>(ag_dup));
                dups.push_back(ag_dup);
                cont++;
            }

            //criar ags. espioes
            vector<Espiao*> esps;
            cont = 0;
            Espiao* ag_esp;

            while(cont<N_AGESPIAO)
            {
                ag_esp = new Espiao(rand()%100);
                colecao.insert(static_cast<Agente*>(ag_esp));
                esps.push_back(ag_esp);
                cont++;
            }

            //associar ags. secretos a espioes(1) e espioes a ags. secretos(2)
            vector<Espiao*>::iterator it_esp;
            vector<Secreto*>::iterator it_sec;

            for(it_esp=esps.begin(); it_esp!=esps.end(); it_esp++)
            {
                for(it_sec=secs.begin(); it_sec!=secs.end(); it_sec++)
                {
                    (*it_esp)->incluirSecreto(*it_sec);//(1)
                    (*it_sec)->incluirEspiao(*it_esp);//(2)
                }
            }

            //associar a cada agente duplo um espião
            int tam_dups = (int)dups.size(),
                tam_esps = (int)esps.size();
            
            for(int i=0; i<tam_dups && i<tam_esps; i++)
                dups[i]->incluirEspiao(esps[i]);
            
        }

        void executar()
        {
            set<Agente*>::iterator it;
            for(it=colecao.begin(); it!=colecao.end(); it++)
            {
                cout << "Agente " << (*it)->getId() << " idade: " << (*it)->getIdade() << endl;
                (*it)->agir();
                (*(*it))++;
            }
        }

};

int main()
{
    Inteligencia intel;

    return 0;
}