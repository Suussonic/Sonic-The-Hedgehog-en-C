#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char *argv[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Erreur SDL_Init : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Création de la fenêtre
    SDL_Window *window = SDL_CreateWindow("Title Screen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Création du rendu
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    // Couleurs
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
    SDL_RenderClear(renderer);

    // Création du bouton Play
    SDL_Rect playButton = {WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 30, 200, 60}; // Position et taille du bouton
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Couleur verte
    SDL_RenderFillRect(renderer, &playButton);
    SDL_RenderPresent(renderer);

    int running = 1;
    SDL_Event event;
    
    // Boucle d'événements
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.x >= playButton.x && event.button.x <= playButton.x + playButton.w &&
                        event.button.y >= playButton.y && event.button.y <= playButton.y + playButton.h) {
                        system("./game"); // Exécute le fichier du jeu (à compiler séparément)
                        running = 0;
                    }
                    break;
            }
        }
    }

    // Nettoyage SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
