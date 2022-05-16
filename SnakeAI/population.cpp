#include "population.h"
#include <fstream>
#include <iterator>


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
	for (unsigned i = 0; i < size; i++) {
		snakePop.push_back(child);
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

	// layer#<neuron#<input connection#<weight>>>
	vector<vector<vector<double>>> mom_weights, dad_weights;

	// Start at 1 because layer 0 (input layer) does not have any input weights
	for (unsigned layer = 1; layer < topology.size(); layer++) {
		mom_weights.push_back(mom_brain.get_layer_weights(layer));
		dad_weights.push_back(dad_brain.get_layer_weights(layer));
	}

	/*ofstream output_file("momlayer0.txt");
	ostream_iterator<double> output_iterator(output_file, "\t");
	for (int i = 0; i < mom_weights[0].size(); i++) {
		copy(mom_weights[0].at(i).begin(), mom_weights[0].at(i).end(), output_iterator);
		output_file << endl;
	}*/

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

	snake child(topology);
	child.neuralnet = child_brain;

	/*ofstream output_file2("childlayer0.txt");
	ostream_iterator<double> output_iterator2(output_file2, "\t");
	for (int i = 0; i < mom_weights[0].size(); i++) {
		copy(child_weights[0].at(i).begin(), child_weights[0].at(i).end(), output_iterator2);
		output_file2 << endl;
	}*/
	
	return child;
}

void population::mutate() {
	for (unsigned i = 1; i < snakePop.size(); i++) {
		snakePop[i].neuralnet.mutate(mutation_rate);
	}
}

pair<snake, snake> population::get_fittest_snakes() {
	int fitnessScore = 0;
	unsigned fittestSnakeIndex = 0;
	unsigned secondFittestSnakeIndex = 0;
	for (unsigned i = 0; i < size; i++) {
		if (snakePop[i].myStats.score > fitnessScore) {
			secondFittestSnakeIndex = fittestSnakeIndex;
			fittestSnakeIndex = i;
			fitnessScore = snakePop[i].myStats.score;
		}
	}
	return make_pair(snakePop[fittestSnakeIndex], snakePop[secondFittestSnakeIndex]);
}