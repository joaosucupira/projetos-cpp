/*Solução desenvolvida por Ana Julia Molinos*/

#include <list>

#include<iostream>
using namespace std;

#include <stdlib.h>
#include <time.h>

class Ente
{
    protected:
        int x;
        int y;
        static int cont;

    public:
        Ente(const int xx=0, const int yy=0): x(xx), y(yy){ }

        virtual ~Ente(){ x = -1; y = -1;}

        virtual const int getX() const{ return x;}

        virtual const int getY() const{ return y;}

        virtual const bool mover(Ente* pE) = 0;
};

int Ente::cont(35);

class Carro: public Ente
{
    private:
        const int y;
        static const int MAXVEL;
        int velocidade;
        int multas;

    public:
        Carro(const int xx = 0): y(5), Ente(xx, y), velocidade(++cont), multas(0){ }

        ~Carro(){ multas = -1;}

        const bool mover(Ente* pE)
        {
            if(pE)
            {
                x += velocidade;
                cout << "Carro " << this << " moveu!" << endl;

                if(x==pE->getX() && y==pE->getY() && velocidade > MAXVEL)
                    return true;
                
                return false;
            }

            cout << "Ptr nulo!" << endl;
            exit(1);
        }

        const bool frenar()
        {
            cout << "Carro " << this << " frenou!" << endl;

            if(velocidade<=MAXVEL)
            {
                velocidade = 0;
                return true;
            }

            velocidade /= 2;
            return false;
        }

        const int getY() const{ return y;}

        void operator++(int){ multas++; cout << "Carro " << this << " foi multado!" << endl;}

        const int getMultas() const{ return multas;}
};

const int Carro::MAXVEL = 40;

class Pedestre: public Ente
{
    private:
        const int x;
        bool guarda;

    public:
        Pedestre(const int yy = 0): x(3), Ente(x, yy), guarda((bool)(rand()%100==1)){ }

        ~Pedestre(){ }

        const bool mover(Ente* pE)
        {
            if(pE)
            {
                cout << "Pedestre " << this << " moveu!" << endl;

                Carro* car = static_cast<Carro*>(pE);
                y++;
                bool mover = car->mover(static_cast<Ente*>(this));

                if(car->getX()==x && car->getY()==y && mover && guarda)
                {
                    (*car)++; //multa
                    if(!(car->frenar()))
                        return true;
                }

                return false;
            }

            cout << "Ptr nulo!" << endl;
            exit(1);
        }

        const int getX() const{ return x;}
};

class Conjunto
{
    private:
        list<Ente*> coletivo;
        list<Ente*>::iterator it;

    public:
        Conjunto(): coletivo(), it(){ coletivo.clear();}
        ~Conjunto(){ coletivo.clear();}
        
        void incluir(Ente* pE)
        {
            if(pE)
                coletivo.push_back(pE);
            else
                cout << "Ptr nulo!" << endl;
        }

        void irParaInicio(){ it = coletivo.begin();}

        void operator++(int){ it++;}

        const bool fim() const{ return ((bool)(it==coletivo.end()));}

        Ente* getAtual(){ return (*it);}
};

class Transito
{
    private:
        Conjunto pedestres;
        Conjunto carros;

    public:
        Transito(): pedestres(), carros()
        {
            //inicializa pedestres
            const static int N_PEDESTRES = 50;
            Pedestre* ped;

            for(int i=0; i<N_PEDESTRES; i++)
            {
                srand(i*(int)time(NULL));
                ped = new Pedestre(rand()%11);
                //cout << ped->getX() << " " << ped->getY() << endl;
                pedestres.incluir(static_cast<Ente*>(ped));
            }

            //inicializa carros
            const static int N_CARROS = 10;
            Carro* car;

            for(int i=0; i<N_CARROS; i++)
            {
                srand(i*(int)time(NULL));
                car = new Carro(rand()%11);
                //cout << car->getX() << " " << car->getY() << " " << endl;
                carros.incluir(static_cast<Ente*>(car));
            }

            transitar();
        }

        ~Transito()
        {
            //desaloca pedestres
            pedestres.irParaInicio();

            while (!pedestres.fim())
            {
                delete(pedestres.getAtual());
                pedestres++;
            }
            
            //desaloca carros
            carros.irParaInicio();

            while (!carros.fim())
            {
                delete(carros.getAtual());
                carros++;
            }          
        }

        void transitar()
        {
            Ente* ped, *car;
            bool acidente, parar = false;

            for (int i=0; i<10 && !parar; i++)
            {
                pedestres.irParaInicio();

                while (!pedestres.fim() && !parar)
                {
                    ped = pedestres.getAtual();

                    carros.irParaInicio();

                    while (!carros.fim() && !parar)
                    {
                        car = carros.getAtual();
                        acidente = ped->mover(car);

                        if(acidente)
                            {parar = true; cout << "Houve acidente!" << endl;}

                        carros++;
                    }

                    pedestres++;
                }
            }

            if(acidente)
            {
                carros.irParaInicio();

                Carro* temp;
                while (!carros.fim())
                {
                    temp = static_cast<Carro*>(carros.getAtual());

                    cout << "Carro " << temp << " possui " << temp->getMultas() << " multas." << endl;

                    carros++;
                }
            }
            
        }
};

int main()
{
    Transito t;

    return 0;
}