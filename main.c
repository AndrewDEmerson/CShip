//Created by Andrew Emerson
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define maxShipLength 5

struct ship
{
	int length;					//The length in map tiles of a given ship, 0 = open ocean
	int spacial[maxShipLength]; //The index of each tile a given ship occupies
	bool status[maxShipLength]; //tells if the ship has been hit at the corosponding spacial index
	char display;
};

void printMap(struct ship **grid);
void shipDebug(struct ship *);
void genShipLayout(struct ship *, struct ship **);
int placeShip(int, int, struct ship* shp, struct ship** grid);
struct ship *strikeShip(int, struct ship** grid);
void initShip(int, char, struct ship*);

//Global
int gridSizeX = 10, gridSizeY = 10; //grid size
struct ship **compgrid;				  //map the game is played on.
struct ship **playergrid;
struct ship *playerShips;
struct ship *compShips;

int main(int argc, char **argv)
{
	time_t t;
	srand((unsigned)time(&t));
	//create a grid covered by a 0-length ship.
	struct ship empty;
	empty.length = 0;
	empty.display = '-';
	compgrid = malloc(gridSizeX * gridSizeY * sizeof(struct ship *));
	playergrid = malloc(gridSizeX * gridSizeY * sizeof(struct ship *));
	for (int i = 0; i < gridSizeX * gridSizeY; i++)
	{
		*(compgrid + i) = &empty;
		*(playergrid + i) = &empty;
	}

	int numShips = 5;
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

void genShipLayout(struct ship *shps, struct ship** grid){
	int numShips = 5;
	for(int i = 0; i < numShips; i++){
		while (true)
		{
			if(placeShip(rand()%((gridSizeX*gridSizeY)),rand()%4,shps+i,grid)	){
				continue;
			}
			break;
		}
		
	}
	
}

void initShip(int lng, char dsp, struct ship* shp){
	shp->length = lng;
	shp->display = dsp;
	for (int i = 0; i < lng; i++){
		shp->status[i] = true;
	}
}

int placeShip(int head, int dir, struct ship *shp, struct ship **grid)
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

struct ship *strikeShip(int cord, struct ship** grid)
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

void printMap(struct ship** grid)
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
