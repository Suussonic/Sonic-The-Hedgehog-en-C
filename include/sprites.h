#ifndef DEF_HEADER_SPRITES_LIB
#define DEF_HEADER_SPRITES_LIB

#include <SDL.h>

typedef enum ImageEnum {
    BADNIKS = 0,
    CRITTERS = 1,
    DR_ROBOTNIK = 2,
    SONIC = 3,
    TITLE_SCREEN = 10,
    GREEN_HILL_BACKGROUND = 13,
    GREEN_HILL_FOREGROUND = 14,
    DR_ROBOTNIK_GREEN_HILL = 15
} ImageEnum;

typedef struct SpriteTexture {
    SDL_Surface * image;
    SDL_Rect sprite;
    int backgroundColor;
    int flipped;
} SpriteTexture;

typedef struct Animation {
    char * name;
    int max;
    SDL_Rect * sprites;
} Animation;

// Déclaration des tableaux de positions des sprites de Sonic
extern SDL_Rect sonic_standing;

void loadImages(SDL_Surface * screen);
void loadImage(ImageEnum img, char path[], Uint32 backgroundColor);

int getBackGroundColor(ImageEnum image);
SDL_Surface * getImage(ImageEnum image);

Uint32 color(int r, int g, int b);
SDL_Surface * zoomSurf(SpriteTexture * sprite, int zoomx, int zoomy, int smooth);
SDL_Rect getSonicSprite(const char *action, int frame);


#endif