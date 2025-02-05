#define SDL_MAIN_HANDLED

#include <maputils.h>
#include <green_hill.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <title.h>

#ifdef _WIN32
#include <windows.h>
#endif
#ifdef linux
#include <unistd.h>
#endif

SDL_Surface * screen;

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 256;
int MAX_WIDTH = 0;

MapElement * sonic;
Mix_Music * bgMusic = NULL;
TTF_Font * font = NULL;

//Mise en place de l'écran
void safeQuit() {
    map_free();
    freeImages();

    if (screen != NULL) SDL_FreeSurface(screen);
    if (bgMusic != NULL) Mix_FreeMusic(bgMusic);
    if (font != NULL) TTF_CloseFont(font);

    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();

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
    element->texture->sprite.x = element->texture->flipped ? element->texture->image->w - sprite.x - sprite.w : sprite.x;
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

void loadFont(char * path) {
    font = TTF_OpenFont(path, 16);
    if (font != NULL) return;
    fprintf(stderr, "Error while loading font \"%s\"\n", path);
    TTF_Quit();
    safeQuit();
}

int rings = 0;
int isSneaking = 0, collided = 0, isJumping = 0, isFalling = 0;
int jumpHeight = 0, kickYeet = 0;
int cameraLocked = 1; 
int initialY;
int frame;

SDL_Rect ringRect = {.x = 24, .y = 198, .w = 16, .h = 16};
SDL_Rect ringPos = {.x = WINDOW_WIDTH-40, .y = 5};
SDL_Rect ringsTextPos = {.x = WINDOW_WIDTH-50, .y = 5};
char ringsStr[3];

void setRings(int amount) {
    rings = amount > 999 ? 999 : amount;
    itoa(rings, ringsStr, 10);
    ringsTextPos.x = WINDOW_WIDTH - 40 - strlen(ringsStr) * 10;
}

void screen_show() {
    map_show();
    SDL_Surface * ringsText = TTF_RenderText_Solid(font, ringsStr, (SDL_Color){255, 255, 255});
    SDL_BlitSurface(ringsText, NULL, screen, &ringsTextPos);
    free(ringsText);
    SDL_BlitSurface(getImage(OBJECTS), &ringRect, screen, &ringPos);
    SDL_Flip(screen);
}

void stand() {
    change(sonic, sonic_standing);
    screen_show();
}

Uint32 fall(Uint32 interval, void * param) {
    MapElement * collision = element_colliding(sonic);

    if (collision) {
        if (collision->texture->image != NULL) {
            map_remove(collision);
            if (collision->texture->image == getImage(OBJECTS)) {
                setRings(rings+1);
            }
            return interval;
        }

        sonic->pos.y = collision->pos.y - sonic->texture->sprite.h;
        setOffsetY(sonic->pos.y - WINDOW_HEIGHT / 2);
        isFalling = 0;
        stand();

        // 🔹 Rebloquer la caméra si Sonic est revenu à sa position initiale
        if (sonic->pos.y >= initialY) {
            cameraLocked = 1;
        }

        return 0;
    }

    move(sonic, 0, 10);

    // 🔹 Si la caméra est débloquée, elle suit Sonic vers le bas
    if (!cameraLocked) {
        setOffsetY(getOffsetY() + 10);
    }

    if (sonic->pos.y > 500) { // Si Sonic tombe hors de l'écran
        printf("\nrip");
        sonic->pos.x = 25;
        sonic->pos.y = 140;
        setOffsetX(0);
        setOffsetY(0);
        cameraLocked = 1; // 🔹 Réinitialisation de la caméra
    }

    screen_show();
    return interval;
}

Uint32 damage(Uint32 interval, void * param) {
    if (kickYeet <= 0) {
        collided = 0;
        isFalling = 1;
        SDL_SetTimer(0, NULL);
        SDL_AddTimer(40, fall, NULL);
        return 0;
    }

    --kickYeet;
    move(sonic, 10 * (*(int*)param ? 1 : -1), -5);
    screen_show();
    return interval;
}

Uint32 jump(Uint32 interval, void * param) {
    MapElement * collision = element_colliding(sonic);

    frame = getSonicFrame(5);
    change(sonic, getSonicSprite("jump", frame));

    if (jumpHeight <= 0 || collision) {
        isFalling = 1;
        isJumping = 0;
        jumpHeight = 0;
        SDL_SetTimer(0, NULL);
        SDL_AddTimer(40, fall, NULL);
        return 0;
    }

    jumpHeight -= 10;
    if (sonic->pos.y <= initialY - 2 * sonic->texture->sprite.h) {
        cameraLocked = 0;
    }

    if (!cameraLocked) {
        setOffsetY(getOffsetY() - 10); // La caméra monte avec Sonic
    }
    move(sonic, 0, -10);

    screen_show();
    return interval;
}

void app_sleep(int ms) {
    #ifdef _WIN32
        Sleep(ms);
    #endif
    #ifdef linux
    usleep(ms);
    #endif
}
void loop(Uint32 windowFlags); // Déclaration de la fonction loop

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

    TTF_Init();
    loadFont("assets/arial.ttf");

    loadImages(screen);
    if (!showTitleScreen(WINDOW_WIDTH, WINDOW_HEIGHT, screen, font)) safeQuit();

    bgMusic = Mix_LoadMUS("assets/sounds/green_hill_zone.mp3");
    if (bgMusic == NULL) {
        fprintf(stderr, "Error trying to load \"assets/sounds/green_hill_zone.mp3\" : %s\n", Mix_GetError());
        exit(EXIT_FAILURE);
    }
    Mix_PlayMusic(bgMusic, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME * .05);

    setRings(0);
    sonic = map_add(SONIC, sonic_standing, 25, 160, 10, 1);
    MAX_WIDTH = load_stage();

    do {
        loop(windowFlags);
        SDL_EnableKeyRepeat(0, 0);
        app_sleep(40);
    } while (showTitleScreen(WINDOW_WIDTH, WINDOW_HEIGHT, screen, font));

    safeQuit();
}

void loop(Uint32 windowFlags) {
    int active = 1;
    SDL_Event event;
    SDL_EnableKeyRepeat(30, SDL_DEFAULT_REPEAT_INTERVAL);
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
                                if (isSneaking || collided || isJumping || isFalling) break;
                                change(sonic, getPos(43, 624, 32, 32));
                                isJumping = 1;
                                dy -= 10;
                                sonic->texture->sprite = getSonicSprite("jump", 0);
                                jumpHeight = 100;

                                // 🔹 Stocke la position Y initiale avant le saut
                                initialY = sonic->pos.y;

                                SDL_AddTimer(30, jump, NULL);
                                break;
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
                                if (sonic->pos.x <= 0 || collided) break;
                                if (!sonic->texture->flipped) flipSprite(sonic->texture);
                                dx -= 10;
                                if (isJumping || isFalling) break;
                                frame = getSonicFrame(6);
                                change(sonic, getSonicSprite("walk", frame));
                                break;
                            case SDLK_RIGHT:
                            case SDLK_d:
                                if (collided) break;
                                if (sonic->texture->flipped) flipSprite(sonic->texture);
                                dx += 10;
                                if (isJumping || isFalling) break;
                                frame = getSonicFrame(6);
                                change(sonic, getSonicSprite("walk", frame));
                                break;
                        }
                    }
                }

                move(sonic, dx, dy);


                MapElement * collision = element_colliding(sonic);
                if (collision) {
                    if (!collision->texture->image || !isFalling && !isJumping && collision->texture->image == getImage(OBJECTS)) { // colliding and is collision or object
                        int bottomY = sonic->pos.y + sonic->texture->sprite.h;

                        if (bottomY >= collision->pos.y) {
                            int height = bottomY - collision->pos.y;
                            if (height > 20) {
                                move(sonic, -dx, -dy);
                                break;
                            }
                            dy -= height;
                            move(sonic, 0, dy);
                            if (isFalling) {
                                isFalling = 0;
                                change(sonic, sonic_standing);
                            }
                        }
                        break;
                    }

                    if (!collided) { // colliding and isn't collision (enemy/object)
                        if (collision->texture->image == getImage(OBJECTS)) break;
                        if (isJumping || isFalling) break;
                        if (isSneaking) {
                            isSneaking = 0;
                            move(sonic, 0, -8);
                        }
                        collided = 1;
                        change(sonic, getPos(39, 807, 40, 32));

                        //loose rings
                        kickYeet = 10;
                        int direction = sonic->pos.x - collision->pos.x > 0;
                        SDL_AddTimer(30, damage, &direction);
                    }
                }

                if (isFalling || isJumping) break;

                move(sonic, 0, -10);
                if (element_colliding(sonic)) {
                    move(sonic, 0, 10);
                    break;
                }
                isFalling = 1;
                SDL_AddTimer(40, fall, NULL);
                break;
            }
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                    case SDLK_a:
                    case SDLK_RIGHT:
                    case SDLK_d:
                        if (isJumping || isSneaking) break;
                        change(sonic, sonic_standing);
                        break;
                    case SDLK_UP:
                    case SDLK_w:
                        if (isSneaking) break;
                    case SDLK_DOWN:
                    case SDLK_s: {
                        switch (event.key.keysym.sym) {
                            case SDLK_DOWN:
                            case SDLK_s:
                                if (!isSneaking) break;
                                isSneaking = 0;
                                if (collided || isJumping || isFalling) break;
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

        screen_show();
    }
}