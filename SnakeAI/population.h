#ifndef POPULATION_H
#define POPULATION_H
#include "neuralnet.h"
#include "snake.h"

class population {
public:
	unsigned size;
	double mutation_rate;
	vector<snake> snakePop;

	population(unsigned size, double mutation_rate, const vector<unsigned>& topology);		// Constructor for initial set of random population
	population(unsigned size, double mutation_rate, const vector<unsigned>& topology, pair<snake, snake>& parents);		// Constructor for population evolved from parents
	void populate();
	snake crossover(snake mom, snake dad, const vector<unsigned>& topology);
	snake get_fittest_snake();
private:
};

#endif // POPULATION_H