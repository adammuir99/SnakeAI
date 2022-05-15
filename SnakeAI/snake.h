#ifndef SNAKE_H
#define SNAKE_H

#include <list>
#include "neuralnet.h"
#include "stats.h"
#include "const.h"
using namespace std;

class snake{
public:
	std::list<std::pair<int, int>> snakeList;
	int snakeLength;
	directions oldDirection;
	net neuralnet;
	vector<double> vision;	// Groupings of 3 doubles: Direction{sees food, sees self, distance}
							// eg. North{1.0, 0.0, 1/5} means the first thing seen in the north
							// direction is food and it is 5 squares away (normalized 1/5)
	stats myStats;

	// Member Functions
	snake(const vector<unsigned>& topology);	// Constructor
	pair<int, int> getHead() const;
	void add_head(pair<int, int> new_joint);
	void remove_tail();
	void get_vision(gridContents a[GRID_HEIGHT][GRID_WIDTH]);
};

#endif // SNAKE_H