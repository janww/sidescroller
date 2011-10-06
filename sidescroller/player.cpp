#include "player.h"

player::player(SDL_Surface* img)
{
	image = img;
	box.x = 0;
	box.y = 0;
	box.w = 50;
	box.h = 100;
	xvel = 0;
	yvel = 0;
	ground = false;
	
	for(int i=0; i < player::FRAMES; i++)
	{
		frames[i].x = baseclass::TILE_BORDER_SIZE + i * 50;
		frames[i].y = 0;
		frames[i].w = 50;
		frames[i].h = 100;
	}
}

player::~player()
{
	SDL_FreeSurface(image);
}

SDL_Rect* player::getRect()
{
	return &box;
}

int player::getXvel()
{
	return xvel;
}

void player::setXvel(int vel)
{
	xvel = vel;
}

void player::show(SDL_Surface* screen)
{
	SDL_BlitSurface(image, &frames[0], screen, &box);
}

void player::move(const std::vector<std::vector<int>>&map)
{
	int start = (baseclass::coord.x - (baseclass::coord.x % baseclass::TILE_SIZE)) / baseclass::TILE_SIZE;
	int end = (baseclass::coord.x + baseclass::coord.w + (baseclass::TILE_SIZE - (baseclass::coord.x + baseclass::coord.w) % baseclass::TILE_SIZE)) / baseclass::TILE_SIZE;
	
	if(start < 0)
		start = 0;
	
	if(end > map[0].size())
		end = map[0].size();

	bool nocollision = false;

	for(int i = 0; i < map.size(); i++)
		for(int j = start; j < end; j++)
		{
			if(map[i][j] == 0)
				continue;
			SDL_Rect destrect = {j * 50 - baseclass::coord.x, i * 50, 50, 50};
			if(collision(&box, &destrect))
			{
				nocollision = true;
				if(destrect.y >= box.y + box.h - 11)
				{
					ground = true;
					yvel = 0;
				} else if(destrect.y + destrect.h <= box.y + 11)
				{
					box.x++;
					yvel = 5;
				}

				if(box.x + box.w >= destrect.x - 5 && box.y + box.h >= destrect.y + 6 && box.x + box.w <= destrect.x + 20)
				{
					xvel = 0;
					box.x--;
				} else if(box.x <= destrect.x + destrect.w && box.y + box.h >= destrect.y + 6)
				{
					xvel = 0;
					box.x++;
				}

			}
		}
	if(!nocollision)
	{
		yvel = 5;
	}	
	
	box.x += xvel;
	box.y += yvel;
}