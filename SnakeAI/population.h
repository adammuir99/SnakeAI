#ifndef POPULATION_H
#define POPULATION_H
#include "neuralnet.h"
#include "snake.h"

class population {
public:
	unsigned size;
	double mutation_rate;
	vector<snake> snakePop;

	population(unsigned size, double mutation_rate, const vector<unsigned>& topology);
	void populate();
private:
};

#endif // POPULATION_H