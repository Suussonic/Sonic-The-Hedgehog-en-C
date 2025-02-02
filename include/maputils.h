#ifndef DEF_HEADER_MAPUTILS_LIB
#define DEF_HEADER_MAPUTILS_LIB

#include <sprites.h>

typedef struct MapElement {
    SpriteTexture * texture;
    SDL_Rect pos;
    int collision;
    int z;
} MapElement;


void map_init(SDL_Surface * screen);
void map_free();
void map_show();
void element_show(MapElement * element);

void setOffsetX(int offset);
int getOffsetX();
void setOffsetY(int offset);
int getOffsetY();

MapElement * map_add(ImageEnum image, SDL_Rect sprite, int x, int y, int z, int collision);
void map_add_collision(SDL_Rect square, int z);
void map_remove(MapElement * element);

int elements_colliding(MapElement * e0, MapElement * e1);
MapElement * element_colliding(MapElement * element);

SDL_Rect getPos(int x, int y, int w, int h);

#endif