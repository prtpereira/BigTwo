#include <stdio.h>

typedef struct carta {
    int naipe;
    int valor;
} CARTA;

typedef CARTA *MAO;

int contar_valores(MAO mao, int tam, int *valores) {
    int i, c;
    int maior = 0;

    for(i = 0; i < 13; i++)
        valores[i] = 0;

    for(i = c = 0; i < tam; i++) {
        valores[mao[i].valor]++;
        if(valores[mao[i].valor] > maior)
            maior = valores[mao[i].valor];
    }
    return maior;
}

int main() {
    CARTA mao[] = {
        {2, 7},
        {2, 0},
        {3, 7},
        {0, 2},
        {3, 12},
        {2, 3},
        {0, 11},
        {3, 0},
        {1, 5},
        {0, 5},
        {2, 4},
        {2, 1},
        {2, 7}
    };
    int i, valores[13];

    printf("%d\t", contar_valores(mao, 13, valores));
    for(i = 0; i < 13; i++) {
		printf("%3d", valores[i]);
	}
	printf("\n");
    return 0;
}

