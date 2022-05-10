#include <iostream>
#include "grid.h"
using namespace std;

grid::grid() {
	this->reset();
}

void grid::reset() {
	for (int i = 0; i < GRID_WIDTH; i++) {
		for (int j = 0; j < GRID_HEIGHT; j++) {
			a[i][j] = EMPTY;
		}
	}
	return;
}

gridContents grid::getContents(std::pair<int, int> coords) const {
	return a[get<0>(coords)][get<1>(coords)];
}

void grid::new_food(){
	// Remove old food
	pair <int, int> oldFood;
	for (int i = 0; i < GRID_HEIGHT; i++){
		for (int j = 0; j < GRID_WIDTH; j++){
			if (a[i][j] == FOOD){
				a[i][j] = EMPTY;
				oldFood = make_pair(i, j);
			}
		}
	}

	int randomX = rand() % GRID_WIDTH;
	int randomY = rand() % GRID_HEIGHT;
	// If the random location is already occupied by the snake or is in the same spot as the old food, find a new location
	while (this->a[randomY][randomX] != EMPTY || make_pair(randomY, randomX) == oldFood){
		randomX = rand() % GRID_WIDTH;
		randomY = rand() % GRID_HEIGHT;
	}

	this->a[randomY][randomX] = FOOD;
	this->foodCoords = make_pair(randomY, randomX);
	return;
}

void grid::place_snake(const snake& currSnake) {
	for (auto iter = currSnake.snakeList.begin(); iter != currSnake.snakeList.end(); ++iter) {
		int y = iter->first;
		int x = iter->second;
		this->a[y][x] = SNAKE;
	}
	return;
}

void grid::place_food() {
	this->a[foodCoords.first][foodCoords.second] = FOOD;
}