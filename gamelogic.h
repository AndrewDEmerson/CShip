#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define maxShipLength 5

typedef struct Ship
{
	int length;					//The length in map tiles of a given ship, 0 = open ocean
	int spacial[maxShipLength]; //The index of each tile a given ship occupies
	bool status[maxShipLength]; //tells if the ship has been hit at the corosponding spacial index
	char display;
} ship;

int gridSizeX = 10, gridSizeY = 10; //grid size
ship **compgrid;					//map the game is played on.
ship **playergrid;
ship empty;
ship *playerShips;
ship *compShips;

//function decelerations
void printMap(ship **grid);						 //takes a gamegrid and prints it to the terminal
void shipDebug(ship *);							 //takes a ship and displays all of its variables
void genShipLayout(ship *, int, ship **);			 //randomly place a set of ships on a given grid
int placeShip(int, int, ship *shp, ship **grid); //places a single ship on a given grid
ship *strikeShip(int, ship **grid);				 //returns a ship at a given position on a given grid
ship *initShips(int);
void initShip(int, char, ship *);				 //initializes a ship for use
void initGrids();								 //initilizes both grids for use in game. Must be called first.

void initGrids()
{
	//create a grid covered by a 0-length ship.
	empty.length = 0;
	empty.display = '-';
	compgrid = malloc(gridSizeX * gridSizeY * sizeof(ship *));
	playergrid = malloc(gridSizeX * gridSizeY * sizeof(ship *));
	for (int i = 0; i < gridSizeX * gridSizeY; i++)
	{
		*(compgrid + i) = &empty;
		*(playergrid + i) = &empty;
	}
}

void genShipLayout(ship *shps, int nships, ship **grid)
{
	for (int i = 0; i < nships; i++)
	{
		while (true)
		{
			if (placeShip(rand() % ((gridSizeX * gridSizeY)), rand() % 4, shps + i, grid))
			{
				continue;
			}
			break;
		}
	}
}

ship *initShips(int nships){
	return malloc(nships*sizeof(ship));
}

void initShip(int lng, char dsp, ship *shp)
{
	shp->length = lng;
	shp->display = dsp;
	for (int i = 0; i < lng; i++)
	{
		shp->status[i] = true;
	}
}

int placeShip(int head, int dir, ship *shp, ship **grid)
{
	//sets up the pointers on the grid to correctly point to ships, return -1 if a ship cannot be placed in that spot.
	int length = shp->length;
	if (dir == 0) //to right
	{
		if (head % gridSizeX + length > gridSizeX)
		{
			return -1;
		}
		for (int i = 0; i < length; i++)
		{
			ship *tmp = *(grid + head + i);
			if (tmp->length)
			{
				return -1;
			}
		}
		for (int i = 0; i < length; i++)
		{
			*(grid + head + i) = shp;
			shp->spacial[i] = head + i;
		}
	}
	else if (dir == 2)
	{ //to left
		if (head % gridSizeX - length + 1 < 0)
		{
			return -1;
		}
		for (int i = 0; i < length; i++)
		{
			ship *tmp = *(grid + head - length + 1 + i);
			if (tmp->length)
			{
				return -1;
			}
		}
		for (int i = 0; i < length; i++)
		{
			*(grid + head - length + 1 + i) = shp;
			shp->spacial[i] = head - length + 1 + i;
		}
	}
	else if (dir == 1)
	{ //downward
		if (head / gridSizeX + length > gridSizeY)
		{
			return -1;
		}
		for (int i = 0; i < length; i++)
		{
			ship *tmp = *(grid + head + i * gridSizeX);
			if (tmp->length)
			{
				return -1;
			}
		}
		for (int i = 0; i < length; i++)
		{
			*(grid + head + i * gridSizeX) = shp;
			shp->spacial[i] = head + i * gridSizeX;
		}
	}
	else if (dir == 3)
	{ //upward
		if (head / gridSizeX - length + 1 < 0)
		{
			return -1;
		}
		for (int i = 0; i < length; i++)
		{
			ship *tmp = *(grid + head + (1 - length + i) * gridSizeX);
			if (tmp->length)
			{
				return -1;
			}
		}
		for (int i = 0; i < length; i++)
		{
			*(grid + head + (1 - length + i) * gridSizeX) = shp;
			shp->spacial[i] = head + (1 - length + i) * gridSizeX;
		}
	}
	return 0;
}

ship *strikeShip(int cord, ship **grid)
{
	ship *shp = *(grid + cord);
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

void shipDebug(ship *shp)
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

void printMap(ship **grid)
{
	for (int i = 0; i < gridSizeX * gridSizeY; i++)
	{
		ship *tmp = *(grid + i);
		printf("%c ", tmp->display);
		if (i % gridSizeX == gridSizeX - 1)
		{
			printf("\n");
		}
	}
}