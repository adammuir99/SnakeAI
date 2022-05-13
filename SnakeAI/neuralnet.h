#ifndef NET_H
#define NET_H
#include <vector>
using namespace std;

class net {
public:

	net(); // Constructor
	void feed_forward(const vector<double>& inputVals);
	void back_prop(const vector<double>& targetVals);
	void get_results(vector<double>& results) const;
};


#endif // NET_H