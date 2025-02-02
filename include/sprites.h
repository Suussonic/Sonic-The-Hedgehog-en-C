#ifndef DEF_HEADER_SPRITES_LIB
#define DEF_HEADER_SPRITES_LIB

#include <SDL.h>

typedef enum ImageEnum {
    BADNIKS = 0,
    CRITTERS = 1,
    DR_ROBOTNIK = 2,
    DR_ROBOTNIK_GREEN_HILL = 3,
    DR_ROBOTNIK_MARBLE = 4,
    DR_ROBOTNIK_SCRAP_BRAIN = 5,
    DR_ROBOTNIK_SPRING_YARD = 6,
    SONIC = 7,
    TITLE_SCREEN = 14,
    GREEN_HILL_BACKGROUND = 23,
    GREEN_HILL_CHUNKS = 48
} ImageEnum;

typedef struct SpriteTexture {
    SDL_Surface * image;
    SDL_Rect sprite;
    int backgroundColor;
    int flipped;
} SpriteTexture;

void loadImages(SDL_Surface * screen);
void loadImage(ImageEnum img, char path[], Uint32 backgroundColor);

int getBackGroundColor(ImageEnum image);
SDL_Surface * getImage(ImageEnum image);

Uint32 color(int r, int g, int b);
SDL_Surface * zoomSurf(SpriteTexture * sprite, int zoomx, int zoomy, int smooth);

#endif