#include <sprites.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>

SDL_Surface * images[17];
int backgroundColors[17];
Animation animations[4];
int animationsAmount = 4;

SDL_Surface * spritesScreen = NULL;

SDL_Rect sonic_standing = {43, 257, 32, 40};
SDL_Rect sprites_walking[6] = {
        {46, 349, 24, 40},
        {109, 347, 40, 40},
        {178, 348, 32, 40},
        {249, 349, 40, 40},
        {319, 347, 40, 40},
        {390, 348, 40, 40}
};
SDL_Rect sprites_running[6] = {
        {46, 349, 23, 39},
        {108, 346, 40, 40},
        {177, 347, 32, 40},
        {248, 348, 40, 40},
        {318, 346, 40, 40},
        {389, 347, 40, 40}
};
SDL_Rect sprites_jumping[4] = {
        {46, 349, 23, 39},
        {46, 349, 23, 39},
        {46, 349, 23, 39},
        {46, 349, 23, 39},
};
SDL_Rect sprites_special[4] = {
        {46, 349, 23, 39},
        {46, 349, 23, 39},
        {46, 349, 23, 39},
        {46, 349, 23, 39},
};

Animation sonic_walking = {
        .name = "walk",
        .max = 6,
        .sprites = sprites_walking
};

Animation sonic_running = {
        .name = "run",
        .max = 6,
        .sprites = sprites_running
};

Animation sonic_jumping = {
        .name = "jump",
        .max = 4,
        .sprites = sprites_jumping
};

// Positions spéciales (Special Positions)
Animation sonic_special_positions = {
        .name = "special",
        .max = 4,
        .sprites = sprites_special
};
// "For his neutral special, he wields a gun" :D

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
        SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL, backgroundColor); // Ajout de SDL_RLEACCEL pour optimiser la transparence
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
    loadImage(BADNIKS, "global/badniks", color(67, 153, 49));
    //images[CRITTERS] = loadImage("global/Critters");
    //images[DR_ROBOTNIK] = loadImage("global/Dr Robotnik");

    // sRGB errors, I'll fix them later...
    //images[DR_ROBOTNIK_GREEN_HILL] = loadImage("global/Dr Robotnik Green Hill Zone");
    loadImage(SONIC, "global/sonic", color(67, 153, 49));
    //loadImage(TITLE_SCREEN, "miscellaneous/Title Screen", -1);
    loadImage(GREEN_HILL_BACKGROUND, "stage/background", -1);
    loadImage(GREEN_HILL_FOREGROUND, "stage/foreground", color(135, 16, 19));

    animations[0] = sonic_walking;
    animations[1] = sonic_running;
    animations[2] = sonic_jumping;
    animations[3] = sonic_special_positions;
}

// Fonction pour obtenir la position d'un sprite en fonction de l'action
SDL_Rect getSonicSprite(const char * action, int frame) {
    for (int i = 0; i < animationsAmount; ++i) {
        Animation * animation = animations + i;
        if (!strcmp(action, animation->name) && frame < animation->max)
            return animation->sprites[frame];
    }
    // Par défaut, retourner la première position normale
    return sonic_standing;
}
