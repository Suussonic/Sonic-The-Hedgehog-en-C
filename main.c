#define SDL_MAIN_HANDLED

#include <SDL.h>

SDL_Surface * screen;
#include "sprites.c"
#include "maputils.c"

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 255;

int sonicID;

void safeQuit() {
    map_free();
    if (screen != NULL) SDL_FreeSurface(screen);
    SDL_Quit();
    exit(EXIT_FAILURE);
}


int flippedX(SpriteTexture sprite, int x) {
    return sprite.flipped ? sprite.image->w - x - sprite.sprite.w : x;
}

void flipSprite(SpriteTexture * sprite) {
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

    loadImages();

    //SDL_Surface * titleScreenBg = images[TITLE_SCREEN];
    //SDL_Rect titleScreenBgSprite = getPos(24, 213, 1024, 112);
    //SDL_Rect titleScreenBgPos = getPos(0, 0, -1, -1);

    sonicID = map_add(SONIC, getPos(43, 257, 32, 40), 25, 140, 10, 1);
    map_add(BADNIKS, getPos(173, 275, 48, 32), 200, 140, 0, 1);

    map_add(GREEN_HILL_BACKGROUND, getPos(24, 181, 1024, 32), 0, 0, -1, 0);
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 221, 1024, 16), 0, 32, -1, 0);
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 245, 1024, 16), 0, 48, -1, 0);
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 269, 1024, 48), 0, 64, -1, 0);
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 325, 1024, 40), 0, 112, -1, 0);
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 373, 1024, 104), 0, 152, -1, 0);

    int isSneaking = 0, collided = 0;

    map_add(GREEN_HILL_CHUNKS, getPos(25, 264, 255, 255), 0, 0, 0,  0);

    int active = 1;
    SDL_Event event;
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    while (active) {
        SDL_WaitEvent(&event);
        MapElement * sonic = element(sonicID);
        switch (event.type) {
            case SDL_QUIT:
                active = 0;
                break;
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    case SDLK_F11:
                        if (windowFlags == (SDL_HWSURFACE | SDL_DOUBLEBUF)) {
                            windowFlags |= SDL_FULLSCREEN;
                        } else windowFlags = SDL_HWSURFACE | SDL_DOUBLEBUF;
                        SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, windowFlags);
                        break;
                    case SDLK_ESCAPE:
                        active = 0;
                        break;
                    case SDLK_UP:
                    case SDLK_w:
                    case SDLK_DOWN:
                    case SDLK_s:
                    case SDLK_LEFT:
                    case SDLK_a:
                    case SDLK_RIGHT:
                    case SDLK_d: {
                        switch (event.key.keysym.sym) {
                            case SDLK_UP:
                            case SDLK_w:
                                if (isSneaking || collided) break;
                                sonic->texture.sprite.x = flippedX(sonic->texture, 425);
                                sonic->texture.sprite.y = 257;
                                break;
                            case SDLK_DOWN:
                            case SDLK_s:
                                if (isSneaking || collided) break;
                                sonic->texture.sprite.x = flippedX(sonic->texture, 507);
                                sonic->texture.sprite.y = 265;
                                sonic->texture.sprite.h = 32;
                                sonic->pos.y += 8;
                                isSneaking = 1;
                                break;
                            case SDLK_LEFT:
                            case SDLK_a:
                                if (!sonic->texture.flipped) flipSprite(&sonic->texture);
                                sonic->pos.x -= 10;
                                break;
                            case SDLK_RIGHT:
                            case SDLK_d:
                                if (sonic->texture.flipped) flipSprite(&sonic->texture);
                                sonic->pos.x += 10;
                                break;
                        }
                    }
                }
                MapElement * collision = element_colliding(sonic);
                if (collision && !collided) {
                    collided = 1;
                    sonic->texture.sprite.w = 40;
                    sonic->texture.sprite.x = flippedX(sonic->texture, 39);
                    sonic->texture.sprite.y = 807;
                    sonic->texture.sprite.h = 32;
                    if (!isSneaking) sonic->pos.y += 8;
                }
                if (!collision && collided) {
                    collided = 0;
                    sonic->texture.sprite.w = 32;
                    sonic->texture.sprite.x = flippedX(sonic->texture, 43);
                    sonic->texture.sprite.y = 257;
                    sonic->texture.sprite.h = 40;
                    sonic->pos.y -= 8;
                }
                break;
            }
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                    case SDLK_w:
                        if (isSneaking) break;
                    case SDLK_DOWN:
                    case SDLK_s: {
                        switch (event.key.keysym.sym) {
                            case SDLK_DOWN:
                            case SDLK_s:
                                isSneaking = 0;
                                if (collided) break;
                                sonic->texture.sprite.w = 32;
                                sonic->texture.sprite.h = 40;
                                sonic->pos.y -= 8;
                                break;
                        }
                        if (collided) break;
                        sonic->texture.sprite.x = flippedX(sonic->texture, 43);
                        sonic->texture.sprite.y = 257;
                    }
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

        map_show();
        SDL_Flip(screen);
    }
    safeQuit();
}
