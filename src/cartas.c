#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
formato para guardas or valores nas seguintes variaveis
&e.mao[0], &e.mao[1], &e.mao[2], &e.mao[3], &e.cartas[0], &e.cartas[1], &e.cartas[2], &e.cartas[3], &e.selecao, &e.passar, 
&e.selecionar, &e.jogar, &e.ultima_jogada, &e.ultimo_jogador)
*/
#define FORMATO 	"%lld_%lld_%lld_%lld_%d_%d_%d_%d_%lld_%d_%d_%d_%lld_%d"
/**
URL da imagem de fundo
*/
#define BACKGROUND 	"http://127.0.0.1/img/background.jpg"

/**
	Estado inicial com todas as 52 cartas do baralho
	Cada carta é representada por um bit que está
	a 1 caso ela pertença à mão ou 0 caso contrário
*/

typedef long long int MAO;

typedef struct estado {

	MAO mao[4]; //soma das combinaçoes de cada jogador
	int cartas[4]; //numero de cartas de cada jogador
	MAO selecao; //soma das cartas selecionadas
	MAO ultima_jogada;
	int ultimo_jogador;
	int passar, jogar, selecionar;

} ESTADO; //declaraçao da variavel do tipo struct estado


ESTADO distribuir ();
ESTADO jogar (ESTADO e);
const MAO ESTADO_INICIAL = 0xfffffffffffff;


/* 
   Estado inicial com todas as 52 cartas do baralho.
   Cada carta é representada por um bit que está a 1 caso ela perntença à mão ou 0 caso contrário.

*/
ESTADO str2estado (char * str) {
	ESTADO e;
	sscanf(str, FORMATO, &e.mao[0], &e.mao[1], &e.mao[2], &e.mao[3], &e.cartas[0], &e.cartas[1], &e.cartas[2], &e.cartas[3], &e.selecao, &e.passar, &e.selecionar, &e.jogar, &e.ultima_jogada, &e.ultimo_jogador);
	return e;
}

char * estado2str (ESTADO e) {
	static char res[10240];
	sprintf(res, FORMATO, e.mao[0], e.mao[1], e.mao[2], e.mao[3], e.cartas[0], e.cartas[1], e.cartas[2], e.cartas[3], e.selecao,  e.passar, e.selecionar, e.jogar, e.ultima_jogada, e.ultimo_jogador);
	return res;
}



int jogada_valida (MAO m) {

	return 1;
}

int combinacao_maior (MAO m1, MAO m2) {
	
	return 1;
}


int posso_jogar (ESTADO e) {

	if (e.ultimo_jogador == 0 && jogada_valida(e.selecao)) return 1;
	if (jogada_valida (e.selecao) && combinacao_maior (e.selecao, e.ultima_jogada)) return 1;

	return 0;
}


/** \brief Devolve o índice da carta

@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O índice correspondente à carta
*/
int indice(int naipe, int valor) {
	return naipe * 13 + valor;
}

/** \brief Adiciona uma carta ao estado

@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O novo estado
*/
long long int add_carta(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return ESTADO | ((long long int) 1 << idx);
}

/** \brief Remove uma carta do estado

@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		O novo estado
*/
long long int rem_carta(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return ESTADO & ~((long long int) 1 << idx);
}

/** \brief Verifica se uma carta pertence ao estado

@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
@return		1 se a carta existe e 0 caso contrário
*/
int carta_existe(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return (ESTADO >> idx) & 1;
}

/** \brief Imprime o html correspondente a uma carta

@param path	o URL correspondente à pasta que contém todas as cartas
@param x A coordenada x da carta
@param y A coordenada y da carta
@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
*/

//BARALHO(path)/ValorNaipe.svg

void imprime_carta(char *path, int x, int y, ESTADO e, int m, int naipe, int valor) {
	char *suit = NAIPES;
	char *rank = VALORES;
	char script[10240];
	

	//if é a minha mao
	if (m == 0) {
		ESTADO novo = e;
		if (carta_existe (novo.selecao, naipe, valor))
			novo.selecao = rem_carta (novo.selecao, naipe, valor);
		else 
			novo.selecao = add_carta (novo.selecao, naipe, valor);
			novo.selecionar = 1;

		//mouse hover, link
		sprintf(script, "%s?%s", SCRIPT, estado2str (novo));
		printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" /></a>\n", script, x, y, path, rank[valor], suit[naipe]);
	
	}
	
	else 
	{
		 if (m==4) //cartas no centro => entao pode mostra-las
		 	printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" />\n", x, y, path, rank[valor], suit[naipe]);
		else
			printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/00.jpg\" />\n", x, y, path);
			//printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" />\n", x, y, path, rank[valor], suit[naipe]);

	}
}


void imprime_botao_jogar (ESTADO e) {
	ESTADO novo = e;
	char script[10240];
	novo.jogar = 1;

	sprintf(script, "%s?%s", SCRIPT, estado2str (novo));
	printf("<a xlink:href = \"%s\"><image x = \"50\" y = \"200\" height = \"100\" width = \"100\" xlink:href = \"http://127.0.0.1/img/playButton.jpg\" /></a>\n", script);

}

void imprime_botao_passar (ESTADO e)
{
	char script [10240];
	ESTADO novo = e;

	sprintf(script, "%s?%s", SCRIPT, estado2str (novo));
	printf("<a xlink:href = \"http://127.0.0.1/cgi-bin/cartas\"><image x = \"50\" y = \"300\" height = \"100\" width = \"100\" xlink:href = \"http://127.0.0.1/img/passButton.jpg\" /></a>\n");
	//printf("<a xlink:href = \"%s\"><image x = \"50\" y = \"300\" height = \"100\" width = \"100\" xlink:href = \"http://127.0.0.1/img/passButton.jpg\" /></a>\n", script);

}


void imprime_botao_limpar (ESTADO e)
{	
	char script [10240];
	ESTADO novo = e;

	sprintf(script, "%s?%s", SCRIPT, estado2str (novo));
	printf("<a xlink:href = \"%s\"><image x = \"50\" y = \"380\" height = \"100\" width = \"100\" xlink:href = \"http://127.0.0.1/img/clearButton.jpg\" /></a>\n", script);
	
}

/** \brief Imprime o estado

Esta função está a imprimir o estado em quatro colunas: uma para cada naipe
@param path	o URL correspondente à pasta que contém todas as cartas
@param ESTADO	O estado atual
*/
void imprime(char *path, ESTADO e) {
	int n, v, m;
	int X[4] = {425, 235, 425, 925};
	int Y[4] = {525, 150, 35, 150};

	for(m=0; m<4; m++) {
		int x = X[m], y = Y[m];
		for (n=0; n<4; n++) {
			for (v=0; v<13; v++) {
				if(carta_existe (e.mao[m], n, v)) {
					if (m % 2 == 0) //m==0 ou 2
						x += 20;  // cartas na horizontal
					else
						y +=20;  // cartas na vertical

					if (carta_existe (e.selecao, n, v))
						imprime_carta (path, x, y-20, e, m, n, v); //subir a carta qd pressionada
					else
						imprime_carta (path, x, y, e, m, n, v);
				}
			}
		}
		if (carta_existe (e.mao[m], 0, 0))
			rem_carta (e.mao[m], 0, 0);
	}



	/*for (m=0; m<4; m++) 
		if (carta_existe (e.mao[m], 0, 0)) {
			
			imprime_carta (BARALHO, 300, 400, e, m, 0, 0);
		}
		imprime_carta (BARALHO, 350, 400, e, m, 0, 0);

	*/
	imprime_botao_jogar (e);
	imprime_botao_passar (e);
	imprime_botao_limpar (e);


}

void primeiroJogar(char *path, ESTADO e)
{
	//quem tem a carta 3d.svg n=0 v=0
	int n, v, m;

//	imprime_carta (BARALHO, x, y, e, 4, 0, 0);

	for(m=0; m<4; m++)
		if(carta_existe (e.mao[m], 0, 0))
	
	e.ultimo_jogador = m;
			
}


/** \brief Poe no jogo a carta escolhida pelo bot.

 */
ESTADO jogar_bots(ESTADO e,int m,int x, int y)
{
	int n, v;

	for (n=0; n<4; n++) {
		for (v=0; v<13; v++) {
			if (carta_existe (e.mao[m], n, v)) {
				e.mao[m] = rem_carta (e.mao[m], n, v);
				imprime_carta (BARALHO, x, y, e, 4, n, v);
				x +=20;
				return e;
			}
		}
	}
	
}


/** \brief Joga a carta selecionada e de seguida invoca as jogadas validas dos bots

 */

ESTADO jogar (ESTADO e) {
	int n, v, x=500, y=400;
	int m;


	for (n=0; n<4; n++) {
		for (v=0; v<13; v++) {
			if (carta_existe (e.selecao, n, v)) {
				x +=20;
				imprime_carta (BARALHO, x, y, e, 4, n, v); // 4-> "mao" na mesa
				e.mao[0] = rem_carta (e.mao[0], n, v);
			}
		}
	}

	e.jogar = 0;
	e.selecao = 0;

	jogar_bots(e,1,320,270);
	jogar_bots(e,2,500,150);
	jogar_bots(e,3,730,270);

	return e;
}

/** \brief Trata os argumentos da CGI

Esta função recebe a query que é passada à cgi-bin e trata-a.
Neste momento, a query contém o estado que é um inteiro que representa um conjunto de cartas.
Cada carta corresponde a um bit que está a 1 se essa carta está no conjunto e a 0 caso contrário.
Caso não seja passado nada à cgi-bin, ela assume que todas as cartas estão presentes.
@param query A query que é passada à cgi-bin
 */
void parse(char *query) {
	
	ESTADO e;
	
	if (query!= NULL && strlen (query)!= 0 ) {
		e = str2estado (query);
		if (e.jogar == 1) e = jogar (e);
		if (e.selecionar == 1) e.selecionar = 0;

	} else {
		e = distribuir ();
	}


	imprime (BARALHO, e);

}


ESTADO distribuir() {
	ESTADO e = {{0}, {0}, 0, 0, 0, 0}; // Inicialização da estrutura
	int n, v;

	for(n=0;n<4;n++) {
		for(v=0;v<13;v++) {
			int r;

			do {
				r = random()%4;
			} while (e.cartas[r]==13);

			e.mao[r] = add_carta(e.mao[r],n,v);
			e.cartas[r]++;
		}
	}

	return e;
}



/** \brief Função principal

Função principal do programa que imprime os cabeçalhos necessários e depois disso invoca
a função que vai imprimir o código html para desenhar as cartas
 */
int main() {
/*
 * Cabeçalhos necessários numa CGI
 */
	printf("Content-Type: text/html; charset=utf-8\n\n");

	//tab title
	printf("<header><title>Big Two - Jogo</title></header>\n");
	
	//background img
	printf("<body background=\"%s\">\n",BACKGROUND);
	
	printf("<body>\n");
	printf("<svg height = \"680\" width = \"1200\" >\n");

	//titulo principal
	printf("<rect x= \"0\" y= \"0\" width=\"1000\" height=\"30\" ry=\"5\" fill =#a0a0a0 />");
	printf("<text size=\"6\" x=\"600\" y=\"15\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\">BIG TWO </text>\n");
	printf("</a>");

	srandom(time(NULL));


/*
 * Ler os valores passados à cgi que estão na variável ambiente e passá-los ao programa
 */
	parse(getenv("QUERY_STRING"));

	printf("</svg>\n");
	printf("</html>\n");
	return 0;
}
