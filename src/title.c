#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== Bienvenue ===\n");
    printf("Appuyez sur Entrée pour lancer le programme principal...\n");

    // Attendre que l'utilisateur appuie sur Entrée
    getchar();

    // Lancer le programme principal
    int status = system("./main");
    
    // Vérifier si l'exécution de main s'est bien passée
    if (status == -1) {
        perror("Erreur lors de l'exécution de main");
        return 1;
    }

    return 0;
}
