#include "neuralnet.h"
using namespace std;

net::net() {

}

void net::feed_forward(const vector<double> &inputVals) {

}

void net::back_prop(const vector<double> &targetVals) {

}

void net::get_results(vector<double> &results) const {

}

void neural_net() {
	net myNet;

	vector<double> inputVals;
	myNet.feed_forward(inputVals);

	vector<double> targetVals;
	myNet.back_prop(targetVals);

	vector<double> resultVals;
	myNet.get_results(resultVals);
}