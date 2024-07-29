#include <iostream>
#include <ctime>
#include <set>
#include <list>
#include <vector>
#include <algorithm>
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

        virtual ~Agente() {
            id = -1;
            idade = -1;
        };

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
        espias.push_back(p); 

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
        Espiao() : Agente(), secrets() { 
            int sorteio = rand() % 101;
            setBocoh(sorteio < 90);
         }
        ~Espiao() {}

        void agir();

        void incluirSecreto(Secreto* p) {
            if (p) {
                secrets.push_back(p);  
            } else {
                cout << endl << "Agente nulo nao pode ser incluido." << endl;
            }

        };
        void setBocoh(const bool b) { bocoh = b; };
        const bool getBocoh() const { return bocoh; };

};


//Comentario do Leonardo teste

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
            cout <<  "Agente Duplo " << id << " contatou espiao " << pContato->getId() << endl; 
        }
};

void Espiao::agir() {
    list<Secreto*>::iterator it;

    it = secrets.begin();
    while (it != secrets.end()) {
        // FAzer ficar igual a solução
        if (*it == nullptr) {
            it = secrets.erase(it);

        } else if ((*it)->getForca() > 0.0) {
            
            --(**it);
            ++it;
            
        } else {
            cout << "Espiao " << id << " neutralizou agente secreto " << (*it)->getId() << endl;
            delete *it;
            it = secrets.erase(it);
            break;
        }
    }
}

void Secreto::agir() {
    vector<Espiao*>::iterator it = espias.begin();
    int n =  (espias.size() > forca ? forca : espias.size());
    int i = 0;
    while (i < n && it != espias.end()) { 
        if (*it == nullptr ) {
            it = espias.erase(it);
        
        } else if ((*it)->getBocoh()) {
            cout << "Agente secreto " << id << " neutralizou o espiao " << (*it)->getId() << endl;
            delete *it;
            it = espias.erase(it);
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

    // Cria os agentes e define a melhor forma de serem preenchidas as coleçoes de cada objeto
    void criarAgentes() {
        int n = 4;
        for(int i = 0; i < n; i ++) {
            Espiao* EEE = new Espiao();
            Secreto* SSS = new Secreto();
            colecao.insert(EEE);
            colecao.insert(SSS);
            if (i % 2 == 0) {
                Duplo* DDD = new Duplo();
                colecao.insert(DDD);
                DDD->incluirEspiao(EEE);
                Espiao* EEE2 = new Espiao();
            }

            if(i == (n - 1)) { cout << "CONTAGEM DE AGENTES: " << SSS->getId() << endl; }
        }



        set<Agente*>::iterator it;
        for (it = colecao.begin(); it != colecao.end(); ++it) {
            Espiao* espiao = dynamic_cast<Espiao*>(*it);
            Secreto* secreto = dynamic_cast<Secreto*>(*it);
            Duplo* duplo = dynamic_cast<Duplo*>(*it);

            if (espiao) {
                set<Agente*>::iterator it2;
                for (it2 = colecao.begin(); it2 != colecao.end(); ++it2) {
                    Secreto* outroSecreto = dynamic_cast<Secreto*>(*it2);
                    if (outroSecreto && outroSecreto != duplo) {
                        espiao->incluirSecreto(outroSecreto);
                    }
                }
            } else if (secreto) {
                set<Agente*>::iterator it2;
                for (it2 = colecao.begin(); it2 != colecao.end(); ++it2) {
                    Espiao* outroEspiao = dynamic_cast<Espiao*>(*it2);
                    if (outroEspiao) {
                        secreto->incluirEspiao(outroEspiao);
                    }
                }
            }
        }
        cout << "! - [Agentes criados e inseridos...]" << endl << endl;

    }

    // Executa os objetos e mostra na tela os eventos ocorrendo e os relacionamentos de objeto ocorrendo. 
    void executar() {
        int n = 50;
        set<Agente*>::iterator iterador;

        for (int i = 0; i < n; i++) {
            iterador = colecao.begin();
            while (iterador != colecao.end()) {
                if (*iterador == nullptr) {
                    iterador = colecao.erase(iterador);
                }

                else {
                    (*iterador)->agir();
                    ++iterador;
                }
            }
            iterador = colecao.begin();
            while (iterador != colecao.end()) {
                if (*iterador == nullptr) {
                    iterador = colecao.erase(iterador);
                } else {
                    ++iterador;
                }
            }
        }


        iterador = colecao.begin();
        while (iterador != colecao.end()) 
        {
            delete (*iterador);
            ++iterador;
        }

        cout << endl << "! - [Fim da execução...]" << endl;
    }
};

int Agente::cont_id = 0; // Inicialização da contagem de Agentes

int main(int argv, char* agrc[]) {
    srand(time(NULL));
    Inteligencia intel; // Classe principal que executa toda a rede de objetos e ações ao ser inicializada

    return 0;
}
