#include <iostream>
#include <bitset>
#include <vector>
#include "time.h"
#include "math.h"

using namespace std;

void AplicacaoTransmissora(void);
void CamadaDeAplicacaoTransmissora(string mensagem);
void CamadaEnlaceDadosTransmissora(vector<int> quadro);
void CamadaEnlaceDadosTrasmissoraControleDeErro(vector<int> *quadro);
void CamadaEnlaceDadosTrasmissoraControleDeErroBitParidadePar(vector<int> *quadro);
void CamadaEnlaceDadosTrasmissoraControleDeErroBitParidadeImpar(vector<int> *quadro);
void CamadaEnlaceDadosTrasmissoraControleDeErroCRC(vector<int> *quadro);
void MeioDeComunicacao(vector<int> *fluxoBrutoDeBits);
void CamadaEnlaceDadosReceptora(vector<int> quadro);
void CamadaEnlaceDadosReceptoraControleDeErro(vector<int> *quadro);
void CamadaEnlaceDadosReceptoraControleDeErroBitParidadePar(vector<int> *quadro);
void CamadaEnlaceDadosReceptoraControleDeErroBitParidadeImpar(vector<int> *quadro);
void CamadaEnlaceDadosReceptoraControleDeErroCRC(vector<int> *quadro);
void CamadaAplicacaoReceptora(vector<int> *quadro);
void AplicacaoReceptora(char *mensagem);
int paridade(int num);
char *converte(vector<int> *quadro);
uint32_t crc32(const char *s,size_t n);