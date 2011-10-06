#include <SDL.h>

#ifndef BASE_H
#define BASE_H

class baseclass
{
	protected:
		bool collision(SDL_Rect* rect1, SDL_Rect* rect2);
	
	public:
		static SDL_Rect coord;
		static const int TILE_SIZE = 50;
		static const int TILE_BORDER_SIZE = 1;
};
#endif