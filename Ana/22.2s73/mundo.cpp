#include <list>

#include <iostream>
using namespace std;

#include <stdlib.h>
#include <time.h>

namespace Essenciais
{
    class Nacao
    {
        private:
            int poderio;

        public:
            Nacao(): poderio(rand()%101){ }
            virtual ~Nacao(){ poderio = -1;}
            const int getPoderio() const{ return poderio;}
            void setPoderio(const int pod){ poderio = pod;}
            void operator++(int){poderio++;}
            virtual const int guerrear()
            {
                (*this)++;
                return poderio;
            }
    };

    class Invasora: public Nacao
    {
        private:
            bool crime_guerra;
            const int RESERVA;

        public:
            Invasora(): Nacao(), crime_guerra((bool)(rand()%100)), RESERVA(100){ }
            ~Invasora(){ }
            const int guerrear(){ return (RESERVA*getPoderio());}
            void operator--()
            {
                if(crime_guerra)
                    setPoderio(getPoderio()-10);
                else
                    setPoderio(getPoderio()-1);
            }
    };
} // namespace Essenciais

using namespace Essenciais;

namespace Principais
{
    class Conjunto
    {
        private:
            list<Nacao*> nacoes_unidas;

        public:
            Conjunto(const int num = 100): nacoes_unidas()
            {
                nacoes_unidas.clear();
                //list<Nacao*>::iterator it;
                Nacao* pNac = NULL;

                for(int i=0; i<num; i++)
                {
                    pNac = new Nacao();

                    if(pNac)
                        nacoes_unidas.push_back(pNac);
                }
            }

            ~Conjunto()
            {
                if(!nacoes_unidas.empty())
                {
                    list<Nacao*>::iterator it;

                    for(it=nacoes_unidas.begin(); it!=nacoes_unidas.end(); it++)
                    {
                        if(*it)
                            delete(*it);
                    }
                    nacoes_unidas.clear();
                }
            }

            const int calc_soma()
            {
                int soma = 0;
                if(!nacoes_unidas.empty())
                {
                    
                    list<Nacao*>::iterator it;

                    for(it=nacoes_unidas.begin(); it!=nacoes_unidas.end(); it++)
                    {
                        if(*it)
                            soma+=(*it)->guerrear();
                    }
                }

                return soma;
            }
    };
} // namespace Principais

using namespace Principais;

namespace Essenciais
{
    class Invadida: public Nacao
    {
        private:
            Invasora* pInvas;
            Conjunto* pAliados;

        public:
            Invadida(Invasora* pI = NULL, Conjunto* pC = NULL): Nacao(), pInvas(pI), pAliados(pC){ }
            ~Invadida()
            {
                pInvas = NULL;
                pAliados = NULL;
            }
            const int guerrear()
            {
                (*this)++;

                if(pInvas)
                    pInvas->operator--();

                if(pAliados)
                    return (pAliados->calc_soma() + getPoderio()); 

                return getPoderio();               
            }
    };
} // namespace Essenciais

using namespace Essenciais;

namespace Principais
{
    class Mundo
    {
        private:
            Conjunto aliados;
            Invasora invas;
            Invadida invad;

        public:
            Mundo(): aliados((rand()%100)+1), invas(), invad(&invas, &aliados)
            {
                guerrear();
            }
            ~Mundo(){ }
            void guerrear()
            {
                int pod_invad = 0, pod_invas = 1;

                while(pod_invad < pod_invas)
                {
                    pod_invas = invas.guerrear();
                    pod_invad = invad.guerrear();
                }

                cout << "Nacao " << &invad << " foi libertada!" << endl;
            }
    };
} // namespace Principais

using namespace Principais;

int main()
{
    srand(time(NULL));
    Mundo m;
    return 0;
}