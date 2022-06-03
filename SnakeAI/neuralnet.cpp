#include "neuralnet.h"
#include <cstdlib>
#include <cassert>
#include <cmath>
using namespace std;

// ------------------------------ NEURON -------------------------------------

neuron::neuron(unsigned numOutputs, unsigned myIndex) {
	for (unsigned c = 0; c < numOutputs; c++) {
		m_outputWeights.push_back(randomWeight());
	}

	m_myIndex = myIndex;
}

void neuron::setOutputVal(double val) {
	m_outputVal = val;
}

double neuron::getOutputVal() const {
	return m_outputVal;
}

// Returns a vector of all the weights from the previous layer to this neuron
vector<double> neuron::getInputWeights(layer& prevLayer) {
	vector<double> inputWeights;
	for (unsigned n = 0; n < prevLayer.size(); n++) {
		neuron& neuron = prevLayer[n];
		inputWeights.push_back(neuron.m_outputWeights[m_myIndex]);
	}
	return inputWeights;
}

void neuron::setInputWeights(layer& prevLayer, vector<double> inputWeights) {
	for (unsigned n = 0; n < prevLayer.size(); n++) {
		neuron& neuron = prevLayer[n];
		neuron.m_outputWeights[m_myIndex] = inputWeights[n];
	}
	return;
}

void neuron::feedForward(const layer& prevLayer) {
	double sum = 0.0;

	// Sum the previous layer's outputs (which are our inputs)
	// Include the bias node from the previous layer

	for (unsigned n = 0; n < prevLayer.size(); n++) {
		sum += prevLayer[n].getOutputVal() * prevLayer[n].m_outputWeights[m_myIndex];
	}

	m_outputVal = neuron::transferFunction(sum);
}

double neuron::transferFunction(double x) {
	// tanh - output range (-1.0 .. 1.0)
	return tanh(x);
}

double neuron::randomWeight() {
	return ((rand() / double(RAND_MAX)) * 2 - 1);
}

void neuron::mutate_weights(layer& prevLayer, double mutation_rate) {
	for (unsigned n = 0; n < prevLayer.size(); n++) {
		if (rand() / double(RAND_MAX) < mutation_rate) {
			neuron& neuron = prevLayer[n];
			neuron.m_outputWeights[m_myIndex] = randomWeight();
		}
	}
}

// ------------------------------ NET -------------------------------------

net::net(const vector<unsigned> &topology) {
	unsigned numLayers = topology.size();
	for (unsigned layerNum = 0; layerNum < numLayers; layerNum++) {
		m_layers.push_back(layer());
		unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

		// We have made a new layer now fill it with neurons,
		// and add a bias neuron to the layer
		for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; neuronNum++) {
			m_layers.back().push_back(neuron(numOutputs, neuronNum));
		}

		// Force the bias node's output value to 1.0. It's the last neuron created above
		m_layers.back().back().setOutputVal(1.0);
	}
}

void net::feed_forward(const vector<double> &inputVals) {
	assert(inputVals.size() == m_layers[0].size() - 1);

	// Assign the input values into the input neurons
	for (unsigned i = 0; i < inputVals.size(); i++) {
		m_layers[0][i].setOutputVal(inputVals[i]);
	}

	// Forward propagate
	for (unsigned layerNum = 1; layerNum < m_layers.size(); layerNum++) {
		layer &prevLayer = m_layers[layerNum - 1];
		for (unsigned n = 0; n < m_layers[layerNum].size() - 1; n++) {
			m_layers[layerNum][n].feedForward(prevLayer);
		}
	}
}

void net::get_results(vector<double> &resultVals) const {
	resultVals.clear();

	for (unsigned n = 0; n < m_layers.back().size() - 1; n++) {
		resultVals.push_back(m_layers.back()[n].getOutputVal());
	}
}

vector<vector<double>> net::get_layer_weights(unsigned layerNum) const {
	assert(layerNum < m_layers.size());
	assert(layerNum > 0);	// If layerNum is 0, prevLayer will be -1 and out of bounds
	layer currLayer = m_layers[layerNum];
	layer prevLayer = m_layers[layerNum - 1];
	vector<vector<double>> layerWeights;
	for (unsigned i = 0; i < currLayer.size() - 1; i++) {
		vector<double> neuron_weights = currLayer[i].getInputWeights(prevLayer);
		layerWeights.push_back(neuron_weights);
	}
	return layerWeights;
}

void net::set_layer_weights(unsigned layerNum, vector<vector<double>> layerWeights) {
	assert(layerNum < m_layers.size());
	assert(layerNum > 0);	// If layerNum is 0, prevLayer will be -1 and out of bounds
	assert(layerWeights.size() == m_layers[layerNum].size() - 1);

	layer currLayer = m_layers[layerNum];
	layer prevLayer = m_layers[layerNum - 1];
	for (unsigned i = 0; i < currLayer.size() - 1; i++) {
		currLayer[i].setInputWeights(prevLayer, layerWeights[i]);
	}
	m_layers[layerNum] = currLayer;
	m_layers[layerNum - 1] = prevLayer;
	return;
}

void net::mutate(double mutation_rate) {
	for (unsigned layerNum = 1; layerNum < m_layers.size(); layerNum++) {
		layer currLayer = m_layers[layerNum];
		layer prevLayer = m_layers[layerNum - 1];
		for (unsigned i = 0; i < currLayer.size() - 1; i++) {
			currLayer[i].mutate_weights(prevLayer, mutation_rate);
		}
		m_layers[layerNum] = currLayer;
		m_layers[layerNum - 1] = prevLayer;
	}
}