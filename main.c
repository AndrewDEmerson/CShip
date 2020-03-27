//Created by Andrew Emerson
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define maxShipLength 5

struct ship
{
	int length;					//The length in map tiles of a given ship, 0 = open ocean
	int spacial[maxShipLength]; //The index of each tile a given ship occupies
	bool status[maxShipLength]; //tells if the ship has been hit at the corosponding spacial index
	char display;
};

void printMap();
void shipDebug(struct ship *);
void placeShip(int, int, struct ship *);
struct ship *strikeShip(int);

//Global
int gridSizeX = 8, gridSizeY = 8; //grid size
struct ship **grid;				  //map the game is played on.
int main(int argc, char **argv)
{
	//create a grid covered by a 0-length ship.
	struct ship empty;
	empty.length = 0;
	empty.display = '-';
	grid = malloc(gridSizeX * gridSizeY * sizeof(struct ship *));
	for (int i = 0; i < gridSizeX * gridSizeY; i++)
	{
		*(grid + i) = &empty;
	}

	struct ship testshp;
	testshp.length = 2;
	testshp.display = 'X';
	placeShip(12, 2, &testshp);

	struct ship testshp2;
	testshp2.length = 3;
	testshp2.display = 'Y';
	placeShip(17, 1, &testshp2);

	struct ship testshp3;
	testshp3.length = 5;
	testshp3.display = 'Z';
	placeShip(61, 3, &testshp3);

	while (true)
	{
		printMap();
		int t = 0;
		scanf("%i", &t);
		if (t < 0)
		{
			break;
		}
		struct ship *strike = strikeShip(t);
	}
	free(grid);
	return 0;
}

void placeShip(int head, int dir, struct ship *shp)
{
	//WARNING, no out of bounds checking when ship is placed, will overflow to next line
	int length = shp->length;
	if (dir == 0) //to right
	{
		for (int i = 0; i < length; i++)
		{
			*(grid + head + i) = shp;
			shp->spacial[i] = head + i;
		}
	}
	else if (dir == 2)
	{ //to left
		for (int i = 0; i < length; i++)
		{
			*(grid + head - length + 1 + i) = shp;
			shp->spacial[i] = head - length + 1 + i;
		}
	}
	else if (dir == 1)
	{ //downward
		for (int i = 0; i < length; i++)
		{
			*(grid + head + i * gridSizeX) = shp;
			shp->spacial[i] = head + i *gridSizeX;
		}
	}
	else if (dir == 3)
	{ //upward
		for (int i = 0; i < length; i++)
		{
			*(grid + head + (1-length+i)*gridSizeX) = shp;
			shp->spacial[i] = head + (1-length+i)*gridSizeX;
		}
	}
}

struct ship *strikeShip(int cord)
{
	struct ship *shp = *(grid + cord);
	if (shp->length)
	{
		for (int i = 0; i < shp->length; i++)
		{
			if (shp->spacial[i] == cord)
			{
				shp->status[i] = false;
				break;
			}
		}
	}
	shipDebug(shp);
	return shp;
}

void shipDebug(struct ship *shp)
{
	printf("Ship Info:\n\tIcon:\t%c\n\tLength:\t%i\n\tSpaces:\t", shp->display, shp->length);
	for (int i = 0; i < shp->length; i++)
	{
		printf("%i, ", shp->spacial[i]);
	}
	printf("\n\tStatus:\t");
	for (int i = 0; i < shp->length; i++)
	{
		if (shp->status[i])
		{
			printf("0, ");
		}
		else
		{
			printf("X, ");
		}
	}
	printf("\n");
}

void printMap()
{
	for (int i = 0; i < gridSizeX * gridSizeY; i++)
	{
		struct ship *tmp = *(grid + i);
		printf("%c ", tmp->display);
		if (i % gridSizeX == gridSizeX - 1)
		{
			printf("\n");
		}
	}
}
