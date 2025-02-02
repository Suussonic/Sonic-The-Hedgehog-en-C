#include <maputils.h>

#include <stdio.h>
#include <stdlib.h>

int mapSize = 0;
SDL_Surface * mapScreen = NULL;
MapElement ** map = NULL;
int dx = 0, dy = 0;

void map_init(SDL_Surface * screen) {
    mapScreen = screen;
}

void map_free() {
    if (map == NULL) return;
    free(map);
    map = NULL;
}

void element_show(MapElement * element) {
    if (element->texture->image == NULL) return;

    SDL_Rect pos = element->pos;
    if (element->texture->image != getImage(GREEN_HILL_BACKGROUND)) {
        pos.x -= dx;
        pos.y -= dy;
    }

    SDL_BlitSurface(element->texture->image, &element->texture->sprite, mapScreen, &pos);
}

void map_show() {
    if (map == NULL) return;

    for (int i = 0; i < mapSize; ++i) {
        element_show(map[i]);
    }
}

void setOffsetX(int offset) {
    dx = offset;
}
int getOffsetX() {
    return dx;
}
void setOffsetY(int offset) {
    dy = offset;
}
int getOffsetY() {
    return dy;
}

int compareElements(const void * a, const void * b) {
    const MapElement * e0 = *(MapElement**) a;
    const MapElement * e1 = *(MapElement**) b;
    if (e0->z < e1->z) return 0;
    return e0->z > e1->z;
}

MapElement * map_add(ImageEnum image, SDL_Rect sprite, int x, int y, int z, int collision) {
    MapElement * element = malloc(sizeof (MapElement));
    SpriteTexture * spriteTexture = malloc(sizeof (SpriteTexture));
    if (element == NULL || spriteTexture == NULL) {
        fprintf(stderr, "An error occured while adding a new element with image %d to the map", image);
        return NULL;
    }

    element->texture = spriteTexture;
    element->texture->image = NULL;
    if (image != -1) {
        element->texture->image = getImage(image);
        element->texture->backgroundColor = getBackGroundColor(image);
    }
    element->texture->sprite = sprite;
    element->texture->flipped = 0;

    SDL_Rect pos = {.x = x, .y = y};
    element->pos = pos;
    element->z = z;
    element->collision = collision;
    MapElement ** ptr = malloc(sizeof (MapElement*) * (mapSize+1));
    if (ptr != NULL) {
        if (map != ptr) {
            for (int i = 0; i < mapSize; ++i) {
                ptr[i] = map[i];
            }
        }
        free(map);
        map = ptr;
        map[mapSize++] = element;
        qsort(map, mapSize, sizeof(MapElement*), compareElements);
    } else fprintf(stderr, "Couldn't reallocate map memory!");
    return element;
}

void map_add_collision(SDL_Rect square, int z) {
    map_add(-1, square, square.x, square.y, z, 1);
}


void map_remove(MapElement * element) {
    int found = 0;
    for (int i = 0; i < mapSize; ++i) {
        if (map[i] == element) {
            ++found;
            --mapSize;
            continue;
        }
        if (found) map[i-1] = map[i];

    }
    MapElement ** ptr = realloc(map, sizeof (MapElement*) * mapSize);
    if (ptr != NULL) map = ptr;
}

int elements_colliding(MapElement * e0, MapElement * e1) {
    if (!e0->collision || !e1->collision) return 0;
    if (e0->pos.x + e0->texture->sprite.w < e1->pos.x) return 0;
    if (e1->pos.x + e1->texture->sprite.w < e0->pos.x) return 0;

    if (e0->pos.y + e0->texture->sprite.h < e1->pos.y) return 0;
    if (e1->pos.y + e1->texture->sprite.h < e0->pos.y) return 0;

    return 1;
}

MapElement * element_colliding(MapElement * element) {
    if (!element->collision) return NULL;

    for (int i = 0; i < mapSize; ++i) {
        MapElement * el = map[i];
        if (el != element && elements_colliding(element, el))
            return el;
    }
    return NULL;
}
