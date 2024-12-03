#include <SDL_image.h>

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

void loadImages() {
    images[BADNIKS] = loadImage("global/Badniks");
    //images[CRITTERS] = loadImage("global/Critters");
    //images[DR_ROBOTNIK] = loadImage("global/Dr Robotnik");

    // sRGB errors, I'll fix them later...
    //images[DR_ROBOTNIK_GREEN_HILL] = loadImage("global/Dr Robotnik Green Hill Zone");
    //images[DR_ROBOTNIK_MARBLE] = loadImage("global/Dr Robotnik Marble Zone");
    //images[DR_ROBOTNIK_SCRAP_BRAIN] = loadImage("global/Dr Robotnik Scrap Brain Zone");
    //images[DR_ROBOTNIK_SPRING_YARD] = loadImage("global/Dr Robotnik Spring Yard");
    images[SONIC] = loadImage("global/sonic");
    images[TITLE_SCREEN] = loadImage("miscellaneous/Title Screen");
    images[GREEN_HILL_CHUNKS] = loadImage("stage_chunks/Green Hill Zone Chunks");
}
