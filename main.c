//Created by Andrew Emerson
#include <stdio.h>
#include <stdbool.h>

#define maxShipLength 5

struct ship{
	int length;						//The length in map tiles of a given ship, 0 = open ocean
	int spacial[maxShipLength];		//The index of each tile a given ship occupies
	bool status[maxShipLength];		//tells if the ship has been hit at the corosponding spacial index
};

void printMap();
void placeShip(int, struct ship*);

//Global
int gridSizeX = 8, gridSizeY = 8;	//grid size
struct ship** grid;					//map the game is played on.
int main(int argc, char** argv) {
	//create a grid covered by a 0-length ship.
	struct ship empty;
	empty.length = 0;
	grid = malloc(gridSizeX* gridSizeY * sizeof(struct ship*));
	for (int i = 0; i < gridSizeX * gridSizeY; i++) {
		*(grid+i) = &empty;
	}

	struct ship arr;
	arr.length = 2;
	placeShip(8*3 + 5, &arr);


	printMap();

	free(empty);
	return 0;
}

void placeShip(int head, struct ship* shp) {
	//WARNING, no out of bounds checking + only one places east of head
	int dir = 3;
	int length = shp->length;
	if (dir == 0) //to right
	{
		for (int i = head; i < head + length; i++) {
			*(grid + i) = shp;
		}
	}
	else if (dir == 2) { //to left
		for (int i = head - length + 1; i <= head; i++) {
			*(grid + i) = shp;
		}
	}
	else if (dir == 1) { //downward
		for (int i = 0; i < length; i++) { 
			*(grid + head + i*gridSizeX) = shp;
		}
	}
	else if (dir == 3) { //upward
		for (int i = 1-length; i <= 0; i++) {
			*(grid + head + i * gridSizeX) = shp;
		}
	}
	
}

void printMap() {
	for (int i = 0; i < gridSizeX * gridSizeY; i++) {
		struct ship* tmp = *(grid + i);
		printf("%i ", tmp->length);
		if (i % gridSizeX == gridSizeX - 1) { printf("\n"); }
	}
}

