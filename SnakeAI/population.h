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
	population(unsigned size, double mutation_rate, const vector<unsigned>& topology, snake& child);		// Constructor for population evolved from parents
	void populate();
	snake crossover(snake mom, snake dad, const vector<unsigned>& topology);
	vector<snake> get_fittest_snakes();
	vector<snake> multi_crossover(vector<snake> snakeVec, const vector<unsigned>& topology);
	void mutate();
private:
};

#endif // POPULATION_H