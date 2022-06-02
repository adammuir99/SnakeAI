#include <iostream>
#include "snake.h"
using namespace std;

snake::snake(const vector<unsigned>& topology) : neuralnet(topology) {
	snakeLength = 1;
	snakeList.push_front(make_pair(20,20));	// Start the snake in the middle of the screen
}

pair<int, int> snake::getHead() const{
	return snakeList.front();
}

void snake::add_head(pair<int, int> new_joint) {
	this->snakeList.push_front(new_joint);
	this->snakeLength++;
	return;
}

void snake::remove_tail() {
	this->snakeList.pop_back();
	this->snakeLength--;
	return;
}

void snake::get_vision(gridContents a[GRID_HEIGHT][GRID_WIDTH]) {
	pair<int, int> head = this->getHead();
	int x = head.second;
	int y = head.first;
	double distance = 1.0;
	vector<double> temp_vision(12);
	// N
	while (y > 0) {
		y--;
		if (a[y][x] == FOOD) {
			temp_vision[0] = 1.0;
			break;
		}
		else if (a[y][x] == SNAKE) {
			temp_vision[1] = 1.0;
			break;
		}
		distance = distance + 1.0;
	}
	temp_vision[2] = 1 / distance;
	distance = 1.0;
	x = head.second;
	y = head.first;
	// NE
	/*while (y > 0 && x < GRID_WIDTH - 1) {
		y--;
		x++;
		if (a[y][x] == FOOD) {
			temp_vision[3] = 1.0;
			break;
		}
		else if (a[y][x] == SNAKE) {
			temp_vision[4] = 1.0;
			break;
		}
		distance = distance + 1.0;
	}
	temp_vision[5] = 1 / distance;
	distance = 1.0;
	x = head.second;
	y = head.first;*/
	// E
	while (x < GRID_WIDTH - 1) {
		x++;
		if (a[y][x] == FOOD) {
			temp_vision[3] = 1.0;
			break;
		}
		else if (a[y][x] == SNAKE) {
			temp_vision[4] = 1.0;
			break;
		}
		distance = distance + 1.0;
	}
	temp_vision[5] = 1 / distance;
	distance = 1.0;
	x = head.second;
	y = head.first;
	// SE
	/*while (y < GRID_HEIGHT - 1 && x < GRID_WIDTH - 1) {
		y++;
		x++;
		if (a[y][x] == FOOD) {
			temp_vision[9] = 1.0;
			break;
		}
		else if (a[y][x] == SNAKE) {
			temp_vision[10] = 1.0;
			break;
		}
		distance = distance + 1.0;
	}
	temp_vision[11] = 1 / distance;
	distance = 1.0;
	x = head.second;
	y = head.first;*/
	// S
	while (y < GRID_HEIGHT - 1) {
		y++;
		if (a[y][x] == FOOD) {
			temp_vision[6] = 1.0;
			break;
		}
		else if (a[y][x] == SNAKE) {
			temp_vision[7] = 1.0;
			break;
		}
		distance = distance + 1.0;
	}
	temp_vision[8] = 1 / distance;
	distance = 1.0;
	x = head.second;
	y = head.first;
	// SW
	/*while (y < GRID_HEIGHT - 1 && x > 0) {
		y++;
		x--;
		if (a[y][x] == FOOD) {
			temp_vision[15] = 1.0;
			break;
		}
		else if (a[y][x] == SNAKE) {
			temp_vision[16] = 1.0;
			break;
		}
		distance = distance + 1.0;
	}
	temp_vision[17] = 1 / distance;
	distance = 1.0;
	x = head.second;
	y = head.first;*/
	// W
	while (x > 0) {
		x--;
		if (a[y][x] == FOOD) {
			temp_vision[9] = 1.0;
			break;
		}
		else if (a[y][x] == SNAKE) {
			temp_vision[10] = 1.0;
			break;
		}
		distance = distance + 1.0;
	}
	temp_vision[11] = 1 / distance;
	distance = 1.0;
	x = head.second;
	y = head.first;
	// NW
	/*while (y > 0 && x > 0) {
		y--;
		x--;
		if (a[y][x] == FOOD) {
			temp_vision[21] = 1.0;
			break;
		}
		else if (a[y][x] == SNAKE) {
			temp_vision[22] = 1.0;
			break;
		}
		distance = distance + 1.0;
	}
	temp_vision[23] = 1 / distance;*/

	this->vision = temp_vision;
	return;
}