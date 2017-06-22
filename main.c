//Made by Matheus Bigarelli
//Computer Engineering student at UTFPR - Brazil

#include <stdio.h>
#include <stdlib.h>


#define N_TIPOS_DE_NAVIOS 3

#define ALTURA 8
#define LARGURA 8


typedef struct
{
	int comprimento;
} Navio;


typedef struct
{
	int altura, largura;
	int** grid;
	Navio navios[N_TIPOS_DE_NAVIOS];
} Tabuleiro;



int time ();
Tabuleiro criaTabuleiro ();
void criaNavios (Tabuleiro* tabuleiro);
void imprimeTabuleiro (Tabuleiro* tabuleiro, int* tem_navio_pra_afundar, int jogadas_restantes);
void fazProximoAtaque (Tabuleiro* tabuleiro);
void revelaNavios (Tabuleiro* tabuleiro);
void destroiTabuleiro (Tabuleiro* tabuleiro);



int main ()
{
	Tabuleiro tabuleiro;
	int tem_navio_pra_afundar, jogadas_restantes;
	int quer_jogar = 1;

	srand (time (NULL));

	/*O loop de fora serve para ter várias partidas*/
	while (quer_jogar)
	{
		tabuleiro = criaTabuleiro ();
		tem_navio_pra_afundar = 1;
        jogadas_restantes = 35;

		/*Aqui tem o jogo inteiro*/
		while (tem_navio_pra_afundar && jogadas_restantes)
		{
			imprimeTabuleiro (&tabuleiro, &tem_navio_pra_afundar, jogadas_restantes);
			fazProximoAtaque (&tabuleiro);
            jogadas_restantes--;
		}

		imprimeTabuleiro (&tabuleiro, &tem_navio_pra_afundar, jogadas_restantes);

		if (tem_navio_pra_afundar)
		{
            printf ("Voce perdeu :(\n");
			revelaNavios(&tabuleiro);
		}
        else
            printf ("Parabens!! Voce ganhou!! :D\n");

		printf("GAME OVER\nJogar Novamente? [1] [0]\n");
		fflush (stdin);
		
		scanf("%d", &quer_jogar);

		destroiTabuleiro (&tabuleiro);
	}

	return 0;
}



Tabuleiro criaTabuleiro ()
{
	Tabuleiro tabuleiro;
	int linha, coluna;

	tabuleiro.altura = ALTURA;
	tabuleiro.largura = LARGURA;

	/*Aloca grid do jogo*/
	//Made by Matheus Bigarelli
	tabuleiro.grid = (int**) malloc (sizeof (int*) * tabuleiro.altura);
	for (linha = 0; linha < tabuleiro.altura; linha++)
		tabuleiro.grid [linha] = malloc (sizeof (int) * tabuleiro.largura);

	/*Zera o grid*/
	for (linha = 0; linha < tabuleiro.altura; linha++)
		for (coluna = 0; coluna < tabuleiro.largura; coluna++)
			tabuleiro.grid[linha][coluna] = 0;

	criaNavios (&tabuleiro);

	return tabuleiro;
}


void criaNavios (Tabuleiro* tabuleiro)
{
	int tipo, x_navio, y_navio, navios_pra_colocar, posicao_valida;
	int linha, coluna;

    srand (rand ());
	/*Determina a quantidade de navios que o jogo terá*/
	navios_pra_colocar = rand () % (tabuleiro->altura / 2) + 3;

	/*SUB*/
	tabuleiro->navios[0].comprimento = 1;

	/*BATTLESHIP*/
	tabuleiro->navios[1].comprimento = 2;

	/*AIR CARRIER*/
	tabuleiro->navios[2].comprimento = 3;


	/*Posiciona os navios no grid*/
	while (navios_pra_colocar)
	{
	    srand (rand ());
		tipo = rand () % N_TIPOS_DE_NAVIOS;


		/*Coloca na horizontal*/
		if (rand () % 2)
		{
			do
			{

				/*Primeira posição do navio*/
                srand (rand ());
				x_navio = rand () % tabuleiro->largura;
				srand (rand ());
				y_navio = rand () % tabuleiro->altura;

				posicao_valida = 1;

				/*Verifica se o barco não vai sair do grid a partir da posição inicial*/
				if (x_navio + tabuleiro->navios[tipo].comprimento < tabuleiro->largura) {
					/*Verifica se já tem algum barco em alguma posição que este colocado agora ocuparia*/
					for (coluna = x_navio; coluna < x_navio + tabuleiro->navios[tipo].comprimento; coluna++)
						if (tabuleiro->grid[y_navio][coluna] == 1)
							posicao_valida = 0;
				}

				else
					posicao_valida = 0;

			} while (!posicao_valida);
			//Made by Matheus Bigarelli

			/*Preenche as posições do barco no grid*/
			for (coluna = x_navio; coluna < x_navio + tabuleiro->navios[tipo].comprimento; coluna++)
				tabuleiro->grid[y_navio][coluna] = 1;
		}

		/*Coloca na vertical*/
		else
		{
			do
			{
				/*Primeira posição do navio*/
				srand (rand ());
				x_navio = rand () % tabuleiro->largura;
				srand (rand ());
				y_navio = rand () % tabuleiro->altura;

				posicao_valida = 1;

				/*Verifica se o barco não vai sair do grid a partir da posição inicial*/
				if (y_navio + tabuleiro->navios[tipo].comprimento < tabuleiro->altura) {
					/*Verifica se já tem algum barco em alguma posição que este colocado agora ocuparia para não colocar um navio em cima de outro*/
					for (linha = y_navio; linha < y_navio + tabuleiro->navios[tipo].comprimento; linha++)
						if (tabuleiro->grid[linha][x_navio])
							posicao_valida = 0;
				}

				else
					posicao_valida = 0;

			} while (!posicao_valida);

			/*Preenche as posições do barco no grid*/
			for (linha = y_navio; linha < y_navio + tabuleiro->navios[tipo].comprimento; linha++)
				tabuleiro->grid[linha][x_navio] = 1;
		}

		navios_pra_colocar--;
	}
}




void imprimeTabuleiro (Tabuleiro* tabuleiro, int* tem_navio_pra_afundar, int jogadas_restantes)
{
	int linha, coluna, tem_navio = 0;

	system ("clear");

	for (linha = 0; linha < tabuleiro->altura; linha++)
	{
		for (coluna = 0; coluna < tabuleiro->largura; coluna++)
		{
			printf("---");
			if (tabuleiro->grid[linha][coluna] == 1)
				tem_navio = 1;
		}

		printf("-\n|");
		for (coluna = 0; coluna < tabuleiro->largura; coluna++)
		{

			/*Atingiu água*/
			if (tabuleiro->grid[linha][coluna] == -1)
				printf("~~");

			/*Não atacado ainda*/
			else if (tabuleiro->grid[linha][coluna] == 0 || tabuleiro->grid[linha][coluna] == 1)
				printf("  ");

			/*Atingiu navio*/
			else
				printf("><");

			printf("|");
		}

		printf("\n");
	}


    printf ("Voce tem %d jogadas restantes\n", jogadas_restantes);

	*tem_navio_pra_afundar = tem_navio;
}



void fazProximoAtaque (Tabuleiro* tabuleiro)
{
	int ataque_x, ataque_y;

	printf("Coordenadas do proximo ataque:\nExemplo: 2,3\n");
	scanf ("%d,%d", &ataque_x, &ataque_y);

	ataque_x--;
	ataque_y--;

	/*Atingiu navio*/
	if (tabuleiro->grid[ataque_y][ataque_x] == 1)
		tabuleiro->grid[ataque_y][ataque_x] = 2;

	/*Atingiu água*/
	if (tabuleiro->grid[ataque_y][ataque_x] == 0)
		tabuleiro->grid[ataque_y][ataque_x] = -1;
}


void revelaNavios (Tabuleiro* tabuleiro)
{
	int linha, coluna;

	system("clear");

	for (linha = 0; linha < tabuleiro->altura; linha++)
	{
		for (coluna = 0; coluna < tabuleiro->largura; coluna++)
			printf("---");
		printf("-\n|");

		for (coluna = 0; coluna < tabuleiro->largura; coluna++)
		{

			/*Água*/
			if (tabuleiro->grid[linha][coluna] == -1 || !tabuleiro->grid[linha][coluna])
				printf("~~");

			/*Navio*/
			else
				printf("><");

			printf("|");
		}

		printf("\n");
	}
}


void destroiTabuleiro (Tabuleiro* tabuleiro)
{
	int linha;

	/*Desaloca grid*/
	for (linha = 0; linha < tabuleiro->altura; linha++)
		free (tabuleiro->grid[linha]);
	free (tabuleiro->grid);
}
//Made by Matheus Bigarelli