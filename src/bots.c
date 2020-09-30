#include "estruturas.h"


/** \brief Jogada do jogador eletrónico

Esta função lê a ultima jogada, e calcula a nova jogada do bot baseada na última jogada, qual o número de cartas a deitar.
Calcula as combinações de 2, 3 e 5 cartas.
Inteligência dos jogadores eletrónicos.
@param ESTADO	O estado atual
@param m	Número do bot (1 a 3)
@param x    Coordenada horizontal do jogador
@param y    Coordenada vertical do jogador
@return		O novo estado
*/
ESTADO jogar_bot(ESTADO e,int m,int x, int y) {

	int n=0, v=0, mv=-1, mn=-1;
	int counter=0;
	MAO aux=0;
	FULLHOUSE fh;
	FLUSH flsh;


	//o que o bot vai jogar
	e.selecao=0;

	if (e.cartas[m]==0) return e;

	if (m==e.ultimo_jogador)
		e.ultima_jogada=-2; //se 3 jogadores passaram


	aux=e.mao[m];

	//inicializar jogada
	e.jogada[m-1]=0;


	if (contaCartas(e.ultima_jogada) == 5  || e.ultima_jogada==-1 || e.ultima_jogada==-2) {


		//sequencia
		if (e.ultima_jogada!=-1 && e.ultima_jogada!=-2)
			mv=straight(e.ultima_jogada, 0);
		else mv=straight(e.mao[m], 0);
		flsh=flush(maostraight,0);
		if ((( e.ultima_jogada==-1 && (mv-4)==0) || (e.ultima_jogada==-2 && mv!=-1) || (e.ultima_jogada!=-1 && mv!=-1)) && flsh.suit==-1){

			/*if (e.ultima_jogada==-1)
				mv=4;*/

			int svalor;

			//svalor=straight(e.mao[m], (mv-4)+1);
			svalor=straight(e.mao[m], (mv-4));
			//verfica se a sequencia nao é um flush (para distinguir do straightFlush)
			flsh=flush(maostraight,0);

		 	if ( svalor!=-1 && flsh.suit==-1) {

		 		counter=0;
				for (v=(svalor-4); v<12; v++) {
					for (n=0; n<4; n++) {
						if (carta_existe (aux, n, v)) {

							aux = rem_carta (aux, n, v);
							e=imprime_carta (BARALHO, x, y, e, 4, n, v);
		            		x +=20;
							e.selecao = add_carta (e.selecao , n , v);
			           		
							counter++;
							if (counter==5) {

								e.ultima_jogada = e.selecao;
								e.ultimo_jogador=m;
								e.cartas[m]-=5;
								e.mao[m]=aux;
								e.jogada[m-1]=e.selecao;
								return e;
							}
							else break; //se imprimiu valor, sai do naipe for e passa para o valor for com v++
						}
					}
				}
			}
			else goto flush;
		}

		
		//mesmo naipe
		if (e.ultima_jogada!=-1 && e.ultima_jogada!=-2)
			flsh=flush(e.ultima_jogada, 0);
		else flsh=flush(e.mao[m], 0);
		if (((e.ultima_jogada==-1 && flsh.lowerValue==0 && flsh.suit==0) || (e.ultima_jogada==-2 && flsh.suit!=-1) || (e.ultima_jogada!=-1 && flsh.suit!=-1)) && straight(flsh.fmao,0)==-1){

			int highValuePlayed = flsh.highValue;
			int highSuitPlayed = flsh.suit;
			v=0;

						
			//apenas entra aqui se for invocado pelo goto "flush"	ou se primeiroAjogar
			if (e.ultima_jogada==-1 || e.ultima_jogada==-2) {flush:
				highValuePlayed=0;
				highSuitPlayed=0;
 			}

			//procurar um flush superior à jogada anterior (constroi flush a partir do valor mais baixo se possivel)
			do {
				flsh=flush(e.mao[m],v);
				v++;
				if (flsh.suit>highSuitPlayed) break;
				if (flsh.highValue>highValuePlayed && flsh.suit==highSuitPlayed) break;
			} while (flsh.suit!=-1 && flsh.suit<=highSuitPlayed && v<9 && straight(flsh.fmao,0)==-1); //v<9 => enquanto valor menor que J, pois de J a 2, há apenas 4 cartas

			if (flsh.suit!=-1 && ( (flsh.highValue>highValuePlayed && flsh.suit==highSuitPlayed) || flsh.suit>highSuitPlayed )) {

			 	for (v=flsh.lowerValue; v<13; v++) {
			 		
			 		if (carta_existe (aux, flsh.suit, v)) {

						aux = rem_carta (aux, flsh.suit, v);
						e=imprime_carta (BARALHO, x, y, e, 4, flsh.suit, v);
	            		x +=20;
						e.selecao = add_carta (e.selecao, flsh.suit, v);
		           		
		           		counter++;
						if (counter==5) {
							e.mao[m]=aux;
							e.cartas[m]-=5;
							e.ultimo_jogador=m;
							e.ultima_jogada = e.selecao;
							e.jogada[m-1]=e.selecao;
							return e;
				 		}
			 		}
				}
			}
			else goto fullhouse;
		}


		

		//3+2cartas
		if (e.ultima_jogada!=-1 && e.ultima_jogada!=-2)
			fh=fullHouse(e.ultima_jogada, 0);
		else fh=fullHouse(e.mao[m], 0);
		if ( ( e.ultima_jogada==-1 && fh.tripleValue==0) || (e.ultima_jogada==-2 && fh.tripleValue!=-1) || (e.ultima_jogada!=-1 && fh.tripleValue!=-1 && fh.doubleValue!=-1) ) {
			

			//apenas entra aqui se for invocado pelo goto "fullhouse"

			//if (0) {fullhouse:
			if (e.ultima_jogada==-1 || e.ultima_jogada==-2) {fullhouse:
				fh.tripleValue=-1;
			}


			fh=fullHouse(e.mao[m], fh.tripleValue+1);
			if (fh.tripleValue!=-1 && fh.doubleValue!=-1) {
				
				for (n=0; n<4; n++) {
			
					if(carta_existe (aux, n, fh.tripleValue)) {
						
						aux = rem_carta (aux, n, fh.tripleValue);
						e=imprime_carta (BARALHO, x, y, e, 4, n, fh.tripleValue);
	            		x +=20;

	            		e.selecao = add_carta (e.selecao , n , fh.tripleValue);
		           		
		           		counter++;
						if (counter==3) {
							e.mao[m]=aux;
							counter=0;
							
							for (n=0; n<4; n++) {
								 if (carta_existe (aux, n, fh.doubleValue)) {
									
									aux = rem_carta (aux, n, fh.doubleValue);
									e=imprime_carta (BARALHO, x, y, e, 4, n, fh.doubleValue);
				            		x +=20;
				            		e.selecao = add_carta (e.selecao , n , fh.doubleValue);

									counter++;
									if (counter==2) {
										e.mao[m]=aux;
										e.cartas[m]-=5;
										e.ultimo_jogador=m;
										e.ultima_jogada = e.selecao;
										e.jogada[m-1]=e.selecao;
										return e;
									}
								}
							}
						}
					}
				}
			}
			else goto fourofakind;
		}


		//4cartas+1
		if ( (mv=fourOfaKind(e.ultima_jogada,0))!=-1 || e.ultima_jogada==-1 || e.ultima_jogada==-2) {

			
			//apenas entra aqui se for invocado pelo goto "fourofakind"
			if (e.ultima_jogada==-1 || e.ultima_jogada==-2) {fourofakind:
				mv=0;
			}

			int foakval;
			if ( (foakval=fourOfaKind(e.mao[m],mv))!=-1) {

				for (n=0; n<4; n++) {

					if (carta_existe (aux, n, foakval)) {
				
						aux = rem_carta (aux, n, foakval);
						e=imprime_carta (BARALHO, x, y, e, 4, n, foakval);
						x +=20;
						e.selecao = add_carta (e.selecao , n , foakval);

						counter++;
						if (counter==4) { //imprimir a menor carta

							for (v=0; v<13; v++) {
								for (n=0; n<4; n++) {
									if (carta_existe (aux, n, v)) {
										
										aux = rem_carta (aux, n, v); //bug acho que posso remover o aux, desnecessario
										e=imprime_carta (BARALHO, x, y, e, 4, n, v);
						           		x +=20;
						           		e.selecao = add_carta (e.selecao , n , v);
						           		
										e.mao[m]=aux;
						           		e.ultima_jogada = e.selecao;
										e.cartas[m]-=5;
										e.ultimo_jogador=m;
										e.jogada[m-1]=e.selecao;
										return e;
									}
								}
							}
						}
					}
				}
			}
			else goto straightflush;
		}



		//STRAIGHTFLUSH
		if (e.ultima_jogada!=-1 || e.ultima_jogada!=-2)
			mv=straight(e.ultima_jogada, 0);
		else mv=straight(e.mao[m], 0);
		flsh=flush(maostraight,0);
		if ((( e.ultima_jogada==-1 && (mv-4)==0) || (e.ultima_jogada!=-1 && mv!=-1)) && flsh.suit!=-1){

/*			if (e.ultima_jogada==-1)
				mv=4;
*/
			if (e.ultima_jogada==-1 || e.ultima_jogada==-2) {straightflush:
				mv=4;
			}

			int svalor;

			//svalor=straight(e.mao[m], (mv-4)+1);
			svalor=straight(e.mao[m], (mv-4));
			flsh=flush(maostraight,0);
		 	if ( svalor!=-1 && flsh.suit!=-1) {

		 		counter=0;
				for (v=(svalor-4); v<12; v++) {
					for (n=0; n<4; n++) {
						if (carta_existe (aux, n, v)) {

							aux = rem_carta (aux, n, v);
							e=imprime_carta (BARALHO, x, y, e, 4, n, v);
		            		x +=20;
							e.selecao = add_carta (e.selecao , n , v);
			           		
							counter++;
							if (counter==5) {

								e.ultima_jogada = e.selecao;
								e.ultimo_jogador=m;
								e.cartas[m]-=5;
								e.mao[m]=aux;
								e.jogada[m-1]=e.selecao;
								return e;
							}
							else break; //se imprimiu valor, sai do naipe for e passa para o valor for com v++
						}
					}
				}
			}
		}


	}



	if (contaCartas(e.ultima_jogada) == 3 || e.ultima_jogada==-1 || e.ultima_jogada==-2) {

		if (e.ultima_jogada!=-1 && e.ultima_jogada!=-2)
			mv=triplo(e.ultima_jogada,0);
		else mv=triplo(e.mao[m],0);
		if ( (e.ultima_jogada==-1 && mv==0) || (e.ultima_jogada!=-1 && mv!=-1) || (e.ultima_jogada==-2 && mv!=-1)) {

			int tval;
			
			if ( (tval=triplo(e.mao[m], mv))!=-1 ) {
				
				for (n=0; n<4; n++) {
					
					if(carta_existe (aux, n, tval)){
						
						aux = rem_carta (aux, n, tval);
						e=imprime_carta (BARALHO, x, y, e, 4, n, tval);
	            		x +=20;

	            		e.selecao = add_carta (e.selecao , n , tval);
		           		counter++;
		           		
						if (counter==3) {
							e.mao[m]=aux;
							e.cartas[m]-=3;
							e.ultimo_jogador=m;
							e.ultima_jogada = e.selecao;
							e.jogada[m-1]=e.selecao;
							return e;
						}
					}
				}
			}
		}
	}



	//if (contaCartas(e.ultima_jogada) == 2) {
	if (contaCartas(e.ultima_jogada) == 2 || e.ultima_jogada==-1 || e.ultima_jogada==-2) {	
		
		if (e.ultima_jogada!=-1 && e.ultima_jogada!=-2)
			mv=duplo(e.ultima_jogada,0);
		else mv=duplo(e.mao[m],0);
		if ( (e.ultima_jogada==-1 && mv==0) || (e.ultima_jogada!=-1 && mv!=-1) || (e.ultima_jogada==-2 && mv!=-1) ) {		

			if (e.ultima_jogada!=-1 && e.ultima_jogada!=-2)
				if (maiorNaipe(e.ultima_jogada)==3)	//se o par jogado possuir uma espada, o maior valor incrementa para procurar duplo de naipe superior
					mv++;
			
			int dval;
			if ( (dval=duplo(e.mao[m], mv)) !=-1 ) {
				
				for (n=0; n<4; n++) {
					
					if(carta_existe (aux, n, dval)){
						
						aux = rem_carta (aux, n, dval);
						e=imprime_carta (BARALHO, x, y, e, 4, n, dval);
	            		x +=20;

	            		e.selecao = add_carta (e.selecao , n , dval);
		           		counter++;

						if (counter==2) { //se ja imprimiu 2 cartas
							e.mao[m]=aux;
							e.cartas[m]-=2;
							e.ultimo_jogador=m;
							e.ultima_jogada = e.selecao;
							e.jogada[m-1]=e.selecao;
							return e;
						}
					}
				}
			}
		}
	}


	if (contaCartas(e.ultima_jogada) == 1 || e.ultima_jogada==-1 || e.ultima_jogada==-2) {
		
		if (e.ultima_jogada!=-1 && e.ultima_jogada!=-2){
			//maior naipe
			mn=maiorNaipe(e.ultima_jogada);
			//maior valor
			mv=maiorValor(e.ultima_jogada);
		}

		if (e.ultima_jogada==-2) {
			mn=mv=0;
		}


		//joga terno de ouros (primeiro jogador)
		if (e.ultima_jogada==-1) {
					e.mao[m] = rem_carta (e.mao[m], 0, 0);
					e=imprime_carta (BARALHO, x, y, e, 4, 0, 0);
					e.cartas[m]=e.cartas[m]-1;
					e.ultimo_jogador = m;
					e.ultima_jogada=1;
					e.jogada[m-1]=e.selecao;
					return e;
		}

		//joga se tiver carta do mesmo valor mas com naipe superior
		for (n=mn; n<4; n++) {
			if (carta_existe (e.mao[m], n, mv)) {

				e.mao[m] = rem_carta (e.mao[m], n, mv);
				e=imprime_carta (BARALHO, x, y, e, 4, n, mv);
        		
        		e.selecao = add_carta (e.selecao , n , mv);
           		e.ultima_jogada = e.selecao;
				
				x +=20;
				e.cartas[m]-=1;
				e.ultimo_jogador=m;
				e.jogada[m-1]=e.selecao;
				return e;
			}
		}

		//começa por procurar a partir do valor jogado
		for (v=mv+1; v<13; v++) {
			for (n=0; n<4; n++) {
				if (carta_existe (e.mao[m], n, v)) {

					e.mao[m] = rem_carta (e.mao[m], n, v);
					e=imprime_carta (BARALHO, x, y, e, 4, n, v);
            		
            		e.selecao = add_carta (e.selecao , n , v);
	           		e.ultima_jogada = e.selecao;
					
					x +=20;
					e.cartas[m]-=1;
					e.ultimo_jogador=m;
					e.jogada[m-1]=e.selecao;
					return e;
					}
			}
		}
	}
	
	//se chegou ate aqui o bot passou

	//imprime a menor carta
	
	// caso saiu do ciclo, ou seja chegou ate aqui, significa que o bot nao tem carta superior, logo passa.
	printf("<image x = \"%d\" y = \"%d\" height = \"120\" width = \"120\" xlink:href = \"http://127.0.0.1/img/passeiButton.png\" />\n",x,y);
	
	return e;
	
}
