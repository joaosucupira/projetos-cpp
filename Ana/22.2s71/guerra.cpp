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
            Atomica(const int mgtns = 5): megatons(mgtns){ potencial_atomico += megatons;}
            virtual ~Atomica(){ megatons = -1;}
            const int getMegatons() const{ return megatons;}
            static int getPotencialAtomico(){ return potencial_atomico;}
            virtual bool explodir() = 0; //função virtual pura que torna a classe abstrata
    };

    int Atomica::potencial_atomico(0);

    class Fusao; //pré-definição da classe Fusao

    class Fissao: public Atomica
    {
        private:
            int potencial;
            Fusao* pFusao;

        public:
            Fissao(Fusao* pFs = NULL, const int mgtns = 15): Atomica(mgtns), potencial(-1), pFusao(pFs)
            {
                srand(time(NULL));
                potencial = rand()%101;
            }  

            ~Fissao(){ potencial = -1; pFusao = NULL;}
            bool explodir(); //implementada posteriormente devido a relação de dependencia com a classe Fusao
    };

    class Fusao: public Atomica
    {
        private:
            Fissao detonadora;
            bool duplicador;

        public:
            Fusao(const int mgtns = 100): Atomica(mgtns), detonadora(this, mgtns), duplicador(false){ }
            ~Fusao(){ }
            void setDuplicador(const bool dp){ duplicador = dp;}
            const int getMegatons() const
            {
                if(duplicador)
                    return (2*megatons);

                return megatons;
            }
            
            bool explodir(){ return (detonadora.explodir());}
    };

    bool Fissao::explodir()
    {
        if(potencial>77)
        {
            if(pFusao)
            {
                pFusao->setDuplicador(true);
                return true;
            }

            cout << "Ptr nulo!" << endl;
            return true;
        }
        
        if(potencial>33)
            return true;

        return false;
    }
} // namespace Bombas

using namespace Bombas;

class Arsenal
{
    private:
        vector<Fusao*> colecao;

    public:
        Arsenal(const int nBombas = 100): colecao()
        {
            colecao.clear();

            Fusao* pF = NULL;

            for(int i=0; i<nBombas; i++)
            {
                srand(i*(int) time(NULL));
                pF = new Fusao((rand()%100)+1);

                if(pF)
                    colecao.push_back(pF);
                else
                    cout << "Falha na alocação de Fusao!" << endl;
            }
        }

        ~Arsenal()
        {
            int tam = (int) colecao.size();

            for(int i=0; i<tam; i++)
            {
                if(colecao[i])
                    delete colecao[i];
                else
                    cout << "Ptr nulo!" << endl;
            }

            colecao.clear();
        }

        const int getTotalMegatons() const
        {
            int total_mgtns = 0;

            int tam = (int)colecao.size();

            for(int i=0; i<tam; i++)
            {
                if(colecao[i])
                {
                    if(colecao[i]->explodir())
                        total_mgtns += (colecao[i]->getMegatons());
                }
                else
                    cout << "Ptr nulo!" << endl;
            }

            return total_mgtns;
        }
};

class Nacao
{
    private:
        static const int MAX;
        bool atacar;
        Nacao* pBeligerante;
        Arsenal arsenal;

    public:
        Nacao(): atacar(false), pBeligerante(NULL), arsenal(rand()%101)
        {
            srand(time(NULL));
            if(!((bool)rand()%100))
                atacar = true;
        }

        ~Nacao(){ pBeligerante = NULL;}
        const bool getAtacar() const{ return atacar;}

        void informar_dano() const
        {
            int total_mgtns = arsenal.getTotalMegatons();

            if(pBeligerante)
                total_mgtns += pBeligerante->arsenal.getTotalMegatons();
            else    
                cout << "Não ha adversario beligerante!" << endl;

            if(total_mgtns > MAX)
                cout << "Fim do mundo!" << endl;
            else if(total_mgtns>(MAX/2))
                cout << "Inverno nuclear!" << endl;
            else
                cout << "Planeta danificado!" << endl;
        }

        void defender()
        {
            if(pBeligerante)
            {
                if(pBeligerante->getAtacar())
                    informar_dano();
                else
                    cout << "Ha paz e o planeta esta a salvo!" << endl;
            }
            else
                cout << "Não ha adversario beligerante!" << endl;
        }

        void operator=(Nacao* pBelig)
        {
            if(pBelig)
                pBeligerante = pBelig;
            else
                cout << "Ptr nulo!" << endl;
        }
};

const int Nacao::MAX(1000);

int main()
{
    Nacao A, B;

    int pot = Atomica::getPotencialAtomico();

    cout << "Potencial atomico geral eh de " << pot << " megatons!" << endl;

    A = &B;

    A.defender();
    B.defender();

    return 0;
}