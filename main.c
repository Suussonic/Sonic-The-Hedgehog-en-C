#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>

void addRect(SDL_Surface * screen, int x, int y, int w, int h, int r, int g, int b);

int main(int argc, char * argv[]){
    if(SDL_Init(SDL_INIT_EVERYTHING) !=0){
        fprintf(stderr,"ErrorinSDL_Init:%s\n",SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Surface * screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL) {
        fprintf(stderr,"ErrorinSDL_SetVideoMode:%s\n",
        SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("Sonic The Hedgehog",NULL);

    addRect(screen, 50, 50, 200, 100, 64, 118, 173);
    addRect(screen, 200, 50, 100, 200, 64, 140, 110);

    int active = 1;
    SDL_Event event;
    while(active){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                active=0;
                break;
            case SDL_KEYUP:
                printf("key pressed!");
                break;
            default: break;
        }
    }
    SDL_Quit();
    exit(EXIT_SUCCESS);
}
void addRect(SDL_Surface * screen, int x, int y, int w, int h, int r, int g, int b) {
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;
    SDL_FillRect(screen, &rectangle, SDL_MapRGB(screen->format, r, g, b));
    SDL_Flip(screen);
}