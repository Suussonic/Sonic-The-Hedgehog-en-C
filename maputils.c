typedef struct {
    SDL_Surface * image;
    SDL_Rect sprite;
    int flipped;
} SpriteTexture;

typedef struct {
    SpriteTexture texture;
    SDL_Rect pos;
    int collision;
    unsigned int id;
} MapElement;

unsigned int lastID = 0;
int mapSize = 0;
MapElement * map = NULL;

void map_free() {
    if (map == NULL) return;
    free(map);
    map = NULL;
}


MapElement map_add(SDL_Surface * image, SDL_Rect sprite, int x, int y, int collision) {
    MapElement element;
    element.texture.image = image;
    element.texture.sprite = sprite;
    element.texture.flipped = 0;
    element.pos.x = x;
    element.pos.y = y;
    element.id = lastID++;
    element.collision = collision;

    MapElement * ptr = realloc(map, sizeof (MapElement) * (mapSize+1));
    if (ptr != NULL) {
        map = ptr;
        map[mapSize++] = element;
    } else printf("Couldn't reallocate map memory!");

    return element;
}

void map_remove(MapElement element) {
    int found = 0;
    for (int i = 0; i < mapSize; ++i) {
        if (map[i].id == element.id) {
            ++found;
            --mapSize;
            continue;
        }
        if (found) map[i-1] = map[i];

    }
    MapElement * ptr = realloc(map, sizeof (MapElement) * mapSize);
    if (ptr != NULL) map = ptr;
}


int elements_colliding(MapElement e0, MapElement e1) {
    if (!e0.collision || !e1.collision) return 0;
    if (e0.pos.x + e0.texture.sprite.w < e1.pos.x) return 0;
    if (e1.pos.x + e1.texture.sprite.w < e0.pos.x) return 0;

    if (e0.pos.y + e0.texture.sprite.h < e1.pos.y) return 0;
    if (e1.pos.y + e1.texture.sprite.h < e0.pos.y) return 0;

    return 1;
}

MapElement * element_colliding(MapElement element) {
    if (!element.collision) return NULL;

    for (int i = 0; i < mapSize; ++i) {
        MapElement * el = map + i;
        if (el->id != element.id && elements_colliding(element, *el))
            return el;
    }
    return NULL;
}
