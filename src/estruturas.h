#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


/**
URL da CGI
*/
#define SCRIPT		"http://127.0.0.1/cgi-bin/cartas"
/**
URL da pasta com as cartas
*/
#define BARALHO		"http://127.0.0.1/cards"
/**
Ordem dos naipes
*/
#define NAIPES		"DCHS"
/**
Ordem das cartas
*/
#define VALORES		"3456789TJQKA2"
/**
Formato para guardar os valores nas seguintes variaveis:
&e.mao[0], &e.mao[1], &e.mao[2], &e.mao[3], &e.cartas[0], &e.cartas[1], &e.cartas[2], &e.cartas[3], &e.selecao, &e.passar, 
&e.selecionar, &e.jogar, &e.ultima_jogada, &e.ultimo_jogador)
*/
#define FORMATO 	"%lld_%lld_%lld_%lld_%d_%d_%d_%d_%lld_%lld_%d_%d_%d_%d_%d_%d"
/**
URL da imagem de fundo
*/
#define BACKGROUND 	"http://127.0.0.1/img/background.jpeg"


/**
Novo tipo de dados do tipo "long long int"
*/
typedef long long int MAO;
/**
	Estado inicial com todas as 52 cartas do baralho
	Cada carta é representada por um bit que está
	a 1 caso ela pertença à mão ou 0 caso contrário
*/
extern const MAO ESTADO_INICIAL;
/**
Pontuação dos jogadores
*/
extern int score[4];

extern MAO maostraight;

typedef struct estado {

	MAO mao[4]; //soma das combinaçoes de cada jogador
	int cartas[4]; //numero de cartas de cada jogador
	MAO selecao; //valor das cartas selecionadas
	MAO ultima_jogada;
	MAO jogada[3]; //jogadas dos bots
	int ultimo_jogador;
	int passar, jogar, selecionar;
	int change,help;

} ESTADO;


typedef struct fullHouseValues {

	int tripleValue,doubleValue;

} FULLHOUSE;


typedef struct flush {

	int suit, highValue, lowerValue;
	MAO fmao;

} FLUSH;


typedef struct straight {

	MAO smao;
	int highValue;
	int lowerValue;

} STRAIGHT;



ESTADO distribuir ();
ESTADO jogar (ESTADO e);
ESTADO primeiroJogar(ESTADO e);
ESTADO call_bots(ESTADO e);
ESTADO imprime_carta(char *path, int x, int y, ESTADO e, int m, int naipe, int valor);
int posso_jogar (ESTADO e);
int combinacao_maior (MAO m1, MAO m2);
int jogada_valida (MAO m);
int contaCartas(MAO m);
int maiorValor(MAO m);
int maiorNaipe(MAO m);
int duplo(MAO m, int valor);
int triplo(MAO m, int valor);
FULLHOUSE fullHouse(MAO m, int valor);
int fourOfaKind(MAO m, int valor);
FLUSH flush(MAO m, int valor);
int straight(MAO m, int valor);
ESTADO calcularScore(ESTADO e, int winner);
int jogoAcabou(ESTADO e);
long long int add_carta(long long int ESTADO, int naipe, int valor);
int indice(int naipe, int valor);
long long int rem_carta(long long int ESTADO, int naipe, int valor);
int carta_existe(long long int ESTADO, int naipe, int valor);
int indice(int naipe, int valor);