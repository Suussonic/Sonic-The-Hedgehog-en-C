#include <title.h>

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

int showTitleScreen(int width, int height, SDL_Surface * screen, TTF_Font * font) {
    SDL_Rect playButton = {.x = width / 2 - 100, .y = height / 2 - 30, 200, 60};

    SDL_Surface * play = TTF_RenderText_Solid(font, "Click or press Space to play!", (SDL_Color){255, 255, 255});
    SDL_BlitSurface(play, NULL, screen, &playButton);
    SDL_Flip(screen);
    free(play);

    SDL_Event event;
    while (1) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: return 0;
                    case SDLK_RETURN:
                    case SDLK_SPACE: return 1;
                }
                break;
            case SDL_QUIT: return 0;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.x >= playButton.x && event.button.x <= playButton.x + playButton.w &&
                    event.button.y >= playButton.y && event.button.y <= playButton.y + playButton.h) {
                    return 1;
                }
                break;
        }
    }
}
