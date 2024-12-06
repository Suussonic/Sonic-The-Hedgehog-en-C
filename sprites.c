#include <SDL_image.h>
#include <SDL_rotozoom.h>

SDL_Surface * images[85];
int backgroundColors[85];

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
    GREEN_HILL_BACKGROUND = 23,
    GREEN_HILL_CHUNKS = 48
};

typedef struct {
    SDL_Surface * image;
    SDL_Rect sprite;
    SDL_Rect pos;
    int flipped;
    int backgroundColor;
} Sprite;

void loadImage(enum ImageEnum img, char path[], Uint32 backgroundColor) {
    char * newPath = (char *) malloc(strlen(path) + 15);
    strcpy(newPath, "sprites/");
    strcat(newPath, path);
    strcat(newPath, ".png");
    SDL_Surface * image = IMG_Load(newPath);
    if (image == NULL) {
        fprintf(stderr, "Error in IMG_Load(\"%s\") : %s\n", newPath, SDL_GetError());
    }
    if (backgroundColor != -1) {
        image = SDL_DisplayFormat(image);
        SDL_SetColorKey(image, SDL_SRCCOLORKEY, backgroundColor);
    }
    free(newPath);
    images[img] = image;
    backgroundColors[img] = backgroundColor;
}

SDL_Surface * zoomSurf(Sprite * sprite, int zoomx, int zoomy, int smooth) {
    SDL_Surface * newSurface = zoomSurface(sprite->image, zoomx, zoomy, smooth);
    newSurface = SDL_DisplayFormat(newSurface);
    if (sprite->backgroundColor != -1)
        SDL_SetColorKey(newSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL, sprite->backgroundColor);
    return newSurface;
}

Uint32 color(SDL_Surface * screen, int r, int g, int b) {
    return SDL_MapRGB(screen->format, r, g, b);
}

void loadImages(SDL_Surface * screen) {
    loadImage(BADNIKS, "global/Badniks", color(screen, 67, 153, 49));
    //images[CRITTERS] = loadImage("global/Critters");
    //images[DR_ROBOTNIK] = loadImage("global/Dr Robotnik");

    // sRGB errors, I'll fix them later...
    //images[DR_ROBOTNIK_GREEN_HILL] = loadImage("global/Dr Robotnik Green Hill Zone");
    //images[DR_ROBOTNIK_MARBLE] = loadImage("global/Dr Robotnik Marble Zone");
    //images[DR_ROBOTNIK_SCRAP_BRAIN] = loadImage("global/Dr Robotnik Scrap Brain Zone");
    //images[DR_ROBOTNIK_SPRING_YARD] = loadImage("global/Dr Robotnik Spring Yard");
    loadImage(SONIC, "global/sonic", color(screen, 67, 153, 49));
    loadImage(TITLE_SCREEN, "miscellaneous/Title Screen", -1);
    loadImage(GREEN_HILL_BACKGROUND, "stage_background/Green Hill Zone Background", -1);
    loadImage(GREEN_HILL_CHUNKS, "stage_chunks/Green Hill Zone Chunks", color(screen, 135, 16, 19));
}

Sprite setSprite(enum ImageEnum image, SDL_Rect spritePos, SDL_Rect pos) {
    Sprite sprite;
    sprite.image = images[image];
    sprite.sprite = spritePos;
    sprite.pos = pos;
    sprite.backgroundColor = backgroundColors[image];
    sprite.flipped = 0;
    return sprite;
}