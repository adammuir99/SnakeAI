#ifndef GRID_H
#define GRID_H
#include <utility>
#include "snake.h"
#include "const.h"

using namespace std;

class grid{
public:
	gridContents a[GRID_HEIGHT][GRID_WIDTH];
	pair<int, int> foodCoords;
	
	// Member Functions
	grid();	// Constructor
	void reset();
	gridContents getContents(std::pair<int, int> coords) const;
	void new_food();
	void place_snake(const snake& currSnake);
	void place_food();
};

#endif // GRID_H