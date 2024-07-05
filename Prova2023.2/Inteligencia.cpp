#include <iostream>
#include <ctime>
#include <set>
#include <list>
#include <vector>
using namespace std;


class Secreto;
class Duplo;
class Espiao;

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

        Agente() { 
            idade = 21 + rand() % (59 - 21);            
            cont_id += 1; 
            id = cont_id;
        }

        virtual ~Agente() {};

        virtual void agir() = 0;
        const int getIdade() const { return idade; }
        const int getId() const { return id; }
        void operator++() { idade++; }

};

// Classe derivada que interage indiretamente com Espião (classe Duplo)
class Secreto : public Agente {
    protected:
        vector<Espiao*> espias;
        float forca;
    public:
    Secreto(const int idd) : Agente(idd) {
        forca = (100 - idade);
    }
    Secreto() : Agente() { forca = (100 - idade); }
    virtual ~Secreto() {}

    virtual void agir(); // Mandei a definição para baixo por dependencia de classes

    const float getForca() const { return forca; };
    virtual void incluirEspiao(Espiao* p) {
        espias.push_back(p); // undefined ref
    };
    void operator--() { forca--; }

};

// Classe derivada que interage diretamente com a classe Secreto (agir)
class Espiao : public Agente {
    private:
        list<Secreto*> secrets;
    bool bocoh;

    public:
        Espiao(const int idd) : Agente(idd) {
            int sorteio = rand() % 101;
            setBocoh(sorteio < 90);
        }
        Espiao() : Agente() { 
            int sorteio = rand() % 101;
            setBocoh(sorteio < 90);
         }
        ~Espiao() {}

        void agir();

        void incluirSecreto(Secreto* p) {
            secrets.push_back(p); // undefined ref
        };
        void setBocoh(const bool b) { bocoh = b; };
        const bool getBocoh() const { return bocoh; };

};

// Classe derivada que interage diretamente com Espião (contatar)
class Duplo : public Secreto {
    private:
        Espiao* pContato;
    public:
        Duplo(const int idd) : Secreto(idd) {};
        Duplo() : Secreto() {}
        ~Duplo() {};

        void incluirEspiao(Espiao* p) {
            pContato = p;
        }
        void agir() {
            pContato->setBocoh(false);
        }
};

void Espiao::agir() {
    list<Secreto*>::iterator it;
    it = secrets.begin();
    while (it != secrets.end()) { // undefined  ref
        if ((*it)->getForca() > 0) {
            --(**it);
            ++it;
            
        } else {
            secrets.erase(it);
            break;
        }
    }
}

void Secreto::agir() {
    vector<Espiao*>::iterator it = espias.begin();
    int n =  (espias.size() > forca ? forca : espias.size());
    int i = 0;
    while (i < n && it != espias.end()) { // undefined ref
        if ((*it)->getBocoh()) {
            espias.erase(it);
            i++;
        } else { 
            ++it;
        }
    }
}

// Classe principal
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

        // for (int i = 0; i < 2; i++) {
        //     Secreto* AAA = new Secreto();
        //     Duplo* BBB = new Duplo();
        //     Espiao* CCC = new Espiao();
        //     colecao.insert(AAA);
        //     colecao.insert(BBB);
        //     colecao.insert(CCC);
        // }
        Secreto* AAA = new Secreto();
        Espiao* BBB = new Espiao();
        Duplo* CCC = new Duplo();
        colecao.insert(AAA);
        colecao.insert(BBB);
        colecao.insert(CCC);

        AAA->incluirEspiao(BBB);
        BBB->incluirSecreto(AAA);
        CCC->incluirEspiao(BBB);

    }

    void executar() {
        for (int i = 0; i < 3; i++) {
            mostrar();
        }

    }
    void mostrar() {
        cout << "ID    IDADE    CLASSE    COLECAO" << endl;
        set<Agente*>::iterator it;
        it = colecao.begin();
        while (it != colecao.end()) {
            Espiao* espiao = dynamic_cast<Espiao*>(*it);
            Duplo* duplo = dynamic_cast<Duplo*>(*it);
            Secreto* secreto = dynamic_cast<Secreto*>(*it);

            (*it)->agir();
            ++(**it);
            ++it;
            cout << endl;
        }
        cout<<endl;
    }
};

int Agente::cont_id = 0; // Inicialização da contagem de Agentes

int main(int argv, char* agrc[]) {
    srand(time(NULL));
    Inteligencia intel; // Classe principal que executa toda a rede de objetos e ações ao ser inicializada
    return 0;
}