#include "bots.h"


const MAO ESTADO_INICIAL = 0xfffffffffffff;
/**
Pontuação dos jogadores
*/
int score[4]={0};

MAO maostraight=0;


/** \brief Converte a string FORMATO para o estado.

@param *str	String do tipo FORMATO (url do browser)
@return		O novo estado
*/
ESTADO str2estado (char * str) {
	ESTADO e;
	sscanf(str, FORMATO, &e.mao[0], &e.mao[1], &e.mao[2], &e.mao[3], &e.cartas[0], &e.cartas[1], &e.cartas[2], &e.cartas[3], &e.ultima_jogada, &e.selecao, &e.ultimo_jogador, &e.jogar, &e.passar, &e.selecionar, &e.change, &e.help);
	return e;
}


/** \brief Converte as variáveis do estado numa string (url).

@param ESTADO	O estado atual
@return		String (url) do tipo formato com as variáveis do estado
*/
char * estado2str (ESTADO e) {
	static char res[10240];
	sprintf(res, FORMATO, e.mao[0], e.mao[1], e.mao[2], e.mao[3], e.cartas[0], e.cartas[1], e.cartas[2], e.cartas[3], e.ultima_jogada, e.selecao, e.ultimo_jogador, e.jogar, e.passar, e.selecionar, e.change, e.help);
	return res;
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


/** \brief Imprime no ecrã o botão respetivo a jogar uma seleção de cartas.

@param ESTADO	O estado atual
*/
void imprime_botao_jogar (ESTADO e) {
	ESTADO novo = e;
	char script[10240];
	novo.jogar = 1;

	//converte a estrutura novo para uma string e coloca-a no link, alterando o estado
	sprintf(script, "%s?%s", SCRIPT, estado2str (novo));
	printf("<a xlink:href = \"%s\"><image x = \"350\" y = \"545\" height = \"80\" width = \"80\" xlink:href = \"http://127.0.0.1/img/playButton.ico\" /></a>\n", script);
	
}


/** \brief Imprime no ecrã o botão respetivo a passar uma jogada.

@param ESTADO	O estado atual
*/
void imprime_botao_passar (ESTADO e) {
	char script [10240];
	ESTADO novo = e;
	novo.passar=1;

	sprintf(script, "%s?%s", SCRIPT, estado2str (novo));
	printf("<a xlink:href = \"%s\"><image x = \"260\" y = \"545\" height = \"80\" width = \"80\" xlink:href = \"http://127.0.0.1/img/skipButton.png\" /></a>\n", script);
}


/** \brief Imprime no ecrã o botão respetivo a limpar uma seleção de cartas.

@param ESTADO	O estado atual
*/
void imprime_botao_limpar (ESTADO e) {	
	
	char script [10240];
	ESTADO novo = e;
	novo.selecao=0;

	sprintf(script, "%s?%s", SCRIPT, estado2str (novo));
	printf("<a xlink:href = \"%s\"><image x = \"155\" y = \"532\" height = \"100\" width = \"100\" xlink:href = \"http://127.0.0.1/img/clearButton.jpg\" /></a>\n", script);
}



/** \brief Imprime no ecrã o botão respetivo a reiniciar o jogo.
*/
void imprime_botao_home() {	
	printf("<a xlink:href = \"%s\"><image x = \"70\" y = \"545\" height = \"80\" width = \"80\" xlink:href = \"http://127.0.0.1/img/homeButton.png\" /></a>\n", SCRIPT);
}


/** \brief Imprime no ecrã o botão respetivo a alterar a disposição das nossas cartas.

@param ESTADO	O estado atual
*/
void imprime_botao_change(ESTADO e) {
	
	char script [10240];
	ESTADO novo = e;

	if (novo.change==0)
		novo.change=1;
	else novo.change=0;

	sprintf(script, "%s?%s", SCRIPT, estado2str (novo));
	printf("<a xlink:href = \"%s\"><image x = \"900\" y = \"550\" height = \"80\" width = \"80\" xlink:href = \"http://127.0.0.1/img/changeButton.png\" /></a>\n", script);
}



/** \brief Imprime no ecrã o botão respetivo a mostrar uma sugestão de jogada

@param ESTADO	O estado atual
*/
void imprime_botao_help(ESTADO e) {
	
	char script [10240];
	ESTADO novo = e;

	if (novo.help==0)
		novo.help=1;
	else novo.help=0;

	sprintf(script, "%s?%s", SCRIPT, estado2str (novo));
	printf("<a xlink:href = \"%s\"><image x = \"160\" y = \"610\" height = \"70\" width = \"70\" xlink:href = \"http://127.0.0.1/img/helpButton.png\" /></a>\n", script);
}


/** \brief Imprime no ecrã o botão respetivo a jogar uma nova partida
*/
void imprime_botao_restart () {
	printf("<a xlink:href = \"%s\"><image x = \"500\" y = \"300\" height = \"80\" width = \"80\" xlink:href = \"http://127.0.0.1/img/restartButton.png\" /></a>\n", SCRIPT);
}


/** \brief Imprime o estado

Esta função está a imprimir o estado em quatro colunas: uma para cada naipe
@param path	o URL correspondente à pasta que contém todas as cartas
@param ESTADO	O estado atual
*/
void imprime(char *path, ESTADO e) {

	int n, v, m=0;
	int X[4] = {445, 235, 425, 925};
	int Y[4] = {525, 150, 35, 150};
	int x = X[m], y = Y[m];
	
	MAO aux=e.mao[0];

	if (e.change==0) {
		for (v=0; v<13; v++) {
			for (n=0; n<4; n++) {
				if(carta_existe (aux, n, v)) {
					if (carta_existe (e.selecao, n, v))
						e=imprime_carta (path, x, y-20, e, m, n, v); //subir a carta qd pressionada

					else
						e=imprime_carta (path, x, y, e, m, n, v);

					x+=20;
				}
			}
		}
	}
	else {
		for (n=0; n<4; n++) {
			for (v=0; v<13; v++) {
				if(carta_existe (aux, n, v)) {
					if (carta_existe (e.selecao, n, v))
						e=imprime_carta (path, x, y-20, e, m, n, v); //subir a carta qd pressionada

					else
						e=imprime_carta (path, x, y, e, m, n, v);

					x+=20;
				}
			}
		}

	}


	for(m=1; m<4; m++) {
		int x = X[m], y = Y[m];
		for (v=0; v<13; v++) {
			for (n=0; n<4; n++) {
				if(carta_existe (e.mao[m], n, v)) {
					
					if (m % 2 == 0) // se m par
						x += 20;  // cartas na horizontal
						
					else
						y +=20;  // cartas na vertical

					if (carta_existe (e.selecao, n, v))
						e=imprime_carta (path, x, y-20, e, m, n, v); //subir a carta qd pressionada

					else
						e=imprime_carta (path, x, y, e, m, n, v);

				}
			}
		}
	}

}


/** \brief Imprime o html correspondente a uma carta

@param path	o URL correspondente à pasta que contém todas as cartas
@param x A coordenada x da carta
@param y A coordenada y da carta
@param ESTADO	O estado atual
@param naipe	O naipe da carta (inteiro entre 0 e 3)
@param valor	O valor da carta (inteiro entre 0 e 12)
*/
ESTADO imprime_carta(char *path, int x, int y, ESTADO e, int m, int naipe, int valor) {
	
	char *suit = NAIPES;
	char *rank = VALORES;
	char script[10240];
	
	ESTADO novo = e;


	if (m == 0) {
		
		if (carta_existe (novo.selecao, naipe, valor))
			novo.selecao = rem_carta (novo.selecao, naipe, valor);
			

		else 
			novo.selecao = add_carta (novo.selecao, naipe, valor);

		sprintf(script, "%s?%s", SCRIPT, estado2str (novo));
		printf("<a xlink:href = \"%s\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" /></a>\n", script, x, y, path, rank[valor], suit[naipe]);
	
	}
	
	else {

		//if (m==4) //cartas no centro => entao pode mostra-las
		if (m>=1 && m<5) //cartas no centro => entao pode mostra-las
		 	printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" />\n", x, y, path, rank[valor], suit[naipe]);
		else
			printf("<image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/00.jpg\" />\n", x, y, path);
	}

	return e;
}



/** \brief Primeira jogada

Esta função determina qual jogador tem o terno de ouros.
Caso algum dos bots tenha o terno de ouros, calcula a melhor jogada possível que inclua o tal terno.
@param ESTADO	O estado atual
@return		O novo estado
*/
ESTADO primeiroJogar(ESTADO e) {

	int m, x=400, y=350;

	for(m=0; m<4; m++) {
		
		if(carta_existe (e.mao[m], 0, 0)) {
			
			//calcular as coordenadas da mao que tiver terno de ouros
			switch (m) {
				case 1:	x=320, y=270; break;
				case 2: x=500, y=150; break;
				case 3: x=730, y=270;
				default: ;
			}
			break;
		}
	}

	//joga se o bot tiver o terno de ouros
	if (m>0 && m<4) {
		if (carta_existe (e.mao[m], 0, 0)) {
			
			e.ultima_jogada=-1;
			e=jogar_bot(e, m, x, y);
		}
	}
	//se jogador tiver o terno de ouros
	else {
		e.ultimo_jogador=-1; // nao jogo, mas fico ja com a flag atribuida
		e.selecao=1; //terno de ouros levantado
	}
	return e;
}






/** \brief Joga na mesa uma combinação de cartas selecionadas pelo jogador, caso as possa jogar.

Caso não possa jogar as cartas selecionadas por mim, imprime a ultima jogada.
@param ESTADO	O estado atual
@return		O novo estado
*/
ESTADO jogar (ESTADO e) {

	int n, v, x=500, y=400;

	if (posso_jogar(e)) {
		
		for (v=0; v<13; v++) {
			for (n=0; n<4; n++) {
				if (carta_existe (e.selecao, n, v) && e.selecao!=0) {
					
						
						e=imprime_carta (BARALHO, x, y, e, 4, n, v); // 4-> "mao" na mesa
						e.mao[0] = rem_carta (e.mao[0], n, v);
						e.cartas[0]-=1;
						x +=20;
						e.ultimo_jogador=0;						
				}
			}
		}
		e.ultima_jogada=e.selecao;
	}

	//se nao posso jogar, imprime a ultima jogada
	else {
		for(n=0, x=20; n<4; n++)
			for(v=0;v<13;v++)
				if(carta_existe(e.ultima_jogada,n,v))
					e=imprime_carta (BARALHO, x+=20, 200, e, 4, n,v);
	}

	e.jogar=e.selecao=0;

	return e;

}



/** \brief Verifica se numa determinada seleção de cartas existe uma sequência de 5 cartas (Straight)
@param m		Seleção de cartas
@param valor	Valor de carta pelo qual a função começa a ler as cartas (este valor nuca é decrementado).
@return		Valor do maior valor (da primeira sequência) lido ou -1 se nao existe nenhum straight na seleção.
*/
int straight(MAO m, int valor){ //no flush
	
	int v,n,num=0;
	maostraight=0;

	for(v=valor; v<12; v++) { //V<12, pois nesta variaçao nao existem straights com A no meio, ex: J-Q-K-A-2
		for (n=0; n<4; n++){
			if (carta_existe(m,n,v)) {
				maostraight = add_carta(maostraight,n,v);
				num++;
				if (num==5) return (v);
				break;
			}
			if (n==3) num=0; //se leu os naipes todos e nao existe carta, reinicia valor
		}
	}
	
	return -1;
}



/** \brief Verifica se numa determinada seleção de cartas existem cinco cartas com o mesmo naipe (Flush)
@param m		Seleção de cartas
@param valor	Valor de carta pelo qual a função começa a ler as cartas (este valor nuca é decrementado).
@return		Valor do naipe, menor e maior valores (do primeiro flush) lido ou -1 se nao existe nenhum flush na seleção.
*/
FLUSH flush(MAO m, int valor){
	
	int v,n,num=0;
	MAO aux=0;
	FLUSH flsh;
	
	for(n=0; n<4; n++) {
		for(v=valor; v<13; v++) {
	
			if (carta_existe(m,n,v)) {
				num++;
				aux=add_carta(aux,n,v);
	
				if (num==1) flsh.lowerValue=v;
			}
			
			//se numero de cartas==5 -> return
			if (num==5){
					flsh.highValue=v;
					flsh.suit=n;
					flsh.fmao=aux;

				return flsh;
			}
		}
		num=0;
	}
	
	flsh.lowerValue=flsh.highValue=flsh.suit=-1;
	return flsh;
}



/** \brief Verifica se numa determinada seleção de cartas existe um par e um triplo (Full House).
@param m		Seleção de cartas
@param valor	Valor de carta pelo qual a função começa a ler as cartas (este valor nuca é decrementado).
@return		Valor do triplo e do par (presente no full house) lido ou -1 se nao existe nenhum full house na seleção.
*/
FULLHOUSE fullHouse(MAO m, int valor){

	MAO aux = m;
	FULLHOUSE fh;

	int n, dval;
	int tval; //valor do triplo

	if ( (tval=triplo(m,valor))!=-1 ){
		
		fh.tripleValue=tval;
		//remover triplo da mao
		for(n=0; n<4; n++) {
			if (carta_existe(aux,n,tval))
				aux = rem_carta (aux, n, tval);	
		}
		
		if ( (dval=duplo(aux,0))!=-1){
			fh.doubleValue=dval;
			return fh;
		}
	}
	
	//se nao encontrou
	fh.doubleValue=fh.tripleValue=-1;
	return fh;
	
}



/** \brief Verifica se numa determinada seleção de cartas existem quatro cartas iguais.
@param m		Seleção de cartas
@param valor	Valor de carta pelo qual a função começa a ler as cartas (este valor nuca é decrementado).
@return		Valor do primeiro poker lido ou -1 se nao existe nenhum poker na seleção.
*/
int fourOfaKind(MAO m, int valor)
{
	int num=0; int n,v;
	
	for(v=valor; v<13; v++) {		
		for(n=0; n<4; n++) {
			if (carta_existe(m,n,v))
				num++;
			if (num==4) return v;
		}
		num=0;
	}
	
	return -1;
}


/** \brief Verifica se numa determinada seleção de cartas existe um triplo.
@param m		Seleção de cartas
@param valor	Valor de carta pelo qual a função começa a ler as cartas (este valor nuca é decrementado).
@return		Valor do primeiro triplo lido ou -1 se nao existe nenhum triplo na seleção.
*/
int triplo(MAO m, int valor)
{
	int num=0; int n,v;
	
	for(v=valor; v<13; v++) {		
		for(n=0; n<4; n++) {
			if (carta_existe(m,n,v))
				num++;
			if (num==3) return v;
		}
		num=0;
	}
	
	return -1;
}


/** \brief Verifica se numa determinada seleção de cartas existe um par.
@param m		Seleção de cartas
@param valor	Valor de carta pelo qual a função começa a ler as cartas (este valor nuca é decrementado).
@return		Valor do primeiro par lido ou -1 se nao existe nenhum par na seleção.
*/
int duplo(MAO m, int valor)
{
	int num=0; int n,v;
	
	for(v=valor; v<13; v++) {		
		for(n=0; n<4; n++) {
			if (carta_existe(m,n,v))
				num++;
			if (num==2) return v;
		}
		num=0;
	}
	
	return -1;
}


/** \brief Calcula o número de cartas presentes numa seleção.
@param m	Seleção de cartas
@return		Número de cartas
*/
int contaCartas(MAO m)
{
	int num=0; int n,v;
	for(v=0; v<13; v++)
		for(n=0; n<4; n++)
			if (carta_existe(m,n,v))
				num++;

	return num;
}


/** \brief Calcula o maior naipe que se encontra numa determinada seleção de cartas.
@param m	Seleção de cartas
@return		Maior naipe da carta presente na seleção de cartas
*/
int maiorNaipe(MAO m)
{
	int n,v, mn=0;
	for(n=0; n<4; n++)
		for(v=0; v<13; v++)
			if (carta_existe(m,n,v))
				if (n>mn) mn=n;
	return mn;
}


/** \brief Calcula o maior valor que se encontra numa determinada seleção de cartas.
@param m	Seleção de cartas
@return		Maior valor da carta presente na seleção de cartas
*/
int maiorValor(MAO m)
{
	int n,v, mv=0;
	for(n=0; n<4; n++)
		for(v=0; v<13; v++)
			if (carta_existe(m,n,v))
				if (v>mv) mv=v;	
	return mv;
}



/** \brief Calcula se uma determinada seleçao de cartas é válida baseando-se nas regras do BigTwo. 

A função verifica, caso a seleçao seja de 2 ou 3 cartas, se estas têm o mesmo valor; ou caso seja uma seleção de 5 cartas,
se estas são uma combinação de poker (straight, flush, straight flush, four of a kind ou full house).
@param m	Seleção de cartas
@return		1 se a jogada é valida, 0 se o contrário
*/
int jogada_valida (MAO m) { //cartas iguais (2ou3)

	int v, n, val=0;
	FLUSH flsh;
	FULLHOUSE fh;
	
	if (contaCartas(m)==1) return 1;
	
	if (contaCartas(m)==2 || contaCartas(m)==3) {
		for(v=0; v<13; v++)
			for(n=0; n<4; n++)
				if(carta_existe(m,n,v)){
					val++;
					break; //sai do for naipe e passa para o valor
				}
  
		if (val==1) return 1;
		else return 0;
	}

	//contaCartas==5
	if (straight(m,0)!=-1 || fourOfaKind(m,0)!=-1) return 1;

	fh = fullHouse(m,0);
	if (fh.tripleValue!=-1) return 1;

	flsh = flush(m,0);
	if (flsh.suit!=-1) return 1;

	//caso num de cartas selecionadas diferente de 1,2,3,5 ou nao seja uma combinaçao valida de 5 cartas
	return 0;
}



/** \brief Verifica se uma nova seleção de cartas é maior do que uma última seleção de (1,2 ou 3) cartas
@param m1	Nova seleção de cartas
@param m2	Ultima combinação de cartas jogada
@return		1 se m1>m2, 0 se m1<m2
*/
int combinacao_maior (MAO m1, MAO m2) { //se m1>m2
	
	//para 1,2 ou 3 cartas

	//se maior valor da sel.ecao > ultima_jogada
	if (maiorValor(m1) > maiorValor(m2)) return 1;
	else {
		if (maiorValor(m1) < maiorValor(m2)) return 0;
		else {
			// caso os valores sejam iguais, verifica se o meu naipe é superior ao ultimo_jogado
			if (maiorNaipe(m1) > maiorNaipe(m2)) return 1;
			else return 0;
		}
	}
}



/** \brief Verifica se posso jogar uma determinada combinação de cartas

@param ESTADO	O estado atual
@return		Valor lógico se pode ou nao jogar (1 se verdadeiro, 0 se falso)
*/
int posso_jogar (ESTADO e) {
	
	if (e.ultimo_jogador==0) { //todos passaram
		if (jogada_valida(e.selecao))
			return 1;
	}

	if (e.ultimo_jogador==-1) { //tenho terno de ouros
		//se na selecao nao tenho o terno de ouros ou tenho terno de ouros mais outro valor
		if (carta_existe(e.selecao,0,0)) { //0,0  terno de ouros
			if (jogada_valida(e.selecao))
				return 1;
			else return 0;
		}
		else return 0;
	}

	//se numero de cartas da ultima jogada é igual à minha seleçao
	if ((contaCartas(e.selecao) == contaCartas(e.ultima_jogada)) || e.ultima_jogada==0) {

		//se tenho valores iguais na seleçao
		if (jogada_valida(e.selecao) && combinacao_maior(e.selecao, e.ultima_jogada))
			return 1;
	}

	return 0;
}




/** \brief Calcula o score do jogo
@param ESTADO	O estado atual
@param winner	A mão do jogador vencedor
@return		O novo estado
*/
ESTADO calcularScore(ESTADO e, int winner)
{
	int m, lp; //lost points
	m=lp=0;

	for(m=0; m<4; m++){
		if (m!=winner) {
			
			if(e.cartas[m]==13)
				lp=39;
			else {	
				if(e.cartas[m]<10)
					lp=e.cartas[m];
				else lp=2*e.cartas[m];
			}

			score[m]-=lp;
		}
	}
	return e;
}



/** \brief Verifica se o jogo acabou, se algum bot já não tem cartas

@param ESTADO	O estado atual
@return		O novo estado
*/
int jogoAcabou(ESTADO e)
{
	int m;
	for(m=0; m<4; m++){
		if (e.cartas[m]==0){
			return m;
		}
	}

	return -1;
}



/** \brief Chama os 3 bots, ou seja, invoca as suas jogadas

@param ESTADO	O estado atual
@return		O novo estado
*/
ESTADO call_bots(ESTADO e) {
	
	int winner;

	e = jogar_bot(e,1,320,270);
	if ((winner=jogoAcabou(e))!=-1){
		e=calcularScore(e,winner);
		imprime_botao_restart ();
		return e;
	}

	e = jogar_bot(e,2,500,150);
	if ((winner=jogoAcabou(e))!=-1){
		e=calcularScore(e,winner);
		imprime_botao_restart ();
		return e;
	}

	e = jogar_bot(e,3,730,270);
	if ((winner=jogoAcabou(e))!=-1){
		e=calcularScore(e,winner);
		imprime_botao_restart ();
		return e;
	}

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
	
	// "e" é o nome da struct ESTADO
	ESTADO e;
	
	if (query!= NULL && strlen (query)!= 0 ) {
	
		e = str2estado (query);

		if (e.jogar == 1) {
			e = jogar (e);

			if (e.ultimo_jogador == 0) {
				e = call_bots(e); //os bots jogam se eu joguei
				e.selecao=0;
			}
		}


		//se passei
		if (e.passar==1) { //passar o jogo
			e = call_bots(e);
			e.passar=0;
			e.jogar=0;
			e.selecao=0;
		}
	}

	
	//entra aqui uma unica vez (apenas no inicio do jogo para distribuir as cartas e imprimir as maos)
	else {

		e = distribuir (); //da cartas aleatorias a cada mao
		
		int x,y;
		int proxM; //proximo jogador
		
		e.selecao=0;
		e.ultima_jogada=0;

		// joga terno de ouros
		e = primeiroJogar(e);

		//os bots seguintes jogam automaticamente depois do terno de ouros jogado
		if (e.ultimo_jogador!=-1) {

			proxM =e.ultimo_jogador+1;

			//acaba quando o proximo jogador for 3
			while (proxM==2 || proxM==3)
			{
				if (proxM==2) {x=500;y=150;}
				else {x=730;y=270;} //bot 3

				e = jogar_bot(e,proxM++,x,y);
				e.selecao=0;
			}
		}	
	}


	int m,zero=0;
	for(m=0; m<4; m++) {
		if (contaCartas(e.mao[m])==0)
			{zero++; break;}
	}

	if (!zero) {
		imprime_botao_passar (e);
		imprime_botao_jogar (e);
	}

	imprime_botao_limpar (e);
	imprime_botao_home();
	imprime_botao_change(e);
	imprime_botao_help(e);

	imprime (BARALHO, e);
	
	printf("<text size=\"6\" x=\"70\" y=\"190\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\">Ultima jogada:</text>\n");
	printf("<text size=\"6\" x=\"90\" y=\"350\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\">Sugestão de jogada:</text>\n");
	printf("<image x = \"760\" y = \"530\" height = \"100\" width = \"120\" xlink:href = \"http://127.0.0.1/img/score.png\" />\n");
	printf("<text size=\"6\" x=\"820\" y=\"600\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\">%d</text>\n",score[0]);
	printf("<image x = \"120\" y = \"170\" height = \"100\" width = \"120\" xlink:href = \"http://127.0.0.1/img/score.png\" />\n");
	printf("<text size=\"6\" x=\"180\" y=\"240\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\">%d</text>\n",score[1]);
	printf("<image x = \"330\" y = \"30\" height = \"100\" width = \"120\" xlink:href = \"http://127.0.0.1/img/score.png\" />\n");
	printf("<text size=\"6\" x=\"390\" y=\"100\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\">%d</text>\n",score[2]);
	printf("<image x = \"1000\" y = \"170\" height = \"100\" width = \"120\" xlink:href = \"http://127.0.0.1/img/score.png\" />\n");
	printf("<text size=\"6\" x=\"1060\" y=\"240\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\">%d</text>\n",score[3]);
	
	//sugestao de jogada
	if (e.help==1) e=jogar_bot(e,0,10,360);	


}



/** \brief Atribui 13 cartas aleatórias a cada uma das mãos e inicializa a estrutura ESTADO

@return		O novo estado
*/
ESTADO distribuir() {

	ESTADO e = {{0}, {0}, 0, 0, {0}, 0, 0, 0, 0, 0 ,0}; // Inicialização da estrutura ESTADO
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
	printf("<header> <title> Big Two - Jogo </title> </header>\n");

	//background img
	printf("<body background=\"%s\" >\n",BACKGROUND);
	printf("<svg height = \"680\" width = \"1200\" >\n");

	//titulo principal
	printf("<rect x= \"0\" y= \"0\" width=\"1000\" height=\"30\" ry=\"5\" fill =#a0a0a0 />");
	printf("<text size=\"6\" x=\"600\" y=\"15\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font color = \"#a0a0a0\" font-weight = \"bold\">BIG TWO </text>\n");
	
	srandom(time(NULL));

/*
 * Ler os valores passados à cgi que estão na variável ambiente e passá-los ao programa
 */
	parse(getenv("QUERY_STRING"));

	printf("</svg>\n");
	printf("</body>\n");
	printf("</html>\n");
	return 0;
}
