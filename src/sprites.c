#include <sprites.h>

#include <SDL_image.h>
#include <SDL_rotozoom.h>

SDL_Surface * images[85];
int backgroundColors[85];

SDL_Surface * spritesScreen = NULL;

void loadImage(ImageEnum img, char path[], Uint32 backgroundColor) {
    char * newPath = (char *) malloc(strlen(path) + 15);
    strcpy(newPath, "assets/sprites/");
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

SDL_Surface * zoomSurf(SpriteTexture * sprite, int zoomx, int zoomy, int smooth) {
    SDL_Surface * newSurface = zoomSurface(sprite->image, zoomx, zoomy, smooth);
    newSurface = SDL_DisplayFormat(newSurface);
    if (sprite->backgroundColor != -1)
        SDL_SetColorKey(newSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL, sprite->backgroundColor);
    return newSurface;
}

int getBackGroundColor(ImageEnum image) {
    return backgroundColors[image];
}
SDL_Surface * getImage(ImageEnum image) {
    return images[image];
}

Uint32 color(int r, int g, int b) {
    return SDL_MapRGB(spritesScreen->format, r, g, b);
}

void loadImages(SDL_Surface * screen) {
    spritesScreen = screen;
    loadImage(BADNIKS, "global/Badniks", color(67, 153, 49));
    //images[CRITTERS] = loadImage("global/Critters");
    //images[DR_ROBOTNIK] = loadImage("global/Dr Robotnik");

    // sRGB errors, I'll fix them later...
    //images[DR_ROBOTNIK_GREEN_HILL] = loadImage("global/Dr Robotnik Green Hill Zone");
    //images[DR_ROBOTNIK_MARBLE] = loadImage("global/Dr Robotnik Marble Zone");
    //images[DR_ROBOTNIK_SCRAP_BRAIN] = loadImage("global/Dr Robotnik Scrap Brain Zone");
    //images[DR_ROBOTNIK_SPRING_YARD] = loadImage("global/Dr Robotnik Spring Yard");
    loadImage(SONIC, "global/sonic", color(67, 153, 49));
    loadImage(TITLE_SCREEN, "miscellaneous/Title Screen", -1);
    loadImage(GREEN_HILL_BACKGROUND, "stage_background/Green Hill Zone Background", -1);
    loadImage(GREEN_HILL_CHUNKS, "stage_chunks/Green Hill Zone Chunks", color(135, 16, 19));
}
