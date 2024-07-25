#include "reg_paciente.h"

Reg_Paciente::Reg_Paciente():
nivel_endo(),
nivel_sero(),
ok(false) //sempre iniciar as var bool!
{
    //TINHA ESQUECIDO DE ASSOCIAR OS OBJETOS!!!!!!!
    nivel_endo.setSero(&nivel_sero);
    
    nivel_endo.calcular_nivel();
    nivel_sero.calcular_nivel();
}

Reg_Paciente::~Reg_Paciente()
{

}

const int Reg_Paciente::getOK() const
{
    return (int)ok; //TINHA ESQUECIDO DE FAZER O CASTING PARA INT -_-
}

void Reg_Paciente::auto_avaliar()
{
    //ok é true se ambos os níveis forem pelo menos "medio"
    //TINHA ESQUECIDO DE FAZER O CASTING PARA O TIPO BOOL.....
    ok = (bool)((nivel_endo.getNivel()=="medio" || nivel_endo.getNivel()=="normal") &&
                (nivel_sero.getNivel()=="medio" || nivel_sero.getNivel()=="normal"));
}