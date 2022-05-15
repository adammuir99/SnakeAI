#include "population.h"


population::population(unsigned size, double mutation_rate, const vector<unsigned>& topology) {
	this->size = size;
	this->mutation_rate = mutation_rate;

	for (unsigned i = 0; i < size; i++) {
		snakePop.push_back(snake(topology));
	}
}

population::population(unsigned size, double mutation_rate, const vector<unsigned>& topology, pair<snake, snake>& parents) {
	this->size = size;
	this->mutation_rate = mutation_rate;
}

void population::populate() {

}

snake population::get_fittest_snake() {
	int fitnessScore = 0;
	unsigned fittestSnakeIndex = 0;
	for (unsigned i = 0; i < size; i++) {
		if (snakePop[i].myStats.score > fitnessScore) {
			fittestSnakeIndex = i;
			fitnessScore = snakePop[i].myStats.score;
		}
	}
	return snakePop[fittestSnakeIndex];
}