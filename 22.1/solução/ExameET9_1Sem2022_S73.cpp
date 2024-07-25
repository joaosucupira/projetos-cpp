// Prof Simão - DAINF/UTFPR - Ctba - TecProg S73 - 1 Sem 2022 - 1a Parcial
// Hah couts suplementares para fins de entedimento da execução.

#include <stdlib.h>
#include <time.h>
#include <list>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

class Hormonio
{
    protected:

        string nivel;
        bool   estresse;

    private:
        static int soma;

    public:
        Hormonio(): nivel ("indeterminado")//, estresse(soma % 2==0?true:false)
        {
             //estresse = !((bool)(soma % 2));
             if ( soma % 2 == 0 )
             {
                 estresse = true;
             }
             else
             {
                 estresse = false;
             }
             soma++;
        }

        virtual ~Hormonio()
        {            //soma--;
        }

        const string getNivel() const
        {
            return nivel;
        }

        virtual void calcular_nivel() = 0;
};

int Hormonio::soma = 0;


class Endorfina;

class Serotonina : public Hormonio
{
    private:
        int inteiro;
        Endorfina* pEndo;

    public:

        Serotonina ()
            : Hormonio(), inteiro(0), pEndo(NULL)
        {
            time_t t;     // https://pt.wikipedia.org/wiki/Time_t

            // Para função srand e rand vide:
            // https://www.tutorialspoint.com/c_standard_library/c_function_srand.htm
            srand((unsigned) time(&t));
            inteiro = rand()%11;

            if ( (estresse) && (inteiro > 0) )
            {
                inteiro--;
            }
        }

        ~Serotonina ()
        {
            pEndo = NULL;
        }

        void setEndo(Endorfina* pE)
        {
            pEndo = pE;
        }

        const int getInteiro() const
        {
            return inteiro;
        }

        void calcular_nivel(); // método é implementado depois pois depende de Endorfina
};

class Endorfina : public Hormonio
{
    private:
        float   real;
        Serotonina* pSero;

    public:
        Endorfina() :
            Hormonio(), real(0.0), pSero(NULL)
        {
            time_t t;     // https://pt.wikipedia.org/wiki/Time_t

            // Para função srand e rand vide:
            // https://www.tutorialspoint.com/c_standard_library/c_function_srand.htm
            srand(3*(unsigned) time(&t));

            real = static_cast<float>(rand()%11);
        }

        ~Endorfina()
        {
            pSero = NULL;
        }

        void setSero(Serotonina* pS)
        {
            pSero = pS;
        }

        const float getReal() const
        {
            return real;
        }

        void calcular_nivel()
        {
            if (pSero == NULL)
            {
                cout << endl << "Erro - pSero nulo" << endl;
                return;
            }

            if ( ( real >= 6.0  ) &&
                 ( real <= 10.0 ) &&
                 ( pSero->getInteiro() > 7 )
               )
            {
                nivel = "normal";
                cout << endl << "Endorfina: " << nivel << endl;
            }
            else
            {
                if ( ( real > 6.0 ) &&
                     ( real < 3.0 ) &&
                     ( pSero->getInteiro() > 5)
                   )
                {
                    nivel = "medio";
                    cout << endl << "Endorfina: " << nivel << endl;
                }
                else
                {
                    if ( ( real <= 3.0 ) &&
                         ( real >= 0.0 ) &&
                         ( pSero->getInteiro() > 0 )
                       )
                    {
                        nivel = "baixo";
                        cout << endl << "Endorfina: " << nivel << endl;
                    }
                    else
                    {
                        cout << endl << "Endorfina: " << nivel << endl;
                    }
                }
            }
        }
};

void Serotonina::calcular_nivel()
{
    if (pEndo == NULL)
    {
        cout << endl << "Erro - pEndo nulo" << endl;
        return;
    }

    if ( ( inteiro >= 7  ) &&
         ( inteiro <= 10 ) &&
         ( pEndo->getReal() > 5.0 )
       )
    {
        nivel = "normal";
        cout << endl << "Serotonina: " << nivel << endl;
    }
    else
    {
        if ( ( inteiro > 4  ) &&
             ( inteiro < 7  ) &&
             ( pEndo->getReal() > 3.0)
           )
        {
            nivel = "medio";
            cout << endl << "Serotonina: " << nivel << endl;
        }
        else
        {
            if ( ( inteiro <= 4 ) &&
                 ( inteiro >= 0 ) &&
                 ( pEndo->getReal() > 0.0 )
               )
            {
                nivel = "baixo";
                cout << endl << "Serotonina: " << nivel << endl;
            }
            else
            {
                cout << endl << "Serotonina: " << nivel << endl;
            }
        }
    }
}

class Reg_Paciente
{
    private:
        Endorfina   nivel_endo;
        Serotonina  nivel_sero;
        bool        ok;


    public:
            Reg_Paciente():
                nivel_endo(),
                nivel_sero(),
                ok (false)
            {
                nivel_endo.setSero(&nivel_sero);

                nivel_sero.setEndo(&nivel_endo);

                nivel_endo.calcular_nivel();

                nivel_sero.calcular_nivel();
            }

            ~Reg_Paciente()
            {
            }

            const int getOK() const
            {
                return (int)ok;
            }

            void auto_avaliar()
            {
                if  ( ( ( nivel_endo.getNivel() == "medio" ) || (nivel_endo.getNivel() == "normal" ) ) &&
                      ( ( nivel_sero.getNivel() == "medio" ) || (nivel_sero.getNivel() == "normal" ) )
                    )
                {
                    ok = true;
                    cout << "RegPaciente é true" << endl;
                }
                else
                {
                    ok = false;
                    cout << "RegPaciente é false" << endl;
                }
            }
    };


class Diagnosticador
{
    private:

        list<Reg_Paciente*> listap;

        ostringstream       saida;

    public:
        Diagnosticador(const int num = 10):
            listap(),
            saida()
        {
            listap.clear();

            Reg_Paciente* pR = NULL;

            for (int i = 0; i < num; i++)
            {
                pR = new Reg_Paciente();
                if (pR)
                {
                    listap.push_back(pR);
                }
                else
                {
                    cout << endl << "Problema de Alocação de memória!" << endl;
                }
                pR = NULL;
            }
            diagnosticar();
        }

        ~Diagnosticador()
        {
            Reg_Paciente* pR = NULL;

            list<Reg_Paciente*>::iterator it;

            it = listap.begin();

            while (it != listap.end())
            {
                pR = *it;

                if (pR)
                {
                  delete pR;
                  pR = NULL;
                }
                it++;
            }
            listap.clear();
        }

        ostringstream& getSaida()
        {
            return saida;
        }


        void diagnosticar()
        {
            Reg_Paciente* pR = NULL;

            list<Reg_Paciente*>::iterator it;

            it = listap.begin();

            int cont = 0;

            while (it != listap.end())
            {
                pR = *it;

                if (pR)
                {
                  pR->auto_avaliar();

                  if (pR->getOK() != 0)
                  {
                    cont++;
                  }
                }
                it++;
            }

            int tam = (int)listap.size();

            float calc = static_cast<float>((cont*100)/tam);

            //string s ("A porcentagem de pacientes OK eh de ");
            saida << endl << "A porcentagem de pacientes OK eh de " << calc << endl;

            //cout << saida.str() << endl;
        }
};

ostream& operator<<( ostream& s, Diagnosticador& d)
{
    cout << (d.getSaida()).str();
    return s ;
}

int main ()
{
   Diagnosticador diag1(100);

   cout << diag1 << endl;

   return 0;
}




