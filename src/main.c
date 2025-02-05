#define SDL_MAIN_HANDLED

#include <maputils.h>
#include <green_hill.h>
#include <SDL_mixer.h>

SDL_Surface * screen;

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 256;
int MAX_WIDTH = 0;

MapElement * sonic;
Mix_Music * bgMusic = NULL;

//Mise en place de l'écran
void safeQuit() {
    map_free();

    if (screen != NULL) SDL_FreeSurface(screen);
    if (bgMusic != NULL) Mix_FreeMusic(bgMusic);

    Mix_CloseAudio();
    Mix_Quit();

    SDL_Quit();
    exit(EXIT_FAILURE);
}

int getSonicFrame(int max) {
    static int frame = 0;
    static Uint32 lastUpdate = 0;
    Uint32 now = SDL_GetTicks();

    if (now - lastUpdate > 100) { // Change toutes les 100 ms (ajuste selon le besoin)
        frame = (frame + 1) % max; // Cycle entre 0 et max-0
        lastUpdate = now;
    }

    return frame;
}

int flippedX(SpriteTexture * sprite, int x) {
    return sprite->flipped ? sprite->image->w - x - sprite->sprite.w : x;
}

//Permet de retourner un sprite par exemple quand sonic passe da gauche a droite
void flipSprite(SpriteTexture * sprite) {
    sprite->flipped = !sprite->flipped;
    sprite->image = zoomSurf(sprite, -1, 1, 1);
    sprite->sprite.x = sprite->image->w - sprite->sprite.x - sprite->sprite.w;
}

//Permet d'effectuer les déplacements de Sonic
void move(MapElement * element, int x, int y) {
    element->pos.x += x;
    element->pos.y += y;
    if (element != sonic) return;

    if (element->pos.x > 300 && element->pos.x < MAX_WIDTH) {
        int dx = getOffsetX() + x;
        setOffsetX(dx < 0
                ? 0
                : dx > MAX_WIDTH
                    ? MAX_WIDTH
                    : dx);
    }
    // temporary test values
    if (element->pos.y < 20 || element->pos.y > 226) setOffsetY(getOffsetY() + y);
}

void change(MapElement * element, SDL_Rect sprite) {
    element->texture->sprite.x = flippedX(element->texture, sprite.x);
    element->texture->sprite.y = sprite.y;
    element->texture->sprite.w = sprite.w;
    element->texture->sprite.h = sprite.h;
}

//Permet de mettre de la musique
void loadMusic() {
    if(Mix_Init(MIX_INIT_MP3) != 0) {
        fprintf(stderr, "Error in Mix_Init : %s\n", Mix_GetError());
        safeQuit();
    }
    if(Mix_OpenAudio(
            MIX_DEFAULT_FREQUENCY,
            MIX_DEFAULT_FORMAT,
            MIX_DEFAULT_CHANNELS,
            MIX_DEFAULT_CHANNELS * 512) != 0) {
        fprintf(stderr, "Error in Mix_OpenAudio : %s\n", Mix_GetError());
        safeQuit();
    }
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
    map_init(screen);
    loadMusic();

    printf("Loading the game!");
    SDL_WM_SetCaption("Sonic C Hedgehog", NULL);


    bgMusic = Mix_LoadMUS("assets/sounds/green_hill_zone.mp3");
    if (bgMusic == NULL) {
        fprintf(stderr, "Error trying to load \"assets/sounds/green_hill_zone.mp3\" : %s\n", Mix_GetError());
        exit(EXIT_FAILURE);
    }
    Mix_PlayMusic(bgMusic, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME * .05);

    loadImages(screen);

    //SDL_Surface * titleScreenBg = images[TITLE_SCREEN];
    //SDL_Rect titleScreenBgSprite = getPos(24, 213, 1024, 112);
    //SDL_Rect titleScreenBgPos = getPos(0, 0, -1, -1);

    sonic = map_add(SONIC, sonic_standing, 25, 140, 10, 1);
    MAX_WIDTH = load_stage();

    int isSneaking = 0, collided = 0, isJumping = 0;
    int active = 1;
    SDL_Event event;
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    int frame;
    SDL_Rect newSprite;
    while (active) {
        SDL_WaitEvent(&event);
        SDL_FillRect(screen, NULL, color(0, 0, 0));
        switch (event.type) {
            case SDL_QUIT:
                active = 0;
                break;
            case SDL_KEYDOWN: {
                int dx = 0;
                int dy = 0;
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
                    case SDLK_SPACE:
                    case SDLK_UP:
                    case SDLK_w:
                    case SDLK_DOWN:
                    case SDLK_s:
                    case SDLK_LEFT:
                    case SDLK_a:
                    case SDLK_RIGHT:
                    case SDLK_d: {
                        switch (event.key.keysym.sym) {
                            case SDLK_SPACE:
                                if (isSneaking || collided) break;
                                change(sonic, getPos(43, 624, 32, 32));
                                isJumping = 1;
                                dy -= 10;
                                sonic->texture->sprite = getSonicSprite("jump", 0);
                            case SDLK_UP:
                            case SDLK_w:
                                if (isSneaking || collided || isJumping) break;
                                change(sonic, getPos(425, 257, 32, 40));
                                break;
                            case SDLK_DOWN:
                            case SDLK_s:
                                if (isSneaking || collided || isJumping) break;
                                change(sonic, getPos(507, 265, 32, 32));
                                move(sonic, 0, 8);
                                isSneaking = 1;
                                break;
                            case SDLK_LEFT:
                            case SDLK_a:
                                if (sonic->pos.x <= 0) break;
                                if (!sonic->texture->flipped) flipSprite(sonic->texture);
                                dx -= 10;
                                frame = getSonicFrame(6);
                                sonic->texture->sprite = getSonicSprite("walk", frame);
                                sonic->texture->sprite.x = flippedX(sonic->texture, sonic->texture->sprite.x);
                                break;
                            case SDLK_RIGHT:
                            case SDLK_d:
                                if (sonic->texture->flipped) flipSprite(sonic->texture);
                                dx += 10;
                                frame = getSonicFrame(6);
                                sonic->texture->sprite = getSonicSprite("walk", frame);
                                break;
                        }
                    }
                }

                move(sonic, dx, dy);


                MapElement * collision = element_colliding(sonic);
                if (!collision) {
                    //falling ? y+10 -> timer
                }

                if (collision) {
                    if (!collision->texture->image) { // colliding and is collision
                        int bottomY = sonic->pos.y + sonic->texture->sprite.h;

                        if (bottomY >= collision->pos.y) {
                            int height = bottomY - collision->pos.y;
                            printf("\nheight %d = %d - %d", height, bottomY, collision->pos.y);
                            if (height > 20) {
                                move(sonic, -dx, -dy);
                                break;
                            }
                            dy -= height;
                            move(sonic, 0, dy);
                        }
                        break;
                    }
/*
                    if (!collided) { // colliding and isn't collision (enemy/object)
                        collided = 1;
                        sonic->texture->sprite.w = 40;
                        sonic->texture->sprite.x = flippedX(sonic->texture, 39);
                        sonic->texture->sprite.y = 807;
                        sonic->texture->sprite.h = 32;
                        if (!isSneaking) move(sonic, 0, 8);
                    }
                */
                }
                /*
                if (!collision && collided) {
                    collided = 0;
                    sonic->texture->sprite.w = 32;
                    sonic->texture->sprite.x = flippedX(sonic->texture, 43);
                    sonic->texture->sprite.y = 257;
                    sonic->texture->sprite.h = 40;
                    move(sonic, 0, -8);
                }*/
                break;
            }
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        if (!isJumping) break;
                        frame = getSonicFrame(1);
                        sonic->texture->sprite = getSonicSprite("normal", frame);
                        break;
                    case SDLK_UP:
                    case SDLK_w:
                        if (isSneaking) break;
                    case SDLK_DOWN:
                    case SDLK_s: {
                    frame = getSonicFrame(1);
                    sonic->texture->sprite = getSonicSprite("special", frame);
                        switch (event.key.keysym.sym) {
                            case SDLK_DOWN:
                            case SDLK_s:
                                isSneaking = 0;
                                if (collided || isJumping) break;
                                sonic->texture->sprite.w = 32;
                                sonic->texture->sprite.h = 40;
                                move(sonic, 0, -8);
                                break;
                        }
                        if (collided || isJumping) break;
                        sonic->texture->sprite.x = flippedX(sonic->texture, 43);
                        sonic->texture->sprite.y = 257;
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
