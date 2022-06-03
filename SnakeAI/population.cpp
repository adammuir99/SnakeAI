#include "population.h"
#include <fstream>
#include <iterator>
#include <algorithm>


population::population(unsigned size, double mutation_rate, const vector<unsigned>& topology) {
	this->size = size;
	this->mutation_rate = mutation_rate;

	for (unsigned i = 0; i < size; i++) {
		snakePop.push_back(snake(topology));
	}
}

population::population(unsigned size, double mutation_rate, const vector<unsigned>& topology, snake& child) {
	this->size = size;
	this->mutation_rate = mutation_rate;

	snake clone(topology);
	clone.neuralnet = child.neuralnet;

	for (unsigned i = 0; i < size; i++) {
		snakePop.push_back(clone);
	}
	mutate();
}

void population::populate() {

}

// Combine the brains of two snakes and return the combined result
snake population::crossover(snake mom, snake dad, const vector<unsigned>& topology) {
	net mom_brain = mom.neuralnet;
	net dad_brain = dad.neuralnet;

	net child_brain(topology);

	snake child(topology);

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
	vector<vector<vector<double>>> child_weights = dad_weights;

	for (unsigned layer = 0; layer < mom_weights.size(); layer++) {
		for (unsigned neuron = 0; neuron < mom_weights[layer].size(); neuron++) {
			for (unsigned connection = 0; connection < mom_weights[layer][neuron].size(); connection++) {
				if (connection < mom_weights[layer][neuron].size() / 2) {	// first half of weights are taken from mom
					child_weights[layer][neuron][connection] = mom_weights[layer][neuron][connection];
				}
			}
		}
	}
	
	// The child's weights are set, now place them in the
	// child's brain
	for (unsigned layer = 1; layer < topology.size(); layer++) {
		child_brain.set_layer_weights(layer, child_weights[layer - 1]);
	}

	child.neuralnet = child_brain;
	
	return child;
}

void population::mutate() {
	for (unsigned i = 1; i < snakePop.size(); i++) {	// Leave first snake (best from last gen), and second snake (crossover snake) unmutated
		snakePop[i].neuralnet.mutate(mutation_rate);
	}
}

vector<snake> population::get_fittest_snakes() {
	
	// first = score, second = snakePop index
	vector<pair<unsigned, unsigned>> sorted_list;

	for (unsigned k = 0; k < this->size; k++) {
		sorted_list.push_back(make_pair(this->snakePop[k].myStats.score, k));
	}
	sort(sorted_list.begin(), sorted_list.end());

	vector<snake> fittest_snakes;
	for (unsigned i = 0; i < 10; i++) {
		fittest_snakes.push_back(snakePop[sorted_list[this->size - i - 1].second]);
	}
	return fittest_snakes;
}

vector<snake> population::multi_crossover(vector<snake> snakeVec, const vector<unsigned>& topology) {
	// Snake with the highest score is in index 0.
	// Crossover the best snake with the n-1 other
	// snakes.
	vector<snake> children;
	for (unsigned i = 1; i < snakeVec.size(); i++) {
		children.push_back(crossover(snakeVec[0], snakeVec[i], topology));
	}
	return children;
}