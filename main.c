#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Surface * screen;

Uint32 getRGB(int r, int g, int b);
void addRect(SDL_Surface * screen, int x, int y, int w, int h, Uint32 rgb);
SDL_Surface * loadImage(char path[]);
void showImage(SDL_Surface * image, int x, int y, int width, int height, int screenX, int screenY);
void safeQuit();

int main(int argc, char * argv[]){
    if (SDL_Init(SDL_INIT_EVERYTHING) !=0) {
        fprintf(stderr,"Error in SDL_Init : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL) {
        fprintf(stderr,"Error in SDL_SetVideoMode : %s\n", SDL_GetError());
        safeQuit();
    }

    SDL_WM_SetCaption("Sonic The Hedgehog", NULL);

    addRect(screen, 50, 50, 200, 100, getRGB(64, 118, 173));
    addRect(screen, 200, 50, 100, 200, getRGB(64, 140, 110));

    SDL_Surface * Sonic = loadImage("sprites/global/Sonic.png");
    showImage(Sonic, 43, 257, 32, 40, 50, 50);

    int active = 1;
    SDL_Event event;
    while (active) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                active = 0;
                break;
            case SDL_KEYUP:
                printf("\nkey pressed!");
                break;
            default: break;
        }
    }
    SDL_Quit();
    exit(EXIT_SUCCESS);
}

void safeQuit() {
    if (screen != NULL) SDL_FreeSurface(screen);
    SDL_Quit();
    exit(EXIT_FAILURE);
}

SDL_Surface * loadImage(char path[]) {
    SDL_Surface * image = IMG_Load(path);
    if (image == NULL) {
        fprintf(stderr, "Error in SDL_LoadBMP(\"%s\") : %s\n", path, SDL_GetError());
        safeQuit();
    }
    return image;
}

void showImage(SDL_Surface * image, int x, int y, int width, int height, int screenX, int screenY) {
    SDL_Rect tilePos;
    SDL_Rect screenPos;
    tilePos.x = x;
    tilePos.y = y;
    tilePos.w = width;
    tilePos.h = height;
    screenPos.x = screenX;
    screenPos.y = screenY;
    SDL_BlitSurface(image, &tilePos, screen, &screenPos);
    SDL_Flip(screen);
}

Uint32 getRGB(int r, int g, int b) {
    return SDL_MapRGB(screen -> format, r, g, b);
}

void addRect(SDL_Surface * screen, int x, int y, int w, int h, Uint32 rgb) {
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;
    SDL_FillRect(screen, &rectangle, rgb);
    SDL_Flip(screen);
}