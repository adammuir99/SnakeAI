#include "population.h"


population::population(unsigned size, double mutation_rate, const vector<unsigned>& topology) {
	this->size = size;
	this->mutation_rate = mutation_rate;

	for (unsigned i = 0; i < size; i++) {
		snakePop.push_back(snake(topology));
	}
}

void population::populate() {

}