#include <title.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#define FRAME_COUNT 18
#define LOOP_START 16 // Répétition des 2 dernières frames

SDL_Rect title_sprites[FRAME_COUNT] = {
    {24, 520, 318, 222}, {352, 520, 318, 222}, {680, 520, 318, 222},
    {24, 752, 318, 222}, {352, 752, 318, 222}, {680, 752, 318, 222},
    {24, 984, 318, 222}, {352, 984, 318, 222}, {680, 984, 318, 222},
    {24, 1216, 318, 222}, {352, 1216, 318, 222}, {680, 1216, 318, 222},
    {24, 1448, 318, 222}, {352, 1448, 318, 222}, {680, 1448, 318, 222},
    {24, 1680, 318, 222}, {352, 1680, 318, 222}, {680, 1680, 318, 222}
};

int showTitleScreen(int width, int height, SDL_Surface *screen, TTF_Font *font) {
    SDL_Surface *titleImage = IMG_Load("assets/sprites/miscellaneous/Title Screen.png");
    if (!titleImage) {
        fprintf(stderr, "Erreur de chargement de l'image du titre : %s\n", IMG_GetError());
        return 0;
    }
    
    SDL_SetColorKey(titleImage, SDL_TRUE, SDL_MapRGB(titleImage->format, 16, 112, 132)); // Supprime la couleur du cadre
    
    int spriteWidth = width * 0.75;
    int spriteHeight = height * 0.75;
    SDL_Rect titlePos = {width / 2 - spriteWidth / 2, height / 2 - spriteHeight / 2, spriteWidth, spriteHeight}; // Placement centré et ajustement taille
    
    int frame = LOOP_START; // Commence directement à la boucle finale
    int running = 1;
    SDL_Event event;
    Uint32 lastUpdate = SDL_GetTicks();
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                running = 0;
            }
        }
        
        Uint32 now = SDL_GetTicks();
        if (now - lastUpdate > 100) { // Change de frame toutes les 100ms
            frame++;
            if (frame >= FRAME_COUNT) frame = LOOP_START; // Boucle uniquement sur les 2 dernières frames
            lastUpdate = now;
        }
        
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        SDL_BlitSurface(titleImage, &title_sprites[frame], screen, &titlePos);
        SDL_Flip(screen);
    }
    
    SDL_FreeSurface(titleImage);
    return 1;
}
