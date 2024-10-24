#include <stdio.h>
#include <stdlib.h>
#include "take6.h"

int main(int argc, char const *argv[])
{
	while (1)
	{
		int32_t option=0;
		printf( YEL_BOLD"~~~~~~~~~~~~~~~~~~~~~~\n"RESET );
		printf( YEL_BOLD"|"RESET );
		printf(	MAG_BOLD" Welcome to TAKE 6! "RESET );
		printf( YEL_BOLD"|\n"RESET );
		printf( YEL_BOLD"~~~~~~~~~~~~~~~~~~~~~~\n"RESET );
		printf( CYAN_BOLD"\n--------------\n"RESET );
		printf( CYAN_BOLD"(1) Start game\n"RESET );
		printf( CYAN_BOLD"(2) Game rules\n"RESET );
		printf( CYAN_BOLD"(3) Exit\n"RESET );
		printf( CYAN_BOLD"--------------\n"RESET );
		printf( MAG_BOLD"Enter your option...>"RESET );
		scanf( "%d", &option );

		CLEAR_INPUT_BUFFER;

		if (option==1)
		{
			int32_t mode=0, playerNum=0, level=0;

			menu(&mode, &playerNum, &level);

			sCard deck[DECK_SIZE]={}, playerCard[playerNum][MAX_CARD];
			setup(deck);
			//printDeck(deck);

			shuffleDeal(deck, playerCard, playerNum);
			//printDeck(deck);
			//printPlayerCard(playerCard, playerNum);

			PAUSE;
			system( "clear" );

			game(deck, playerCard, playerNum, level);
		}
		else if (option==2)
		{
			
		}
		else if (option==3)
		{
			exit(EXIT_FAILURE);
		}
		else
		{
			printf( RED_BOLD"Invalid option! Please input again!\n"RESET );
		}

		PAUSE;
		system( "clear" );
	}

	return 0;
}