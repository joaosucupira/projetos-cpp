#include "hormonio.h"

/*no contexto de desenvolver o código em um unico documento.cpp, a inicialização da variavel static
  é feita fora da definição e implementação da classe*/
int Hormonio::soma = 0; //inicialização de variavel static

Hormonio::Hormonio() //DEVERIA TER CHAMADO A CONSTRUTORA DOS OBJETOS DO TIPO BOOL E SOMA...
//nivel("indeterminado"),
//estresse(!((bool)(soma%2)))
{
    nivel = "indeterminado";
    estresse = !((bool)(soma%2)); //se soma for par, estresse é true
    soma++;

    //P.S.: esqueci de fazer o casting para bool!
}

Hormonio::~Hormonio()
{
    nivel = "indeterminado";
    soma--;
}

const string Hormonio::getNivel()const
{
    return nivel;
}