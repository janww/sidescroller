#include "game.h"
#include "base.h"

SDL_Rect baseclass::coord;

game::game()
{
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	SDL_WM_SetCaption("SDL Side Scroller", "SDL Side Scroller");
	background = load_image("res/tex/background.bmp");
	block = load_image("res/tex/blocks.bmp");
	baseclass::coord.x = baseclass::coord.y = camera.x = camera.y = 0;
	baseclass::coord.w = camera.w = SCREEN_WIDTH;
	baseclass::coord.h = camera.h = SCREEN_HEIGHT;
	direction[0] = direction[1] = false;
	running = true;

	player1 = new player(load_image("res/tex/player.bmp"));
}

game::~game()
{
	SDL_FreeSurface(background);
	SDL_FreeSurface(block);
	player1 = NULL;
	SDL_Quit();
}

SDL_Surface* game::load_image(const char* filename)
{
	SDL_Surface* tmp = SDL_LoadBMP(filename);
	SDL_Surface* tmp2 = SDL_DisplayFormat(tmp);
	SDL_SetColorKey(tmp2, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xff, 0x00, 0xff));
	SDL_FreeSurface(tmp);
	return tmp2;
}

void game::handleEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				running = false;
				return;							
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_LEFT:
						direction[0] = true;
						break;
					case SDLK_RIGHT:
						direction[1] = true;
						break;
				}
				break;
			case SDL_KEYUP:
				switch(event.key.keysym.sym)
				{
					case SDLK_LEFT:
						direction[0] = false;
						break;
					case SDLK_RIGHT:
						direction[1] = false;
						break;
				}
				break;
		}
	}
}

void game::loadmap(const char* filename)
{
	std::ifstream in(filename);
	if(!in.is_open())
	{
		std::cout << "Problem with loading the map!" << std::endl;
		return;
	}
	int width, height;
	in >> width;
	in >> height;
	int current;
	for(int i = 0; i < height; i++)
	{
		//std::vector<std::vector<int>> map;
		std::vector<int> vec;
		for(int j = 0; j < width; j++)
		{
			if(in.eof())
			{
				std::cout << "End of file reached too early!" << std::endl;
				return;
			}
			in >> current;
			if (current >= 0 && current <= 1)
			{
				vec.push_back(current);
			}else
			{
				vec.push_back(0);
			}
		}
		map.push_back(vec);
	}
	in.close();
}

void game::showmap()
{
	int start = (baseclass::coord.x - (baseclass::coord.x % baseclass::TILE_SIZE)) / baseclass::TILE_SIZE;
	int end = (baseclass::coord.x + baseclass::coord.w + (baseclass::TILE_SIZE - (baseclass::coord.x + baseclass::coord.w) % baseclass::TILE_SIZE)) / baseclass::TILE_SIZE;
	
	if(start < 0)
		start = 0;
	
	if(end > map[0].size())
		end = map[0].size();

	for(int i = 0; i < map.size(); i++)
		for(int j = start; j < end; j++)
			if(map[i][j] != 0)
			{	
				SDL_Rect blockrect = {baseclass::TILE_BORDER_SIZE + (map[i][j] - 1) * baseclass::TILE_SIZE, 1, baseclass::TILE_SIZE, baseclass::TILE_SIZE};
				SDL_Rect destrect = {j * baseclass::TILE_SIZE - baseclass::coord.x, i * baseclass::TILE_SIZE};
				SDL_BlitSurface(block, &blockrect, screen, &destrect);
			}
}


void game::start()
{
	Uint32 start;
	loadmap("map.map");
	while(running)
	{
		start = SDL_GetTicks();
		handleEvents();

		if(direction[0])
		{
			if(player1->getRect()->x > 0)
				player1->setXvel(-1);
			else
			{
				player1->setXvel(0);
				camera.x--;
				baseclass::coord.x--;

			}
			if(camera.x < 0)
				camera.x = 2560-SCREEN_WIDTH;
		}else if(direction[1])
		{
			camera.x++;
			baseclass::coord.x++;
			if(camera.x >= 2560-SCREEN_WIDTH)
				camera.x = 0;
		}

		player1->move(map);

		SDL_BlitSurface(background, &camera, screen, NULL);
		showmap();
		player1->show(screen);

		SDL_Flip(screen);
		if(1000/30 > (SDL_GetTicks() - start))
			SDL_Delay(1000/30 - (SDL_GetTicks() - start));
	}
}