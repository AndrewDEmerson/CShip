//Created by Andrew Emerson
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "gamelogic.h"

int main(int argc, char **argv)
{
	srand((unsigned)time(NULL));
	initGrids();

	compShips = malloc(numShips*sizeof(struct ship));
	initShip(5,'A', compShips + 0);
	initShip(4,'B', compShips + 1);
	initShip(3,'C', compShips + 2);
	initShip(3,'D', compShips + 3);
	initShip(2,'E', compShips + 4);
	genShipLayout(compShips, compgrid);

	playerShips = malloc(numShips*sizeof(struct ship));
	initShip(5,'F', playerShips + 0);
	initShip(4,'G', playerShips + 1);
	initShip(3,'H', playerShips + 2);
	initShip(3,'J', playerShips + 3);
	initShip(2,'K', playerShips + 4);
	genShipLayout(playerShips, playergrid);

	while (true)
	{
		printMap(compgrid);
		printf("\n");
		printMap(playergrid);
		int t = 0;
		scanf("%i", &t);
		if (t < 0)
		{
			break;
		}
		struct ship *strike = strikeShip(t, compgrid);
	}
	free(compgrid);
	free(compShips);
	return 0;
}