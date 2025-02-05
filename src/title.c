#include <title.h>
#include <sprites.h>
#include <SDL_image.h>
#include <stdio.h>

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
    SDL_Surface *titleImage = getImage(TITLE_SCREEN);

    SDL_Rect playPos = {.x = width / 2 - 75, .y = height - 50, 200, 60};
    SDL_Surface * play = TTF_RenderText_Solid(font, "Press Space to play", (SDL_Color){255, 255, 255});

    SDL_Surface * bg = getImage(TITLE_SCREEN);
    SDL_Rect bgSprite = {24, 213, 1024, 112};
    SDL_Rect bgPos = {0, 0, -1, -1};
    SDL_Rect bgSprite2 = {24, 334, 1024, 144};
    SDL_Rect bgPos2 = {0, 112, -1, -1};

    int spriteWidth = width * 0.75;
    int spriteHeight = height * 0.75;
    SDL_Rect titlePos = {width / 2 - spriteWidth / 5, height / 2 - spriteHeight, spriteWidth, spriteHeight}; // Placement centré et ajustement taille

    int frame = LOOP_START; // Commence directement à la boucle finale
    SDL_Event event;
    Uint32 lastUpdate = SDL_GetTicks();
    
    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                free(play);
                return 0;
            }
            if (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_RETURN)) {
                free(play);
                return 1;
            }
        }
        
        Uint32 now = SDL_GetTicks();
        if (now - lastUpdate > 100) { // Change de frame toutes les 100ms
            frame++;
            if (frame >= FRAME_COUNT) frame = LOOP_START; // Boucle uniquement sur les 2 dernières frames
            lastUpdate = now;
        }
        
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        SDL_BlitSurface(bg, &bgSprite, screen, &bgPos);
        SDL_BlitSurface(bg, &bgSprite2, screen, &bgPos2);
        SDL_BlitSurface(titleImage, &title_sprites[frame], screen, &titlePos);
        SDL_BlitSurface(play, NULL, screen, &playPos);
        SDL_Flip(screen);
    }
}
