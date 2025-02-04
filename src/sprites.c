#include <sprites.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>
#include <stdlib.h>
#include <string.h>

SDL_Surface * images[85];
int backgroundColors[85];

SDL_Surface * spritesScreen = NULL;

// Déclaration des tableaux de positions des sprites de Sonic
extern SDL_Rect sonic_normal_movement[];
extern SDL_Rect sonic_fast_movement[];
extern SDL_Rect sonic_jump_roll[];
extern SDL_Rect sonic_special_positions[];

SDL_Rect sonic_normal_movement[] = {
    {43, 257, 32, 40}, {85, 257, 32, 40}, {127, 257, 32, 40}, {169, 257, 32, 40}
};

SDL_Rect sonic_fast_movement[] = {
    {211, 257, 32, 40}, {253, 257, 32, 40}, {295, 257, 32, 40}, {337, 257, 32, 40}
};

SDL_Rect sonic_jump_roll[] = {
    {379, 257, 32, 40}, {421, 257, 32, 40}, {463, 257, 32, 40}, {505, 257, 32, 40}
};

SDL_Rect sonic_special_positions[] = {
    {547, 257, 32, 40}, {589, 257, 32, 40}, {631, 257, 32, 40}, {673, 257, 32, 40}
};

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
    loadImage(SONIC, "global/sonic", color(67, 153, 49));
    loadImage(TITLE_SCREEN, "miscellaneous/Title Screen", -1);
    loadImage(GREEN_HILL_BACKGROUND, "stage_background/Green Hill Zone Background", -1);
    loadImage(GREEN_HILL_CHUNKS, "stage_chunks/Green Hill Zone Chunks", color(135, 16, 19));
}

// Fonction pour obtenir la position d'un sprite en fonction de l'action
SDL_Rect getSonicSprite(const char *action, int frame) {
    if (strcmp(action, "normal") == 0 && frame < 4) return sonic_normal_movement[frame];
    if (strcmp(action, "fast") == 0 && frame < 4) return sonic_fast_movement[frame];
    if (strcmp(action, "jump") == 0 && frame < 4) return sonic_jump_roll[frame];
    if (strcmp(action, "special") == 0 && frame < 4) return sonic_special_positions[frame];

    // Par défaut, retourner la première position normale
    return sonic_normal_movement[0];
}
