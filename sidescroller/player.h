#include <SDL.h>
#include <iostream>
#include <vector>
#include "base.h"

class player:baseclass
{
	public:
		static const int FRAMES = 4;
	
	SDL_Rect box;
	SDL_Surface* image;
	int xvel, yvel;
	SDL_Rect frames[FRAMES];
	bool ground;


	public:
		player(SDL_Surface* img);
		~player();

	SDL_Rect* getRect();
	void setXvel(int vel);
	int getXvel();
	void move(const std::vector<std::vector<int>>&map);
	void show(SDL_Surface* screen);

	
};