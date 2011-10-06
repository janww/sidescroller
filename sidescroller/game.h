#include <iostream>
#include <fstream>
#include <vector>
#include <SDL.h>
#include "base.h"
#include "player.h"

class game{
	SDL_Surface* screen, *background, *block;
	SDL_Rect camera;
	std::vector<std::vector<int>> map;
	bool direction[2];
	bool running;

	SDL_Surface* load_image(const char* filename);
	void loadmap(const char* filename);
	void showmap();
	void handleEvents();

	static const int SCREEN_WIDTH = 640;
	static const int SCREEN_HEIGHT = 480;
	static const int SCREEN_BPP = 32;

	player* player1;

	public:
		game();
		~game();
		void start();
};