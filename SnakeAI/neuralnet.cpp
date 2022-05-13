#include "neuralnet.h"
#include <cstdlib>
#include <cassert>
#include <cmath>
using namespace std;

// ------------------------------ NEURON -------------------------------------

double neuron::eta = 0.15;	// overall net learning rate, [0.0 .. 1.0]
double neuron::alpha = 0.5; // momentum, multiplier of last deltaWeight, [0.0 .. n]

neuron::neuron(unsigned numOutputs, unsigned myIndex) {
	for (unsigned c = 0; c < numOutputs; c++) {
		m_outputWeights.push_back(connection());
		m_outputWeights.back().weight = randomWeight();
	}

	m_myIndex = myIndex;
}

void neuron::setOutputVal(double val) {
	m_outputVal = val;
}

double neuron::getOutputVal() const {
	return m_outputVal;
}

void neuron::updateInputWeights(layer& prevLayer) {
	// The weights to be updated are in the connection container
	// in the neurons in the preceding layer
	for (unsigned n = 0; n < prevLayer.size(); n++) {
		neuron& neuron = prevLayer[n];
		double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaWeight;

		double newDeltaWeight =
			// Individual input, magnified by the gradient and train rate:
			eta
			* neuron.getOutputVal()
			* m_gradient
			// Also add momentum = a fraction of the previous delta weight
			* alpha
			* oldDeltaWeight;

		neuron.m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
		neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;
	}
}

double neuron::sumDOW(const layer& nextLayer) const {
	double sum = 0.0;

	// Sum our contributions of the errors at the nodes we feed
	for (unsigned n = 0; n < nextLayer.size() - 1; n++) {
		sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;
	}

	return sum;
}

void neuron::calcOutputGradients(double targetVal) {
	double delta = targetVal - m_outputVal;
	m_gradient = delta * neuron::transferFunctionDerivative(m_outputVal);
}

void neuron::calcHiddenGradients(const layer& nextLayer) {
	double dow = sumDOW(nextLayer);
	m_gradient = dow * neuron::transferFunctionDerivative(m_outputVal);
}

void neuron::feedForward(const layer& prevLayer) {
	double sum = 0.0;

	// Sum the previous layer's outputs (which are our inputs)
	// Include the bias node from the previous layer

	for (unsigned n = 0; n < prevLayer.size(); n++) {
		sum += prevLayer[n].getOutputVal() * prevLayer[n].m_outputWeights[m_myIndex].weight;
	}

	m_outputVal = neuron::transferFunction(sum);
}

double neuron::transferFunction(double x) {
	// tanh - output range (-1.0 .. 1.0)
	return tanh(x);
}

double neuron::transferFunctionDerivative(double x) {
	// tanh derivative
	return 1.0 - x * x;
}

double neuron::randomWeight() {
	return rand() / double(RAND_MAX);
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

void net::back_prop(const vector<double> &targetVals) {
	// Calculate overall net error (RMS of output neuron errors)

	layer& outputLayer = m_layers.back();
	m_error = 0.0;

	for (unsigned n = 0; n < outputLayer.size() - 1; n++) {
		double delta = targetVals[n] - outputLayer[n].getOutputVal();
		m_error += delta * delta;
	}
	m_error /= outputLayer.size() - 1; // get average error squared
	m_error = sqrt(m_error); // RMS

	// Implement a recent average measurement

	m_recentAverageError = (m_recentAverageError * m_recentAverageSmoothingFactor + m_error) / (m_recentAverageSmoothingFactor + 1.0);

	// Calculate output layer gradients

	for (unsigned n = 0; n < outputLayer.size() - 1; n++) {
		outputLayer[n].calcOutputGradients(targetVals[n]);
	}

	// Calculate gradients on hidden layers

	for (unsigned layerNum = m_layers.size() - 2; layerNum > 0; layerNum--) {
		layer& hiddenLayer = m_layers[layerNum];
		layer& nextLayer = m_layers[layerNum + 1];

		for (unsigned n = 0; n < hiddenLayer.size(); n++) {
			hiddenLayer[n].calcHiddenGradients(nextLayer);
		}
	}

	// For all layers from outputs to first hidden layer,
	// update connection weights

	for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; layerNum--) {
		layer& currLayer = m_layers[layerNum];
		layer& prevLayer = m_layers[layerNum - 1];

		for (unsigned n = 0; n < currLayer.size() - 1; n++) {
			currLayer[n].updateInputWeights(prevLayer);
		}
	}

}

void net::get_results(vector<double> &resultVals) const {
	resultVals.clear();

	for (unsigned n = 0; n < m_layers.back().size() - 1; n++) {
		resultVals.push_back(m_layers.back()[n].getOutputVal());
	}
}

void neural_net() {
	// e.g. {3, 2, 1}
	vector<unsigned> topology;
	topology.push_back(1);
	topology.push_back(2);
	topology.push_back(3);
	net myNet(topology);

	vector<double> inputVals;
	myNet.feed_forward(inputVals);

	vector<double> targetVals;
	myNet.back_prop(targetVals);

	vector<double> resultVals;
	myNet.get_results(resultVals);
}