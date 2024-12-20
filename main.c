#define SDL_MAIN_HANDLED

#include <SDL.h>
#include "sprites.c"

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 255;

SDL_Surface * screen;

Sprite sonic;

void safeQuit() {
    if (screen != NULL) SDL_FreeSurface(screen);
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void showImage(SDL_Surface * image, SDL_Rect spritePos, SDL_Rect screenPos) {
    SDL_BlitSurface(image, &spritePos, screen, &screenPos);
}

int flippedX(Sprite sprite, int x) {
    return sprite.flipped ? sprite.image->w - x - sprite.sprite.w : x;
}

void flipSprite(Sprite * sprite) {
    sprite->flipped = !sprite->flipped;
    sprite->image = zoomSurf(sprite, -1, 1, 1);
    sprite->sprite.x = sprite->image->w - sprite->sprite.x - sprite->sprite.w;
}

SDL_Rect getPos(int x, int y, int w, int h) {
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    if (w != -1) pos.w = w;
    if (h != -1) pos.h = h;
    return pos;
}

int main(int argc, char * argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr,"Error in SDL_Init : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    Uint32 windowFlags = SDL_HWSURFACE | SDL_DOUBLEBUF;
    screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, windowFlags);
    if (screen == NULL) {
        fprintf(stderr,"Error in SDL_SetVideoMode : %s\n", SDL_GetError());
        safeQuit();
    }

    SDL_WM_SetCaption("Sonic C Hedgehog", NULL);

    //addRect(screen, 50, 50, 200, 100, getRGB(64, 118, 173));
    //addRect(screen, 200, 50, 100, 200, getRGB(64, 140, 110));

    loadImages(screen);

    //SDL_Surface * titleScreenBg = images[TITLE_SCREEN];
    //SDL_Rect titleScreenBgSprite = getPos(24, 213, 1024, 112);
    //SDL_Rect titleScreenBgPos = getPos(0, 0, -1, -1);

    SDL_Surface * background = images[GREEN_HILL_BACKGROUND];
    SDL_Rect backgroundSprite0 = getPos(24, 181, 1024, 32);
    SDL_Rect backgroundPos0 = getPos(0, 0, -1, -1);
    SDL_Rect backgroundSprite1 = getPos(24, 221, 1024, 16);
    SDL_Rect backgroundPos1 = getPos(0, 32, -1, -1);
    SDL_Rect backgroundSprite2 = getPos(24, 245, 1024, 16);
    SDL_Rect backgroundPos2 = getPos(0, 48, -1, -1);
    SDL_Rect backgroundSprite3 = getPos(24, 269, 1024, 48);
    SDL_Rect backgroundPos3 = getPos(0, 64, -1, -1);
    SDL_Rect backgroundSprite4 = getPos(24, 325, 1024, 40);
    SDL_Rect backgroundPos4 = getPos(0, 112, -1, -1);
    SDL_Rect backgroundSprite5 = getPos(24, 373, 1024, 104);
    SDL_Rect backgroundPos5 = getPos(0, 152, -1, -1);

    sonic = setSprite(SONIC, getPos(43, 257, 32, 40), getPos(25, 140, -1, -1));
    int isSneaking = 0;

    SDL_Surface * GreenHillChunks = images[GREEN_HILL_CHUNKS];
    SDL_Rect GreenHillSprite = getPos(25, 264, 255, 255);
    SDL_Rect GreenHillPos = getPos(0, 0, -1, -1);

    int active = 1;
    SDL_Event event;
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    while (active) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                active = 0;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_F11:;
                        if (windowFlags == (SDL_HWSURFACE | SDL_DOUBLEBUF)) {
                            windowFlags |= SDL_FULLSCREEN;
                        } else windowFlags = SDL_HWSURFACE | SDL_DOUBLEBUF;
                        SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, windowFlags);
                        break;
                    case SDLK_ESCAPE:
                        active = 0;
                        break;
                    case SDLK_UP:
                    case SDLK_z:
                    case SDLK_DOWN:
                    case SDLK_s:
                    case SDLK_LEFT:
                    case SDLK_q:
                    case SDLK_RIGHT:
                    case SDLK_d:
                        switch (event.key.keysym.sym) {
                            case SDLK_UP:
                            case SDLK_z:
                                if (isSneaking) break;
                                sonic.sprite.x = flippedX(sonic,425);
                                sonic.sprite.y = 257;
                                break;
                            case SDLK_DOWN:
                            case SDLK_s:
                                if (isSneaking) break;
                                sonic.sprite.x = flippedX(sonic, 507);
                                sonic.sprite.y = 265;
                                sonic.sprite.h = 32;
                                sonic.pos.y += 8;
                                isSneaking = 1;
                                break;
                            case SDLK_LEFT:
                            case SDLK_q:
                                if (!sonic.flipped) flipSprite(&sonic);
                                sonic.pos.x -= 10;
                                break;
                            case SDLK_RIGHT:
                            case SDLK_d:
                                if (sonic.flipped) flipSprite(&sonic);
                                sonic.pos.x += 10;
                        }
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                    case SDLK_z:
                        if (isSneaking) break;
                    case SDLK_DOWN:
                    case SDLK_s:
                        switch (event.key.keysym.sym) {
                            case SDLK_DOWN:
                            case SDLK_s:
                                sonic.sprite.w = 32;
                                sonic.sprite.h = 40;
                                sonic.pos.y -= 8;
                                isSneaking = 0;
                                break;
                        }
                        sonic.sprite.x = flippedX(sonic, 43);
                        sonic.sprite.y = 257;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                break;
            case SDL_MOUSEBUTTONUP:
                break;
            case SDL_MOUSEMOTION:
                break;
            default: break;
        }

        //showImage(titleScreenBg, titleScreenBgSprite, titleScreenBgPos);
        showImage(background, backgroundSprite0, backgroundPos0);
        showImage(background, backgroundSprite1, backgroundPos1);
        showImage(background, backgroundSprite2, backgroundPos2);
        showImage(background, backgroundSprite3, backgroundPos3);
        showImage(background, backgroundSprite4, backgroundPos4);
        showImage(background, backgroundSprite5, backgroundPos5);
        showImage(GreenHillChunks, GreenHillSprite, GreenHillPos);
        showImage(sonic.image, sonic.sprite, sonic.pos);
        SDL_Flip(screen);
    }
    SDL_Quit();
    exit(EXIT_SUCCESS);
}
