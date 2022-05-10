#include <iostream>
#include "snake.h"
using namespace std;

snake::snake() {
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