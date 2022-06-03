#ifndef NET_H
#define NET_H
#include <vector>
using namespace std;

class neuron;

typedef vector<neuron> layer;

class neuron {
public:
	neuron(unsigned numOutputs, unsigned myIndex);
	void setOutputVal(double val);
	double getOutputVal(void) const;
	void feedForward(const layer& prevLayer);
	vector<double> getInputWeights(layer& prevLayer);
	void setInputWeights(layer& prevLayer, vector<double> inputWeights);
	void mutate_weights(layer& prevLayer, double mutation_rate);

private:
	double m_outputVal;
	vector<double> m_outputWeights;
	unsigned m_myIndex;
	static double randomWeight();
	static double transferFunction(double x);
};

class net {
public:

	net(const vector<unsigned> &topology); // Constructor
	void feed_forward(const vector<double>& inputVals);
	void back_prop(const vector<double>& targetVals);
	void get_results(vector<double>& resultVals) const;
	vector<vector<double>> get_layer_weights(unsigned layerNum) const;
	void set_layer_weights(unsigned layerNum, vector<vector<double>> layerWeights);
	void mutate(double mutation_rate);

private:
	vector<layer> m_layers; // layers[layerNum][neuronNum]
};


#endif // NET_H