#include "SDL.h"
#include "SDL_ttf.h"
#include "grid.h"
#include "snake.h"
#include "stats.h"
#include <string>
#include "neuralnet.h"
#include "population.h"
#include "const.h"
#include <iostream>
using namespace std;

bool check_valid(grid& theGrid, snake& theSnake, directions direction) {
	pair<int, int> head = theSnake.getHead();
	pair<int, int> next_move;
	
	// Check if the snake moves into the border or into itself
	switch (direction) {
		case directions::UP:
			if (get<0>(head) == 0) {
				return false;
			}
			next_move = make_pair(get<0>(head) - 1, get<1>(head));
			if (theGrid.getContents(next_move) == SNAKE) {
				return false;
			}
			break;
		case directions::DOWN:
			if (get<0>(head) == GRID_HEIGHT - 1) {
				return false;
			}
			next_move = make_pair(get<0>(head) + 1, get<1>(head));
			if (theGrid.getContents(next_move) == SNAKE) {
				return false;
			}
			break;
		case directions::LEFT:
			if (get<1>(head) == 0) {
				return false;
			}
			next_move = make_pair(get<0>(head), get<1>(head) - 1);
			if (theGrid.getContents(next_move) == SNAKE) {
				return false;
			}
			break;
		case directions::RIGHT:
			if (get<1>(head) == GRID_WIDTH - 1) {
				return false;
			}
			next_move = make_pair(get<0>(head), get<1>(head) + 1);
			if (theGrid.getContents(next_move) == SNAKE) {
				return false;
			}
			break;
	}

	return true;
}

void make_move(grid& theGrid, snake& theSnake, directions direction) {
	pair<int, int> head = theSnake.getHead();
	pair<int, int> next_move;
	switch (direction) {
		case(directions::UP):
			next_move = make_pair(get<0>(head) - 1, get<1>(head));
			break;
		case(directions::DOWN):
			next_move = make_pair(get<0>(head) + 1, get<1>(head));
			break;
		case(directions::LEFT):
			next_move = make_pair(get<0>(head), get<1>(head) - 1);
			break;
		case(directions::RIGHT):
			next_move = make_pair(get<0>(head), get<1>(head) + 1);
			break;
	}

	// Check if we're moving into food in which the snake will have to grow
	if (theGrid.a[next_move.first][next_move.second] == FOOD) {
		// Clear the board to remove the snake from last iteration
		theGrid.reset();
		// Place the new snake location and food
		theSnake.add_head(next_move);
		theGrid.place_snake(theSnake);
		theGrid.new_food();
		theGrid.place_food();
		theSnake.myStats.foodCount++;
		theSnake.myStats.moveCount = theSnake.myStats.moveCount + 200;
		if (theSnake.myStats.moveCount > 300) {
			theSnake.myStats.moveCount = 300;
		}
		theSnake.myStats.score = theSnake.myStats.score - theSnake.myStats.timePenalty + 1000;
		theSnake.myStats.timePenalty = 0;
	}
	else {
		// Clear the board to remove the snake from last iteration
		theGrid.reset();
		// Place the new snake location and food
		theSnake.add_head(next_move);
		theSnake.remove_tail();
		theGrid.place_snake(theSnake);
		theGrid.place_food();
		theSnake.myStats.moveCount--;	// Decrease moveCount after every move
		theSnake.myStats.score++;		// Score increases each move to incentivize staying alive longer
		theSnake.myStats.timePenalty = theSnake.myStats.timePenalty + 2;
	}
	return;
}

void draw_scorecard(SDL_Renderer* renderer, TTF_Font* font, stats const &theStats, unsigned generation) {
	SDL_SetRenderDrawColor(renderer, 210, 210, 255, 255);	// Grey

	// Create Rectangle
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 200;
	rect.h = 600;
	SDL_RenderFillRect(renderer, &rect);

	SDL_Surface* surface = TTF_RenderText_Solid(font, "SNAKE", {0, 0, 0, 255});
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { 60, 10, texW, texH };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	surface = TTF_RenderText_Solid(font, "Food Eaten:", { 0, 0, 0, 255 });
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	dstrect = { 5, 100, texW, texH };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	string tmp = std::to_string(theStats.foodCount);
	char const* foodCount = tmp.c_str();
	surface = TTF_RenderText_Solid(font, foodCount, { 0, 0, 0, 255 });
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	dstrect = { 130, 100, texW, texH };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	surface = TTF_RenderText_Solid(font, "Moves Left:", { 0, 0, 0, 255 });
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	dstrect = { 5, 200, texW, texH };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	tmp = std::to_string(theStats.moveCount);
	char const* moveCount = tmp.c_str();
	surface = TTF_RenderText_Solid(font, moveCount, { 0, 0, 0, 255 });
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	dstrect = { 120, 200, texW, texH };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	surface = TTF_RenderText_Solid(font, "Score:", { 0, 0, 0, 255 });
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	dstrect = { 5, 300, texW, texH };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	tmp = std::to_string(theStats.score);
	char const* score = tmp.c_str();
	surface = TTF_RenderText_Solid(font, moveCount, { 0, 0, 0, 255 });
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	dstrect = { 80, 300, texW, texH };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	surface = TTF_RenderText_Solid(font, "Generation:", { 0, 0, 0, 255 });
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	dstrect = { 5, 400, texW, texH };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);

	tmp = std::to_string(generation);
	char const* generation_char = tmp.c_str();
	surface = TTF_RenderText_Solid(font, generation_char, { 0, 0, 0, 255 });
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	dstrect = { 120, 400, texW, texH };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	return;
}

void draw_game(SDL_Renderer* renderer,grid const theGrid, stats const &theStats) {
	// 601x601 pixel area for the 40x40 grid
	// 40 tiles 14x14 with 1 pixel space bordering each (40x14+41=601)

	int scorecardOffset = 201;	// 200 pixel scorecard + 1 pixel border
	for (int i = 0; i < GRID_HEIGHT; i++) {
		for (int j = 0; j < GRID_WIDTH; j++) {
			if (theGrid.a[i][j] == FOOD) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);	// Red
				SDL_Rect rect;
				rect.x = (j * 13) + (j * 2) + scorecardOffset;
				rect.y = (i * 13) + (i * 2) + 1;
				rect.w = 14;
				rect.h = 14;
				SDL_RenderFillRect(renderer, &rect);
			}
			else if (theGrid.a[i][j] == SNAKE) {
				if (theStats.gameOver == false) {
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);	// White
				}
				else {
					SDL_SetRenderDrawColor(renderer, 255, 155, 155, 255);	// Dark Red
				}
				SDL_Rect rect;
				rect.x = (j * 13) + (j * 2) + scorecardOffset;
				rect.y = (i * 13) + (i * 2) + 1;
				rect.w = 14;
				rect.h = 14;
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
}

// Will play the game using the given snake's brain
void visualize_snake(vector<pair<int, int >> testFood, net &snake_brain, SDL_Renderer* renderer, TTF_Font* font, unsigned generation) {
	bool isRunning = true;	// Flag controls graphics
	bool gameActive = true;	// Flag controls game
	directions newDirection = directions::DOWN;

	vector<unsigned> topology;
	topology.push_back(24);	// Input Layer
	topology.push_back(16);	// Hidden Layer(s)
	topology.push_back(16);
	topology.push_back(4);	// Output Layer
	snake theSnake(topology);

	theSnake.neuralnet = snake_brain;

	grid theGrid;
	theGrid.testFood = testFood;
	theGrid.foodIndex = 0;
	theGrid.foodCoords = theGrid.testFood[theGrid.foodIndex];

	while (isRunning) {
		SDL_Event event;

		theSnake.get_vision(theGrid.a);
		vector<double> inputVals = theSnake.vision;
		theSnake.neuralnet.feed_forward(inputVals);

		vector<double> resultVals;
		theSnake.neuralnet.get_results(resultVals);

		double highest_output = 0.0;
		for (unsigned n = 0; n < 4; n++) {
			if (resultVals[n] > highest_output) {
				switch (n) {
				case 0: if (theSnake.oldDirection != directions::DOWN) {
					newDirection = directions::UP;
					highest_output = resultVals[n];
				}
					  break;
				case 1: if (theSnake.oldDirection != directions::UP) {
					newDirection = directions::DOWN;
					highest_output = resultVals[n];
				}
					  break;
				case 2: if (theSnake.oldDirection != directions::RIGHT) {
					newDirection = directions::LEFT;
					highest_output = resultVals[n];
				}
					  break;
				case 3: if (theSnake.oldDirection != directions::LEFT) {
					newDirection = directions::RIGHT;
					highest_output = resultVals[n];
				}
					  break;

				}

			}
		}

		if (check_valid(theGrid, theSnake, newDirection)) {
			make_move(theGrid, theSnake, newDirection);
		}
		else {
			gameActive = false;
			theSnake.myStats.gameOver = true;
			theGrid.foodIndex = 0;
			theGrid.foodCoords = theGrid.testFood[theGrid.foodIndex];
		}
		theSnake.oldDirection = newDirection;	// Update the old direction
		if (theSnake.myStats.moveCount == 0) {
			gameActive = false;
			theSnake.myStats.gameOver = true;
			theGrid.foodIndex = 0;
			theGrid.foodCoords = theGrid.testFood[theGrid.foodIndex];
		}

		// (3) Clear and Draw the Screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);	// Black
		SDL_RenderClear(renderer);

		draw_game(renderer, theGrid, theSnake.myStats);
		draw_scorecard(renderer, font, theSnake.myStats, generation);

		SDL_RenderPresent(renderer);
		SDL_Delay(75);
		if (!gameActive) {
			SDL_Delay(1000);	// Pause the screen after game ends
			return;
		}
	}
}

int main(int argc, char *argv[]) {
	// Change these variables
	unsigned popSize = 2000;
	double mutation_rate = 0.01;

	// Create the SDL window and renderer
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 801, 601, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	TTF_Init();
	TTF_Font* font = TTF_OpenFont("arial.ttf", 22);

	// Create snake, grid, and stats objects
	//grid theGrid;
	//theGrid.new_food();

	vector<unsigned> topology;
	topology.push_back(24);	// Input Layer
	topology.push_back(16);	// Hidden Layer(s)
	topology.push_back(16);
	topology.push_back(4);	// Output Layer
	
	srand(1);

	vector<pair<int, int>> testFood;

	for (unsigned i = 0; i < 100; i++) {
		int x = rand() % 40;
		int y = rand() % 40;
		testFood.push_back(make_pair(y, x));
	}
	
	
	population pop(popSize, mutation_rate, topology);
	vector<pair<snake, snake>> fittest_snakes;

	for (unsigned generation = 1; generation <= 10; generation++) {
		if (generation > 1) {
			population pop2(popSize, mutation_rate, topology, fittest_snakes[generation - 2]);
			pop = pop2;
		}

		// Randomize the starting direction
		directions newDirection = directions::DOWN;

		bool isRunning = true;	// Flag controls graphics
		bool gameActive = true;	// Flag controls game
		// Main Application Loop
		for (unsigned i = 0; i < popSize; i++) {	// Loop through each snake and play the game
			grid theGrid;
			theGrid.testFood = testFood;
			theGrid.foodIndex = 0;
			theGrid.foodCoords = theGrid.testFood[theGrid.foodIndex];
			while (gameActive) {	// Main game loop
				pop.snakePop[i].get_vision(theGrid.a);
				vector<double> inputVals = pop.snakePop[i].vision;
				pop.snakePop[i].neuralnet.feed_forward(inputVals);

				vector<double> resultVals;
				pop.snakePop[i].neuralnet.get_results(resultVals);

				double highest_output = 0.0;
				for (unsigned n = 0; n < 4; n++) {
					if (resultVals[n] > highest_output) {
						switch (n) {
						case 0: if (pop.snakePop[i].oldDirection != directions::DOWN) {
							newDirection = directions::UP;
							highest_output = resultVals[n];
						}
							  break;
						case 1: if (pop.snakePop[i].oldDirection != directions::UP) {
							newDirection = directions::DOWN;
							highest_output = resultVals[n];
						}
							  break;
						case 2: if (pop.snakePop[i].oldDirection != directions::RIGHT) {
							newDirection = directions::LEFT;
							highest_output = resultVals[n];
						}
							  break;
						case 3: if (pop.snakePop[i].oldDirection != directions::LEFT) {
							newDirection = directions::RIGHT;
							highest_output = resultVals[n];
						}
							  break;

						}

					}
				}

				if (check_valid(theGrid, pop.snakePop[i], newDirection)) {
					make_move(theGrid, pop.snakePop[i], newDirection);
				}
				else {
					gameActive = false;
					pop.snakePop[i].myStats.gameOver = true;
					theGrid.foodIndex = 0;
					theGrid.foodCoords = theGrid.testFood[theGrid.foodIndex];
				}
				pop.snakePop[i].oldDirection = newDirection;	// Update the old direction
				if (pop.snakePop[i].myStats.moveCount == 0) {
					gameActive = false;
					pop.snakePop[i].myStats.gameOver = true;
					theGrid.foodIndex = 0;
					theGrid.foodCoords = theGrid.testFood[theGrid.foodIndex];
				}
			}
			gameActive = true;
		}

		fittest_snakes.push_back(pop.get_fittest_snakes());
		cout << "Generation " << generation << ": done loading." << endl;
		cout << "Fittest snake had score: " << fittest_snakes[generation - 1].first.myStats.score << endl;
	}

	for (unsigned generation = 1; generation <= 10; generation++) {
		net fittest_snake_brain = fittest_snakes[generation - 1].first.neuralnet;
		visualize_snake(testFood, fittest_snake_brain, renderer, font, generation);
	}
	

	TTF_CloseFont(font);
	TTF_Quit();
	font = NULL;
	return 0;
}