#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define TICK_INTERNAL 100

#define SDL_ERR() (fprintf(stderr, "%s:%d %s\n", __FILE__, __LINE__, SDL_GetError()), exit(EXIT_FAILURE))

int main() {

	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0)
		SDL_ERR();

	SDL_Window *win = SDL_CreateWindow("Test",
	                                   SDL_WINDOWPOS_CENTERED,
	                                   SDL_WINDOWPOS_CENTERED,
	                                   SCREEN_WIDTH,
	                                   SCREEN_HEIGHT,
	                                   SDL_WINDOW_SHOWN);
	if(win == NULL) SDL_ERR();

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(ren == NULL) SDL_ERR();

	SDL_SetRenderDrawColor(ren, 0,0,0,255);


	SDL_Texture *txt = IMG_LoadTexture(ren, "img/png/TestBox.png");
	if(txt == NULL) SDL_ERR();

	SDL_Event e;
	SDL_bool quit = SDL_FALSE;

	Uint32 next = SDL_GetTicks() + TICK_INTERNAL;

	unsigned short pillars = 0;

	while (!quit) {
		while (SDL_PollEvent(&e))
			if(e.type == SDL_QUIT)
				quit = SDL_TRUE;

		SDL_RenderClear(ren);

		// floor
		for(int i=0; i<8; i++)
			for(int j=0; j<8; j++) {
				SDL_Rect r;
				r.x = SCREEN_WIDTH/2 + ((i - j - 1) * 32);
				r.y = 100 + (i + j) * 16;;
				r.w = 64;
				r.h = 96;
				SDL_RenderCopy(ren, txt, NULL, &r);
			}

		// walls

		for(int i=0; i<8; i++) {
			for(int j=0; j<8; j++) {
				if((pillars & 0xff) & (1 << i)) continue;
				if((pillars >> 8) & (1 << j)) continue;


				SDL_Rect r;
				r.x = SCREEN_WIDTH/2 + ((i - j - 1) * 32);
				r.y = 34 + (i + j) * 16;;
				r.w = 64;
				r.h = 96;
				SDL_RenderCopy(ren, txt, NULL, &r);
			}
		}

		pillars++;
		SDL_RenderPresent(ren);

		// make sure the framerate is more
		// or less fine and don't fty the cpu

		Uint32 current = SDL_GetTicks();
		if(next > current)
			SDL_Delay(next - current);
		next += TICK_INTERNAL;
	}



	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return EXIT_SUCCESS;
}
