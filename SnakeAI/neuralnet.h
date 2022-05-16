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
	void calcOutputGradients(double targetVal);
	void calcHiddenGradients(const layer &nextLayer);
	void updateInputWeights(layer& prevLayer);
	vector<double> getInputWeights(layer& prevLayer);
	void setInputWeights(layer& prevLayer, vector<double> inputWeights);
	void mutate_weights(layer& prevLayer, double mutation_rate);

private:
	double m_outputVal;
	vector<double> m_outputWeights;
	unsigned m_myIndex;
	double m_gradient;
	static double eta;	// [0.0 .. 1.0] overall net training rate
	static double alpha;// [0.0 .. n] multiplier of last weight change (momentum)
	static double randomWeight();
	static double transferFunction(double x);
	static double transferFunctionDerivative(double x);
	double sumDOW(const layer & nextLayer) const;
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
	double m_error;
	double m_recentAverageError;
	double m_recentAverageSmoothingFactor;
};


#endif // NET_H