#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_mixer.h>

SDL_Surface * screen;
#include "sprites.c"
#include "maputils.c"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;
int MAX_WIDTH = 0;

int sonicID;
Mix_Music * bgMusic = NULL;

// Animation state for Sonic
typedef struct {
    SDL_Rect frames[8]; // Array of frames for animation
    int currentFrame;   // Current frame index
    Uint32 lastUpdateTime; // Time of last frame update
    int frameDelay;        // Milliseconds between frames
} Animation;

Animation sonicAnimation;

// Initialisation des frames d'animation
void initSonicAnimation() {
    for (int i = 0; i < 8; i++) {
        sonicAnimation.frames[i].x = i * 32; // Chaque frame mesure 32px de large
        sonicAnimation.frames[i].y = 0;     // Ligne de l'animation
        sonicAnimation.frames[i].w = 32;    // Largeur d'une frame
        sonicAnimation.frames[i].h = 40;    // Hauteur d'une frame
    }
    sonicAnimation.currentFrame = 0;
    sonicAnimation.lastUpdateTime = 0;
    sonicAnimation.frameDelay = 100; // 100 ms entre les frames
}

// Mise à jour de l'animation
void updateSonicAnimation() {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > sonicAnimation.lastUpdateTime + sonicAnimation.frameDelay) {
        sonicAnimation.currentFrame = (sonicAnimation.currentFrame + 1) % 8;
        sonicAnimation.lastUpdateTime = currentTime;
    }
}

// Rendu de Sonic avec la frame courante
void renderSonic(SDL_Surface *screen, SDL_Rect sonicPos) {
    SDL_BlitSurface(images[SONIC], &sonicAnimation.frames[sonicAnimation.currentFrame], screen, &sonicPos);
}

// Fonction de sortie sécurisée
void safeQuit() {
    map_free();

    if (screen != NULL) SDL_FreeSurface(screen);
    if (bgMusic != NULL) Mix_FreeMusic(bgMusic);

    Mix_CloseAudio();
    Mix_Quit();

    SDL_Quit();
    exit(EXIT_FAILURE);
}

int flippedX(SpriteTexture sprite, int x) {
    return sprite.flipped ? sprite.image->w - x - sprite.sprite.w : x;
}

// Permet de retourner un sprite par exemple quand sonic passe de gauche à droite
void flipSprite(SpriteTexture * sprite) {
    sprite->flipped = !sprite->flipped;
    sprite->image = zoomSurf(sprite, -1, 1, 1);
    sprite->sprite.x = sprite->image->w - sprite->sprite.x - sprite->sprite.w;
}

// Permet de créer une instance de SDL_Rect pour la position
SDL_Rect getPos(int x, int y, int w, int h) {
    SDL_Rect pos;
    pos.x = x;
    pos.y = y;
    if (w != -1) pos.w = w;
    if (h != -1) pos.h = h;
    return pos;
}

// Permet d'effectuer les déplacements de Sonic
void move(MapElement * element, int x, int y) {
    element->pos.x += x;
    element->pos.y += y;
    if (element->id != sonicID) return;

    if (element->pos.x > 300 && element->pos.x < MAX_WIDTH)
        dx = dx + x < 0
                ? 0
                : dx + x > MAX_WIDTH
                    ? MAX_WIDTH
                    : dx + x;
}

// Permet de mettre de la musique
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

    loadMusic();

    printf("Loading the game!");
    SDL_WM_SetCaption("Sonic C Hedgehog", NULL);

    bgMusic = Mix_LoadMUS("sound/green_hill_zone.mp3");
    if (bgMusic == NULL) {
        fprintf(stderr, "Error trying to load \"sound/green_hill_zone.mp3\" : %s\n", Mix_GetError());
        exit(EXIT_FAILURE);
    }
    Mix_PlayMusic(bgMusic, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME * .05);

    loadImages();
    initSonicAnimation();

    sonicID = map_add(SONIC, getPos(43, 257, 32, 40), 25, 140, 10, 1);
    map_add(BADNIKS, getPos(173, 275, 48, 32), 200, 140, 0, 1);

    map_add(GREEN_HILL_BACKGROUND, getPos(24, 181, 3840, 32), 0, 0, -2, 0); // weird hack I came up with to not have missing background
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 181, 3840, 32), 0, 0, -1, 0);
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 221, 3840, 16), 0, 32, -1, 0);
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 245, 3840, 16), 0, 48, -1, 0);
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 269, 3840, 48), 0, 64, -1, 0);
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 325, 3840, 40), 0, 112, -1, 0);
    map_add(GREEN_HILL_BACKGROUND, getPos(24, 373, 3840, 104), 0, 152, -1, 0);

    int isSneaking = 0, collided = 0;

    const int chunks = 10;
    for (int i = 0; i < chunks; ++i) {
        map_add(GREEN_HILL_CHUNKS, getPos(
                24 + (256 + 8) * (i % 5),
                264 + (256 + 8) * ((i-1) / 5),
                256, 256
        ),256 * i, 0, 0,  0);
    }
    MAX_WIDTH = (chunks - 2) * 265 - 73; // don't ask me, I have no idea

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
                                move(sonic, 0, -10);
                                break;
                            case SDLK_DOWN:
                            case SDLK_s:
                                move(sonic, 0, 10);
                                if (isSneaking || collided) break;
                                sonic->texture.sprite.x = flippedX(sonic->texture, 507);
                                sonic->texture.sprite.y = 265;
                                sonic->texture.sprite.h = 32;
                                move(sonic, 0, 8);
                                isSneaking = 1;
                                break;
                            case SDLK_LEFT:
                            case SDLK_a:
                                if (sonic->pos.x <= 0) break;
                                if (!sonic->texture.flipped) flipSprite(&sonic->texture);
                                move(sonic, -10, 0);
                                break;
                            case SDLK_RIGHT:
                            case SDLK_d:
                                if (sonic->texture.flipped) flipSprite(&sonic->texture);
                                move(sonic, 10, 0);
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
                    if (!isSneaking) move(sonic, 0, 8);
                }
                if (!collision && collided) {
                    collided = 0;
                    sonic->texture.sprite.w = 32;
                    sonic->texture.sprite.x = flippedX(sonic->texture, 43);
                    sonic->texture.sprite.y = 257;
                    sonic->texture.sprite.h = 40;
                    move(sonic, 0, -8);
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
                                move(sonic, 0, -8);
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

        updateSonicAnimation();
        renderSonic(screen, sonic->pos);
        map_show();
        SDL_Flip(screen);
    }
    safeQuit();
}
