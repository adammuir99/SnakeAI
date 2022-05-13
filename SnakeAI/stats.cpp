#include "stats.h"

stats::stats() {
	gameOver = false;
	foodCount = 0;
	moveCount = 200;	// Initially have 200 moves to get to the first food
	score = 0;
	timePenalty = 0;
}