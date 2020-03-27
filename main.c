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
int placeShip(int, int, struct ship *);
struct ship *strikeShip(int);
void initShip(int, char, struct ship*);

//Global
int gridSizeX = 8, gridSizeY = 8; //grid size
struct ship **grid;				  //map the game is played on.
struct ship *playerShips;
struct main *comShip;

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

	int numShips = 3;
	playerShips = malloc(numShips*sizeof(struct ship));
	initShip(3, 'X', playerShips + 0);
	placeShip(40,1,playerShips + 0);

	initShip(3,'Y',playerShips+1);
	placeShip(57,0,playerShips + 1);

	initShip(5,'Z', playerShips+2);
	placeShip(32,3,playerShips + 2);

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
	free(playerShips);
	return 0;
}

void genShipLayout(){
	
}

void initShip(int lng, char dsp, struct ship* shp){
	shp->length = lng;
	shp->display = dsp;
	for (int i = 0; i < lng; i++){
		shp->status[i] = true;
	}
}

int placeShip(int head, int dir, struct ship *shp)
{
	//sets up the pointers on the grid to correctly point to ships, return -1 if a ship cannot be placed in that spot.
	int length = shp->length;
	if (dir == 0) //to right
	{
		if(head%gridSizeX+length>gridSizeX){
			return -1;
		}
		for (int i = 0; i < length; i++){
			struct ship* tmp = *(grid+head+i);
			if (tmp->length){
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
		if(head%gridSizeX-length+1<0){
			return -1;
		}
		for (int i = 0; i < length; i++){
			struct ship* tmp = *(grid+head-length+1+i);
			if (tmp->length){
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
		if(head/gridSizeX+length>gridSizeY){
			return -1;
		}
		for (int i = 0; i < length; i++){
			struct ship* tmp = *(grid+head+i*gridSizeX);
			if (tmp->length){
				return -1;
			}
		}
		for (int i = 0; i < length; i++)
		{
			*(grid + head + i * gridSizeX) = shp;
			shp->spacial[i] = head + i *gridSizeX;
		}
	}
	else if (dir == 3)
	{ //upward
		if(head/gridSizeX-length+1<0){
			return -1;
		}
		for (int i = 0; i < length; i++){
			struct ship* tmp = *(grid+head+(1-length+i)*gridSizeX);
			if (tmp->length){
				return -1;
			}
		}
		for (int i = 0; i < length; i++)
		{
			*(grid + head + (1-length+i)*gridSizeX) = shp;
			shp->spacial[i] = head + (1-length+i)*gridSizeX;
		}
	}
	return 0;
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
