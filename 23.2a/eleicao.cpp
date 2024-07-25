#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <set>
#include <list>
#include <vector>
using namespace std;

class Cidadao{
    protected:
        static int cont_id;
        int id;

    public:
        Cidadao(): id(++cont_id){ }
        virtual ~Cidadao(){ id = -1; cont_id--;}
        virtual void votar() = 0;
        virtual void desalienar() = 0;
        virtual const int getLimpo()const = 0;
        const int getId()const{return id;}
};

int Cidadao::cont_id = 0;

class Candidato;

class Eleitor: public Cidadao{
    protected:
        bool alienado;
        set<Candidato*> cands; //conjunto de candidatos
        const int limpo;

    public:
        //(bool)(id%2): ids pares recebem false e impares true, garantindo 50% de eleitores alienados
        Eleitor(): Cidadao(), alienado((bool)(id%2)), cands(),   limpo(100){ }
        virtual~Eleitor(){ cands.clear();}
        //implementado depois da implementação da classe candidato, devido a dependencia
        virtual void votar();
        virtual const int getLimpo()const{ return limpo;}
        void desalienar(){ alienado = false;}
        void considerar(Candidato* p){ cands.insert(p);}
};

class Candidato: public Eleitor{
    private:
        static int votos_total;
        int votos;
        int limpo;

    public:
        Candidato(): Eleitor(), votos(0), limpo(-1)
        {
            //multiplicando a semente pelo id do candidato, a fim de garantir maior aleatoriedade
            srand(id*(int)time(NULL));
            limpo = rand()%101;
        }

        ~Candidato(){ votos = -1; limpo = -1;}
        virtual void votar(){ (*this)++;}
        const int getLimpo()const{ return limpo;}
        void operator++(int){votos_total++; votos++;}
        const bool getEleito()const{ return (bool)(votos>(votos_total/2));}        
};

int Candidato::votos_total = 0;

//Da modelagem: o método votar se baseia nos seguintes critérios...
void Eleitor::votar()
{
    Candidato* voto;

    /*...Se o eleitor for alienado, ele tem chance de 10% de votar no primeiro candidato, 20%
        de votar no segundo e assim por diante. 
        (Na minha interpretação, como o último candidato do conjunto terá a maior chance, é 
        ele quem leva o voto)*/
    if(alienado)
    {
        //ptr voto recebe o ponteiro para o último candidato do conjunto
        voto = *(--cands.end()); 
        (*voto)++; //o voto é realizado por meio do incremento.
    }
    /*Se o eleitor não for alienado ele tem a mesma chance percentual do valor em atributo
        limpo do candidato avaliado.
        (Ou seja, ao percorrer o conjunto de candidatos considerados, o votado será o com maior
        valor no atributo limpo.)*/
    else 
    {
        set<Candidato*>::iterator it;
        voto = *(cands.begin());
        Candidato* cand;

        //procura o candidato com maior valor de Limpo
        for(it=++cands.begin(); it!=cands.end(); it++)
        {
            cand = (*it);
            if(cand->getLimpo()>voto->getLimpo())
                voto = (*it);
        }

        (*voto)++; //vota por meio do incremento
    }
}

class Partido{
    private:
        list<Cidadao*> filiados;

    public:
        Partido(): filiados(){ }
        ~Partido(){ filiados.clear();}
        void filiar(Cidadao* p){ filiados.push_back(p);}
        void desafiliar(Cidadao* p){ filiados.remove(p);}
        void politizar()
        {
            list<Cidadao*>::iterator it;

            for(it=filiados.begin(); it!=filiados.end(); it++)
            {
                if((*it)->getLimpo()<77)
                    desafiliar(*it);
                else
                    (*it)->desalienar();
            }
        }
};

//quantidade de objetos de "eleição" definidos obedecendo os critérios da modelagem
#define N_ELEITORES 20
#define N_CANDIDATOS 5
#define N_PARTIDOS 3

class Eleicao{
    private:
        vector<Eleitor*> eleitores;
        vector<Candidato*> candidatos;
        vector<Partido*> partidos;

    public:
        Eleicao(): eleitores(), candidatos(), partidos()
        {
            criarEleitores();
            criarCandidatos();
            criarPartidos();

            filiarEleitores();
            filiarCandidatos();

            executar();
        }

        ~Eleicao()
        {
            int tam = (int)eleitores.size();
            
            for(int i=0; i<tam; i++)
                delete(eleitores[i]);

            eleitores.clear();

            tam = (int)candidatos.size();

            for(int i=0; i<tam; i++)
                delete(candidatos[i]);

            candidatos.clear();

            tam = (int)partidos.size();

            for(int i=0; i<tam; i++)
                delete(partidos[i]);

            partidos.clear();

        }

        void criarEleitores()
        {
            Eleitor* eleit;

            for(int i=0; i<N_ELEITORES; i++)
            {
                eleit = new Eleitor();
                eleitores.push_back(eleit);
            }
        }
        void criarCandidatos()
        {
            Candidato* cand;

            for(int i=0; i<N_CANDIDATOS; i++)
            {
                cand = new Candidato();
                candidatos.push_back(cand);
            }
        }

        void criarPartidos()
        {
            Partido* part;

            for(int i=0; i<N_PARTIDOS; i++)
            {
                part = new Partido();
                partidos.push_back(part);
            }
        }

        void filiarEleitores()
        {
            //aqui a intenção é dividir a qtdd de eleitores pela de partidos, para definir
            //quantos eleitores vão ser filiados a cada partido.
            //Ex.: 20 eleitores e 3 partidos: serão filiados 6 eleit. p/ cada part., e 2 ficarão desafiliados
            int interv = (int)eleitores.size()/(int)partidos.size(),
                tam = (int)partidos.size();

            for(int i=0; i<tam; i++)
            {
                for(int j=i*interv; j<(i*interv)+interv; j++)
                    partidos[i]->filiar(eleitores[j]);
            }
        }

        void filiarCandidatos()
        {
            int qtdd_c = (int)candidatos.size(),
                qtdd_p = (int)partidos.size();

            /*percorre cada candidato filiando a um partido diferente, quando filia ao ultimo partido
              começa a filiar ao primeiro novamente (por causa da função de módulo partidos[i%qtdd_p],
              que permite acessar os partidos de maneira circular)*/
            for(int i=0; i<qtdd_c; i++)
            {
                partidos[i%qtdd_p]->filiar(candidatos[i]);

                //apos a filiação, os candidatos são inseridos na lista de eleitores
                eleitores.push_back(candidatos[i]);
            }

            

        }

        void executar()
        {
            //desalienar e desafiliar
            int tam = (int)partidos.size();
            for(int i=0; i<tam; i++)
                partidos[i]->politizar();

            //considerar candidatos
            tam = (int)eleitores.size();
            for(int i=0; i<tam; i++)
            {
                for(int j=0; j<(int)candidatos.size(); j++)
                    eleitores[i]->considerar(candidatos[j]);
            }

            //votar
            for(int i=0; i<tam; i++)
                eleitores[i]->votar();

            //imprimir eleitos
            tam = (int)candidatos.size();
            for(int i=0; i<tam; i++)
            {
                Candidato* cand = candidatos[i];

                cout << "Candidato " << ((!(cand->getEleito()))?"não " : "") << "eleito!" << endl;
            }
        }
};

int main()
{
    Eleicao e;

    return 0;
}