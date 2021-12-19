// https://lxp32.github.io/docs/a-simple-example-crc32-calculation/
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
    cout << "Valor binario do quadro: ";
    for (auto i = quadro.begin(); i != quadro.end(); i++)
    {
        cout << *i;
    }
    cout << endl;
    CamadaEnlaceDadosTrasmissoraControleDeErro(&quadro);
    for (auto i = quadro.begin(); i != quadro.end(); i++)
    {
        cout << *i;
    }
    cout << endl;
    MeioDeComunicacao(&quadro);
}

void CamadaEnlaceDadosTrasmissoraControleDeErro(vector<int> *quadro)
{
    int tipoDeControleDeErro = 2; // alterar de acordo com o teste
    switch (tipoDeControleDeErro)
    {
    case 0: // bit de paridade par
        cout << "Valor binario com paridade par: ";
        CamadaEnlaceDadosTrasmissoraControleDeErroBitParidadePar(quadro);
        break;
    case 1: // bit de paridade impar
        cout << "Valor binario com paridade impar: ";
        CamadaEnlaceDadosTrasmissoraControleDeErroBitParidadeImpar(quadro);
        break;
    case 2: // CRC
        cout << "Valor binario com CRC: ";
        CamadaEnlaceDadosTrasmissoraControleDeErroCRC(quadro);
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
    (*quadro).push_back((paridade(contador) == 0) ? 1 : 0);
}

void CamadaEnlaceDadosTrasmissoraControleDeErroCRC(vector<int> *quadro)
{
    // implementacao do algoritmo
    // usar polinomio CRC-32(IEEE 802)
    char *hexa = converte(quadro);
    string aux = bitset<32>(crc32(hexa, (*quadro).size() / 8)).to_string();
    for (int i = 0; i < aux.length(); i++)
    {
        (*quadro).push_back(aux[i] - 48);
    }
    free(hexa);
}

void MeioDeComunicacao(vector<int> *fluxoBrutoDeBits)
{
    int erro, porcentagemDeErros;
    vector<int> fluxoBitsB;

    porcentagemDeErros = 80;
    for (auto i = (*fluxoBrutoDeBits).begin(); i != (*fluxoBrutoDeBits).end(); i++)
    {
        srand(time(NULL));
        if (rand() % 100 + 1 >= porcentagemDeErros)
            fluxoBitsB.push_back(*i);
        else
            (*i) == 0 ? fluxoBitsB.push_back(1) : fluxoBitsB.push_back(0);
    }

    CamadaEnlaceDadosReceptora(fluxoBitsB);
}

void CamadaEnlaceDadosReceptora(vector<int> quadro)
{
    cout << "Valor binario recebido: ";
    for (auto i = quadro.begin(); i != quadro.end(); i++)
    {
        cout << *i;
    }
    cout << endl;
    CamadaEnlaceDadosReceptoraControleDeErro(&quadro);
    for (auto i = quadro.begin(); i != quadro.end(); i++)
    {
        cout << *i;
    }
    cout << endl;
    CamadaAplicacaoReceptora(&quadro);
}

void CamadaEnlaceDadosReceptoraControleDeErro(vector<int> *quadro)
{
    int tipoDeControleDeErro = 2; // alterar de acordo com o teste
    switch (tipoDeControleDeErro)
    {
    case 0: // bit de paridade par
        CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(quadro);
        break;
    case 1: // bit de paridade impar
        CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(quadro);
        break;
    case 2: // CRC
        CamadaEnlaceDadosReceptoraControleDeErroCRC(quadro);
        break;
    }
}

void CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(vector<int> *quadro)
{
    int contador = 0;
    for (auto i = (*quadro).begin(); i != (*quadro).end() - 1; i++)
    {
        if (*i)
            contador++;
    }
    if (paridade(contador) != (*((*quadro).end() - 1)))
        cout << "ERRO DETECTADO!" << endl;
    (*quadro).pop_back();
    cout << "Valor binario sem bit de paridade par: ";
}

void CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(vector<int> *quadro)
{
    int contador = 0;
    for (auto i = (*quadro).begin(); i != (*quadro).end() - 1; i++)
    {
        if (*i)
            contador++;
    }
    if (!paridade(contador) != (*((*quadro).end() - 1)))
        cout << "ERRO DETECTADO!" << endl;
    (*quadro).pop_back();
    cout << "Valor binario sem bit de paridade impar: ";
}

void CamadaEnlaceDadosReceptoraControleDeErroCRC(vector<int> *quadro)
{

    // implementacao do algoritmo
    // usar polinomio CRC-32(IEEE 802)
    vector<int> novo_quadro;
    for (int i = 0; i < (*quadro).size() - 32; i++)
    {
        novo_quadro.push_back((*quadro)[i]);
    }
    char *hexa = converte(&novo_quadro);
    string aux = bitset<32>(crc32(hexa, novo_quadro.size() / 8)).to_string();
    for (int i = 0; i < aux.length(); i++)
    {
        novo_quadro.push_back(aux[i] - 48);
    }
    free(hexa);
    for (int i = 0; i < novo_quadro.size(); i++)
    {
        if (novo_quadro[i] != (*quadro)[i])
        {
            cout << "ERRO DETECTADO!" << endl;
            break;
        }
    }
    for (int i = 0; i < 32; i++)
    {
        (*quadro).pop_back();
    }
    cout << "Valor binario sem o R: " << endl;
}

void CamadaAplicacaoReceptora(vector<int> *quadro)
{
    char *c = converte(quadro);
    cout << "Mensagem convertida: " << c << endl;
    AplicacaoReceptora(c);
}

void AplicacaoReceptora(char *mensagem)
{
    cout << "Mensagem recebida: " << mensagem << endl;
    free(mensagem);
}

int paridade(int num)
{
    if (num % 2 == 0)
        return 0;
    else
        return 1;
}

char *converte(vector<int> *quadro)
{

    int res = 0, j = 7, k = 0;
    char *vetor = (char *)malloc(sizeof((*quadro).size() / 8 + 1));
    vetor[(int)((*quadro).size() / 8)] = '\0';
    for (auto i = (*quadro).begin(); i != (*quadro).end(); i++)
    {
        res = res + (*i) * pow(2, j--);
        if (j == -1)
        {
            j = 7;
            vetor[k++] = res;
            res = 0;
        }
    }
    return vetor;
}

uint32_t crc32(const char *s, size_t n)
{
    uint32_t crc = 0xFFFFFFFF;

    for (size_t i = 0; i < n; i++)
    {
        char ch = s[i];
        for (size_t j = 0; j < 8; j++)
        {
            uint32_t b = (ch ^ crc) & 1;
            crc >>= 1;
            if (b)
                crc = crc ^ 0xEDB88320;
            ch >>= 1;
        }
    }

    return ~crc;
}

int main(void)
{
    AplicacaoTransmissora();
}
