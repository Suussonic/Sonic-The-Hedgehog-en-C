typedef struct {
    SpriteTexture texture;
    SDL_Rect pos;
    int collision;
    unsigned int id;
    int z;
} MapElement;

unsigned int lastID = 0;
int mapSize = 0;
MapElement * map = NULL;

void map_free() {
    if (map == NULL) return;
    free(map);
    map = NULL;
}

void element_show(MapElement element) {
    SDL_BlitSurface(element.texture.image, &element.texture.sprite, screen, &element.pos);
}

void map_show() {
    if (map == NULL) return;

    for (int i = 0; i < mapSize; ++i) {
        element_show(map[i]);
    }
}

int compareElements(const void * a, const void * b) {
    const MapElement * e0 = (MapElement*) a;
    const MapElement * e1 = (MapElement*) b;
    if (e0->z < e1->z) return 0;
    return e0->z > e1->z;
}

unsigned int map_add(enum ImageEnum image, SDL_Rect sprite, int x, int y, int z, int collision) {
    MapElement element;
    element.texture.image = images[image];
    element.texture.sprite = sprite;
    element.texture.backgroundColor = backgroundColors[image];
    element.texture.flipped = 0;
    element.pos.x = x;
    element.pos.y = y;
    element.z = z;
    element.id = lastID++;
    element.collision = collision;

    MapElement * ptr = realloc(map, sizeof (MapElement) * (mapSize+1));
    if (ptr != NULL) {
        if (map != ptr) {
            for (int i = 0; i < mapSize; ++i) {
                ptr[i] = map[i];
            }
        }
        map = ptr;
        map[mapSize++] = element;
        qsort(map, mapSize, sizeof(MapElement), compareElements);
    } else printf("Couldn't reallocate map memory!");

    return element.id;
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

MapElement * element(unsigned int id) {
    for (int i = 0; i < mapSize; ++i) {
        if (map[i].id == id)
            return map+i;
    }
    return NULL;
}

int elements_colliding(MapElement e0, MapElement e1) {
    if (!e0.collision || !e1.collision) return 0;
    if (e0.pos.x + e0.texture.sprite.w < e1.pos.x) return 0;
    if (e1.pos.x + e1.texture.sprite.w < e0.pos.x) return 0;

    if (e0.pos.y + e0.texture.sprite.h < e1.pos.y) return 0;
    if (e1.pos.y + e1.texture.sprite.h < e0.pos.y) return 0;

    return 1;
}

MapElement * element_colliding(MapElement * element) {
    if (!element->collision) return NULL;

    for (int i = 0; i < mapSize; ++i) {
        MapElement * el = map + i;
        if (el->id != element->id && elements_colliding(*element, *el))
            return el;
    }
    return NULL;
}
