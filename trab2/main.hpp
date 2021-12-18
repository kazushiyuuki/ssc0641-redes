#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

void AplicacaoTransmissora(void);
void CamadaDeAplicacaoTransmissora(string mensagem);
void CamadaEnlaceDadosTransmissora(vector<int> quadro);
void CamadaEnlaceDadosTrasmissoraControleDeErro(vector<int> *quadro);
void CamadaEnlaceDadosTrasmissoraControleDeErroBitParidadePar(vector<int> *quadro);
void CamadaEnlaceDadosTrasmissoraControleDeErroBitParidadeImpar(vector<int> *quadro);
void CamadaEnlaceDadosTrasmissoraControleDeErroCRC(vector<int> quadro);
int paridade(int num);