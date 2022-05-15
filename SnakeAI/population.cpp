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

	snake child = crossover(parents.first, parents.second, topology);

}

void population::populate() {

}

// Combine the brains of two snakes and return the combined result
snake population::crossover(snake mom, snake dad, const vector<unsigned>& topology) {
	net mom_brain = mom.neuralnet;
	net dad_brain = dad.neuralnet;
	net child_brain(topology);

	// layer#<neuron#<input connection#<weight>>>
	vector<vector<vector<double>>> mom_weights, dad_weights;

	// Start at 1 because layer 0 (input layer) does not have any input weights
	for (unsigned layer = 1; layer < topology.size(); layer++) {
		mom_weights.push_back(mom_brain.get_layer_weights(layer));
		dad_weights.push_back(dad_brain.get_layer_weights(layer));
	}

	// Now we have 3D vectors of mom and dad's weights
	// Take half of mom's weights and half of dad's weights,
	// and assign to child
	vector<vector<vector<double>>> child_weights;

	for (unsigned layer = 1; layer < topology.size(); layer++) {
		for (unsigned neuron = 0; neuron < topology[layer]; neuron++) {
			for (unsigned connection = 0; connection < topology[layer - 1]; connection++) {
				if (connection < topology[layer - 1] / 2) {	// first half of weights are taken from mom
					child_weights[layer][neuron][connection] = mom_weights[layer][neuron][connection];
				}
				else {	// Second half of weights are from dad
					child_weights[layer][neuron][connection] = dad_weights[layer][neuron][connection];
				}
			}
		}
	}
	
	// The child's weights are set, now place them in the
	// child's brain
	for (unsigned layer = 1; layer < topology.size(); layer++) {
		child_brain.set_layer_weights(layer, child_weights[layer]);
	}
	snake child(topology);
	child.neuralnet = child_brain;
	
	return child;
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