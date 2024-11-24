#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
//#include <SDL/SDL_rotozoom.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Surface * screen;
SDL_Surface * images[85];

enum ImageEnum {
    BADNIKS = 0,
    CRITTERS = 1,
    DR_ROBOTNIK = 2,
    DR_ROBOTNIK_GREEN_HILL = 3,
    DR_ROBOTNIK_MARBLE = 4,
    DR_ROBOTNIK_SCRAP_BRAIN = 5,
    DR_ROBOTNIK_SPRING_YARD = 6,
    SONIC = 7,
    TITLE_SCREEN = 14,
    GREEN_HILL_CHUNKS = 48
};

SDL_Rect getPos(int x, int y, int w, int h);
Uint32 getRGB(int r, int g, int b);
void addRect(SDL_Surface * screen, int x, int y, int w, int h, Uint32 rgb);
void showImage(SDL_Surface * image, SDL_Rect spritePos, SDL_Rect screenPos);
void safeQuit();

void loadImages();
SDL_Surface * loadImage(char path[]);

int main(int argc, char * argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) !=0) {
        fprintf(stderr,"Error in SDL_Init : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    screen = SDL_SetVideoMode(800, 255, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
    if (screen == NULL) {
        fprintf(stderr,"Error in SDL_SetVideoMode : %s\n", SDL_GetError());
        safeQuit();
    }

    SDL_WM_SetCaption("Sonic The Hedgehog", NULL);

    //addRect(screen, 50, 50, 200, 100, getRGB(64, 118, 173));
    //addRect(screen, 200, 50, 100, 200, getRGB(64, 140, 110));

    loadImages();

    //SDL_Surface * titleScreenBg = images[TITLE_SCREEN];
    //SDL_Rect titleScreenBgSprite = getPos(24, 213, 1024, 112);
    //SDL_Rect titleScreenBgPos = getPos(0, 0, -1, -1);

    SDL_Surface * Sonic = images[SONIC];
    SDL_Rect sonicSprite = getPos(43, 257, 32, 40);
    SDL_Rect sonicPos = getPos(25, 140, -1, -1);

    SDL_Surface * GreenHillChunks = images[GREEN_HILL_CHUNKS];
    SDL_Rect GreenHillSprite = getPos(25, 264, 255, 255);
    SDL_Rect GreenHillPos = getPos(0, 0, -1, -1);

    int active = 1;
    SDL_Event event;
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    while (active) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                active = 0;
                break;
            case SDL_KEYDOWN:
                SDL_FillRect(screen, NULL, 0x000000);
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        active = 0;
                        break;
                    case SDLK_UP:
                    case SDLK_z:
                    case SDLK_DOWN:
                    case SDLK_s:
                    case SDLK_LEFT:
                    case SDLK_q:
                    case SDLK_RIGHT:
                    case SDLK_d:
                        switch (event.key.keysym.sym) {
                            case SDLK_UP:
                            case SDLK_z:
                                sonicSprite.x = 425;
                                sonicSprite.y = 257;
                                break;
                            case SDLK_DOWN:
                            case SDLK_s:
                                sonicSprite.x = 507;
                                sonicSprite.y = 265;
                                sonicSprite.h = 32;
                                sonicPos.y += 8;
                                break;
                            case SDLK_LEFT:
                            case SDLK_q:
                                sonicPos.x -= 10;
                                break;
                            case SDLK_RIGHT:
                            case SDLK_d:
                                sonicPos.x += 10;
                        }
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                    case SDLK_z:
                    case SDLK_DOWN:
                    case SDLK_s:
                        sonicSprite.x = 43;
                        sonicSprite.y = 257;
                        switch (event.key.keysym.sym) {
                            case SDLK_DOWN:
                            case SDLK_s:
                                sonicSprite.w = 32;
                                sonicSprite.h = 40;
                                sonicPos.y -= 8;
                                break;
                        }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                break;
            case SDL_MOUSEBUTTONUP:
                break;
            case SDL_MOUSEMOTION:
                break;
            case SDL_VIDEORESIZE:
                SDL_SetVideoMode(event.resize.w, event.resize.h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
                break;

            default: break;
        }

        //showImage(titleScreenBg, titleScreenBgSprite, titleScreenBgPos);
        showImage(GreenHillChunks, GreenHillSprite, GreenHillPos);
        showImage(Sonic, sonicSprite, sonicPos);
        SDL_Flip(screen);
    }
    SDL_Quit();
    exit(EXIT_SUCCESS);
}

void safeQuit() {
    if (screen != NULL) SDL_FreeSurface(screen);
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void showImage(SDL_Surface * image, SDL_Rect spritePos, SDL_Rect screenPos) {
    SDL_BlitSurface(image, &spritePos, screen, &screenPos);
}

SDL_Rect getPos(int x, int y, int w, int h) {
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    if (w != -1) pos.w = w;
    if (h != -1) pos.h = h;
    return pos;
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

// sprites.c

void loadImages() {
    //images[BADNIKS] = loadImage("global/Badniks");
    //images[CRITTERS] = loadImage("global/Critters");
    //images[DR_ROBOTNIK] = loadImage("global/Dr Robotnik");

    // sRGB errors, I'll fix them later...
    //images[DR_ROBOTNIK_GREEN_HILL] = loadImage("global/Dr Robotnik Green Hill Zone");
    //images[DR_ROBOTNIK_MARBLE] = loadImage("global/Dr Robotnik Marble Zone");
    //images[DR_ROBOTNIK_SCRAP_BRAIN] = loadImage("global/Dr Robotnik Scrap Brain Zone");
    //images[DR_ROBOTNIK_SPRING_YARD] = loadImage("global/Dr Robotnik Spring Yard");
    images[SONIC] = loadImage("global/Sonic");
    images[TITLE_SCREEN] = loadImage("miscellaneous/Title Screen");
    images[GREEN_HILL_CHUNKS] = loadImage("stage_chunks/Green Hill Zone Chunks");
}

SDL_Surface * loadImage(char path[]) {
    char * newPath = (char *) malloc(strlen(path) + 15);
    strcpy(newPath, "sprites/");
    strcat(newPath, path);
    strcat(newPath, ".png");
    SDL_Surface * image = IMG_Load(newPath);
    if (image == NULL) {
        fprintf(stderr, "Error in SDL_LoadBMP(\"%s\") : %s\n", newPath, SDL_GetError());
    }
    free(newPath);
    return image;
}