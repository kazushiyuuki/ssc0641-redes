#include <iostream>
#include <bitset>
#include <vector>
#include "main.hpp"

using namespace std;

void AplicacaoTransmissora(void)
{
    string mensagem;
    cout << "Digite uma mensagem:" << endl;
    cin >> mensagem;

    CamadaDeAplicacaoTransmissora(mensagem);
}

void CamadaDeAplicacaoTransmissora(string mensagem)
{
    int len_msg = mensagem.length();
    string aux;
    for (int i = 0; i < len_msg; i++)
    {
        aux += bitset<8>(mensagem.c_str()[i]).to_string();
    }
    int len_aux = aux.length();
    vector<int> quadro;
    for (int i = 0; i < len_aux; i++)
    {
        quadro.push_back(aux[i] - 48);
    }
    CamadaEnlaceDadosTransmissora(quadro);
}

void CamadaEnlaceDadosTransmissora(vector<int> quadro)
{
    CamadaEnlaceDadosTrasmissoraControleDeErro(&quadro);
    for (auto i = quadro.begin(); i != quadro.end(); i++)
    {
        cout << *i;
    }
    cout << endl;
}

void CamadaEnlaceDadosTrasmissoraControleDeErro(vector<int> *quadro)
{
    int tipoDeControleDeErro = 1; // alterar de acordo com o teste
    switch (tipoDeControleDeErro)
    {
    case 0: // bit de paridade par
        CamadaEnlaceDadosTrasmissoraControleDeErroBitParidadePar(quadro);
        break;
    case 1: // bit de paridade impar
        CamadaEnlaceDadosTrasmissoraControleDeErroBitParidadeImpar(quadro);
        break;
    case 2: // CRC
        //
        break;
    }
}

void CamadaEnlaceDadosTrasmissoraControleDeErroBitParidadePar(vector<int> *quadro)
{
    int contador = 0;
    for (auto i = (*quadro).begin(); i != (*quadro).end(); i++)
    {
        if (*i)
            contador++;
    }
    (*quadro).push_back(paridade(contador));
}

void CamadaEnlaceDadosTrasmissoraControleDeErroBitParidadeImpar(vector<int> *quadro)
{
    int contador = 0;
    for (auto i = (*quadro).begin(); i != (*quadro).end(); i++)
    {
        if (*i)
            contador++;
    }
    (*quadro).push_back((paridade(contador) == 0) ? 1:0);
    
}

void CamadaEnlaceDadosTrasmissoraControleDeErroCRC(vector<int> quadro[])
{
    // implementacao do algoritmo
    // usar polinomio CRC-32(IEEE 802)
}
/*
void MeioDeComunicacao(int fluxoBrutoDeBits[])
{
    int erro, porcentagemDeErros;
    int fluxoBrutoDeBitsPontoA[2], fluxoBrutoDeBitsPontoB[2];

    porcentagemDeErros = 0;
    fluxoBrutoDeBitsPontoA = fluxoBrutoDeBits;

    while (fluxoBrutoDeBitsPontoB.lenght != fluxoBrutoDeBits)
    {
        if ((rand) % 100 == 1)
            fluxoBrutoDeBitsPontoB += fluxoBrutoDeBitsPontoA;
        else
            fluxoBrutoDeBitsPontoB == 0 ? fluxoBrutoDeBitsPontoA = fluxoBrutoDeBitsPontoB++ : fluxoBrutoDeBitsPontoA = fluxoBrutoDeBitsPontoB--;
    }
}*/

int paridade(int num)
{
    if (num % 2 == 0)
        return 0;
    else
        return 1;
}


int main(void)
{
    AplicacaoTransmissora();
}
