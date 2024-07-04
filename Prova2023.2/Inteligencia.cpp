#include <iostream>
#include <set>
#include <list>
#include <vector>
using namespace std;

class Secreto;

// Classe abstrata principal
class Agente {
    protected:
        static int cont_id;
        int id;
        int idade;
    public:
        Agente(const int idd) { 
            idade = idd; 
            if (idade < 21) { idade = 21; }
            if (idade > 59) { idade = 59; }
            cont_id += 1;
            id = cont_id;  
        }

        Agente() { idade = 21; ++cont_id; }

        virtual ~Agente() {};

        virtual void agir() = 0;
        const int getIdade() const { return idade; }
        const int getId() const { return id; }
        void operator++() { idade++; }

};

class Espiao : public Agente {
    private:
    list<Secreto*> secrets;
    bool bocoh;

    public:
        Espiao(const int idd) : Agente(idd) {}
        Espiao() : Agente() { idade = 21; }
        ~Espiao() {}

        void agir() {};
        void incluirSecreto(Secreto* p) {};
        void setBocoh(const bool b) {};
        const bool getBocoh() const {};

};

class Secreto : public Agente {
    protected:
        vector<Espiao*> espias;
        float forca;
    public:
    Secreto(const int idd) : Agente(idd) {}
    Secreto() : Agente() {}
    virtual ~Secreto() {}

    virtual void agir() {}
    const float getForca() const {};
    virtual void incluirEspiao() {};
    void operator--() {};

};

// Classe derivada que interage com Espião (contatar)
class Duplo : public Secreto {
    private:
        Espiao* pContato;
    public:
        Duplo(const int idd) : Secreto(idd) {};
        Duplo() : Secreto() {}
        ~Duplo() {};

        void incluirEspiao(Espiao* p) {}
};


class Inteligencia
{
private:
    set<Agente*> colecao;
public:
    Inteligencia() {
        criarAgentes();
        executar();
    }
    ~Inteligencia() {};

    void criarAgentes() {
        colecao.insert(new Espiao(80));
        colecao.insert(new Duplo(24));
        colecao.insert(new Secreto(15));
    };
    void executar() {
        set<Agente*>::iterator iterator;
        iterator = colecao.begin();
        while (iterator != colecao.end()) {
            cout << (*iterator)->getId() 
                 << " - IDADE = " << (*iterator)->getIdade()
            << endl;
            ++iterator;
        }


    };
};

int Agente::cont_id = 0;

int main(int argv, char* agrc[]) {

    Inteligencia intel;
    return 0;
}