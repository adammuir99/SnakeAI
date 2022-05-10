#ifndef SNAKE_H
#define SNAKE_H

#include <list>
using namespace std;

enum class directions {
	UP,
	DOWN,
	RIGHT,
	LEFT
};

class snake{
public:
	std::list<std::pair<int, int>> snakeList;
	int snakeLength;
	directions oldDirection;

	// Member Functions
	snake();	// Constructor
	pair<int, int> getHead() const;
	void add_head(pair<int, int> new_joint);
	void remove_tail();
};

#endif // SNAKE_H