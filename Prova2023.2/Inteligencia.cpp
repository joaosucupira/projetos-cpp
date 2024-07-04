#include <iostream>
#include <set>
#include <list>
#include <vector>
using namespace std;

class Agente {
    protected:
        static int cont_id;
        int id;
        int idade;
    public:
        Agente(const int idd) { idade = idd; }
        Agente() { idade = 21; }

        virtual void agir() = 0;
        const int getIdade() const { return idade; }
        const int getId() const { return id; }
        void operator++() { idade++; }
};
class Secreto;
class Duplo;
class Espiao;
class Inteligencia
{
private:
    set<Agente*> colecao;
public:
    Inteligencia() {
        criarAgentes();
        executar();
    }
    ~Inteligencia();

    void criarAgentes() {};
    void executar() {};
};


int main(int argv, char* agrc[]) {

    return 0;
}