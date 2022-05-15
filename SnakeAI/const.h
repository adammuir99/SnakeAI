#pragma once
constexpr int GRID_WIDTH = 40;
constexpr int GRID_HEIGHT = 40;

enum gridContents {
	EMPTY,
	SNAKE,
	FOOD
};

enum class directions {
	UP,
	DOWN,
	RIGHT,
	LEFT
};