#include <iostream>
#include "grid.h"
using namespace std;

grid::grid() {
	foodIndex = 0;
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

	foodIndex++;
	// Loop food vector so it never ends
	if (foodIndex > 999) {
		foodIndex = 0;
	}
	// Prevent food from appearing on snake square
	while (this->a[this->testFood[foodIndex].first][this->testFood[foodIndex].second] == SNAKE) {
		foodIndex++;
		if (foodIndex > 999) {
			foodIndex = 0;
		}
	}
	this->foodCoords = this->testFood[foodIndex];
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