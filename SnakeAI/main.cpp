#include "SDL.h"
#include "SDL_ttf.h"
#include "grid.h"
#include "snake.h"
#include "stats.h"
#include <string>
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

void make_move(grid& theGrid, snake& theSnake, directions direction, stats& theStats) {
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
		theStats.foodCount++;
		theStats.moveCount++;
	}
	else {
		// Clear the board to remove the snake from last iteration
		theGrid.reset();
		// Place the new snake location and food
		theSnake.add_head(next_move);
		theSnake.remove_tail();
		theGrid.place_snake(theSnake);
		theGrid.place_food();
		theStats.moveCount++;
	}
	return;
}

void draw_scorecard(SDL_Renderer* renderer, TTF_Font* font, stats& theStats) {
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

	surface = TTF_RenderText_Solid(font, "Moves:", { 0, 0, 0, 255 });
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
	dstrect = { 80, 200, texW, texH };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	return;
}

void draw_game(SDL_Renderer* renderer, grid theGrid, stats& theStats) {
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

int main(int argc, char *argv[]) {

	// Create the SDL window and renderer
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 801, 601, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	TTF_Init();
	TTF_Font* font = TTF_OpenFont("arial.ttf", 22);

	// Create snake, grid, and stats objects
	grid theGrid;
	theGrid.new_food();
	snake theSnake;
	stats theStats;
	
	srand(2);

	// Randomize the starting direction
	directions newDirection = directions(rand()%4);

	bool isRunning = true;	// Flag controls graphics
	bool gameActive = true;	// Flag controls game
	// Main Application Loop
	while (isRunning) {
		SDL_Event event;
		
		// (1) Handle Input
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isRunning = false;
			}
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_LEFT:
						if (theSnake.oldDirection != directions::RIGHT) {	// prevent the snake from backing into itself
							newDirection = directions::LEFT;
						}
						break;
					case SDLK_RIGHT:
						if (theSnake.oldDirection != directions::LEFT) {
							newDirection = directions::RIGHT;
						}
						break;
					case SDLK_UP:
						if (theSnake.oldDirection != directions::DOWN) {
							newDirection = directions::UP;
						}
						break;
					case SDLK_DOWN:
						if (theSnake.oldDirection != directions::UP) {
							newDirection = directions::DOWN;
						}
						break;
					default:
						break;
				}
			}
		}

		// (2) Handle Updates
		if (gameActive) {
			if (check_valid(theGrid, theSnake, newDirection)) {
				make_move(theGrid, theSnake, newDirection, theStats);
			}
			else {
				gameActive = false;
				theStats.gameOver = true;
			}
		}
		theSnake.oldDirection = newDirection;	// Update the old direction

		// (3) Clear and Draw the Screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);	// Black
		SDL_RenderClear(renderer);

		draw_game(renderer, theGrid, theStats);
		draw_scorecard(renderer, font, theStats);

		SDL_RenderPresent(renderer);
		SDL_Delay(75);
	}

	TTF_CloseFont(font);
	TTF_Quit();
	font = NULL;
	return 0;
}