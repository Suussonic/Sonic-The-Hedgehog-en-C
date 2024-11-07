#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface * ecran = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);
    SDL_Event event;
    int active = 1;
    while(active) {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT : active = 0; break;
        }
    }
    SDL_FreeSurface(ecran);
    SDL_Quit();
    exit(EXIT_SUCCESS);
}