#include "diagnosticador.h"

Diagnosticador::Diagnosticador(const int num)/*:
listap(),
saida()
*/
{
    int cont = 0;

    //garantir q a lista ta vazia antes de preencher
    listap.clear();

    Reg_Paciente* registro;

    while(cont<num)
    {
        registro = new Reg_Paciente(); //aloca um novo registro e passa o endereço pro ptr temporario
        listap.push_back(registro); //insere o registro na lista
        cont++;
    }

    diagnosticar();
}

Diagnosticador::~Diagnosticador()
{
    list<Reg_Paciente*>::iterator iterador; //declaração do iterador para percorrer a lista

    //copiei esse for dos slides, mas fiquei na duvida se ele não exclui o 1o ou o ultimo elem. da lista?
    for(iterador=listap.begin(); iterador!=listap.end(); iterador++)
        delete(*iterador); 
        //desaloca o conteúdo apontado por iterador (que na pratica é um ptr para um registro)

    listap.clear(); //libera a lista
}

ostringstream& Diagnosticador::getSaida()
{
    return saida;
}

void Diagnosticador::diagnosticar()
{
    //um contador para o total de registros e outro somente para os registros 'ok'
    int cont_ok = 0;

    list<Reg_Paciente*>::iterator iterador;

    for(iterador=listap.begin(); iterador!=listap.end(); iterador++)
    {
        //o conteudo apontado por iterador, que na pratica é um ponteiro para registro
        //executa a auto-avaliação para definir se está ok ou não
        (*iterador)->auto_avaliar();

        if(((*iterador)->getOK()))
            cont_ok++;
    }

    //calcula a porcentagem
    float porcent = ((float)cont_ok/(float)listap.size())*100.0;

    //armazena a porcentagem no buffer
    saida << "A porcentagem de pacientes ok eh de " << porcent << "%" << endl;
}

ostream& operator<<(ostream& saida, Diagnosticador& d)
{
    saida << (d.getSaida()).str();
    return saida;
}